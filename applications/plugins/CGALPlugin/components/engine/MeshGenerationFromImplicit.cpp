/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2017 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include <fstream>
#include <sofa/core/ObjectFactory.h>

/// Misc
#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/objectmodel/Link.h>

/// Mesh Volume
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>
#include <CGAL/Implicit_to_labeling_function_wrapper.h>
#include <CGAL/Labeled_mesh_domain_3.h>
#include <CGAL/make_mesh_3.h>
#include <CGAL/Bbox_3.h>

#include "MeshGenerationFromImplicit.h"


namespace sofa
{
namespace component
{
namespace engine
{
namespace _meshgenerationfromimplicit_
{

using namespace CGAL::parameters;
using namespace sofa::core;
using namespace sofa::core::visual;
using namespace sofa::core::objectmodel;
using sofa::defaulttype::Vec3d ;

// Domain
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

class ImplicitFunction : public std::unary_function<K::Point_3, K::FT> {

private:
    ScalarField* m_shape;

public:
    typedef result_type return_type;
    typedef K::Point_3 Point;
    ImplicitFunction(ScalarField* shape) {m_shape=shape;}
    K::FT operator()(Point p) const
    {
        Vec3d tmp(p.x(),p.y(),p.z());
        return m_shape->getValue(tmp);
    }
};

typedef CGAL::Implicit_multi_domain_to_labeling_function_wrapper<ImplicitFunction> Function_wrapper;
typedef Function_wrapper::Function_vector Function_vector;
typedef CGAL::Labeled_mesh_domain_3<Function_wrapper, K> Mesh_domain;
//Triangulation
typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;
typedef typename Tr::Finite_vertices_iterator Finite_vertices_iterator;
typedef typename Tr::Point Point_3;
typedef typename C3t3::Cell_iterator Cell_iterator;
typedef typename Tr::Vertex_handle Vertex_handle;
//Mesh Criteria
typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;
typedef Mesh_criteria::Facet_criteria Facet_criteria;
typedef Mesh_criteria::Cell_criteria Cell_criteria;


//factory register
int MeshGenerationFromImplicitShapeClass = RegisterObject("This component mesh a domain with a distance grid").add< MeshGenerationFromImplicitShape >();

//create a bbox 3 for meshing in volume
CGAL::Bbox_3 MeshGenerationFromImplicitShape::BoundingBox(double x_min, double y_min, double z_min, double x_max, double y_max, double z_max) {

    CGAL::Bbox_3 bbox(x_min,y_min,z_min,x_max,y_max,z_max);
    return bbox;

}

MeshGenerationFromImplicitShape::MeshGenerationFromImplicitShape()
    : in_facetsize(initData(&in_facetsize,"facet_size","size of facet"))
    , in_approximation(initData(&in_approximation,"approximation","approximation"))
    , in_cellsize(initData(&in_cellsize,"cell_size","size of cell"))
    , xmin_box(initData(&xmin_box,0.0,"xmin_box","xmin of bbox"))
    , ymin_box(initData(&ymin_box,0.0,"ymin_box","ymin of bbox"))
    , zmin_box(initData(&zmin_box,-5.0,"zmin_box","zmin of bbox"))
    , xmax_box(initData(&xmax_box,27.0,"xmax_box","xmax of bbox"))
    , ymax_box(initData(&ymax_box,27.0,"ymax_box","ymax of bbox"))
    , zmax_box(initData(&zmax_box,5.0,"zmax_box","zmax of bbox"))
    , drawTetras(initData(&drawTetras,false,"drawTetras","display generated tetra mesh"))
    , out_Points(initData(&out_Points, "outputPoints", "position coordinates from the tetrahedral generation"))
    , out_tetrahedra(initData(&out_tetrahedra, "outputTetras", "list of tetrahedra"))
    , in_scalarfield(initLink("scalarfield", "The scalar field that defined the iso-function"))
{
}

void MeshGenerationFromImplicitShape::init() {

    if(in_scalarfield.empty() == false)
    {
        if(in_scalarfield->isComponentStateValid() == false) {
            m_componentstate = ComponentState::Invalid;
            msg_error() << "Previous component invalid";
        }
        volumeMeshGeneration(in_facetsize.getValue(),in_approximation.getValue(),in_cellsize.getValue());
    }
    else
    {
        m_componentstate = ComponentState::Invalid;
        msg_error() << "Component non linked";
    }

}




//mesh the implicit domain and export it to vtu file
int MeshGenerationFromImplicitShape::volumeMeshGeneration(float facet_size, float approximation, float cell_size) {

    if(m_componentstate == ComponentState::Invalid)
        return 0;

    //Domain
    Mesh_domain *domain = NULL;
    if(in_scalarfield.empty())
        return 0;

    ImplicitFunction function(in_scalarfield);
    Function_vector v;
    v.push_back(function);
    domain = new Mesh_domain(v, K::Sphere_3(CGAL::ORIGIN, 5. *5.), 1e-3);

    //Criteria
    Facet_criteria facet_criteria(30, facet_size, approximation); // angle, size, approximation
    Cell_criteria cell_criteria(2., cell_size); // radius-edge ratio, size
    Mesh_criteria criteria(facet_criteria, cell_criteria);

    //Mesh generation
    C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(*domain, criteria, no_exude(), no_perturb());
    delete domain;

    //Topology
    sofa::helper::WriteAccessor< Data<VecCoord> > newPoints = out_Points;
    sofa::helper::WriteAccessor< Data<SeqTetrahedra> > tetrahedra = out_tetrahedra;
    Tr& tr = c3t3.triangulation();
    std::map<Vertex_handle, int> Vnbe;
    for( Cell_iterator cit = c3t3.cells_begin() ; cit != c3t3.cells_end() ; ++cit )
    {
        for (int i=0; i<4; i++)
            ++Vnbe[cit->vertex(i)];
    }
    std::map<Vertex_handle, int> V;
    int inum = 0;
    int notconnected = 0;
    Coord bbmin, bbmax;
    for( Finite_vertices_iterator vit = tr.finite_vertices_begin(); vit != tr.finite_vertices_end(); ++vit)
    {
        Point_3 pointCgal = vit->point();
        Coord p;
        p[0] = CGAL::to_double(pointCgal.x());
        p[1] = CGAL::to_double(pointCgal.y());
        p[2] = CGAL::to_double(pointCgal.z());
        if (Vnbe.find(vit) == Vnbe.end() || Vnbe[vit] <= 0)
        {
            ++notconnected;
        }
        else
        {
            V[vit] = inum++;
            if (newPoints.empty())
                bbmin = bbmax = p;
            else
                for (unsigned int c=0; c<p.size(); c++)
                    if (p[c] < bbmin[c]) bbmin[c] = p[c]; else if (p[c] > bbmax[c]) bbmax[c] = p[c];
            newPoints.push_back(p);
        }
    }
    if (notconnected > 0) serr << notconnected << " points are not connected to the mesh."<<sendl;
    for( Cell_iterator cit = c3t3.cells_begin() ; cit != c3t3.cells_end() ; ++cit )
    {
        Tetra tetra;
        for (int i=0; i<4; i++)
            tetra[i] = V[cit->vertex(i)];
        tetrahedra.push_back(tetra);
    }

    return 0;
}


//visualisation of the shape before mesh
void MeshGenerationFromImplicitShape::draw(const sofa::core::visual::VisualParams* vparams) {

    if(m_componentstate == ComponentState::Invalid)
        return;

    if(drawTetras.getValue()) {
        sofa::helper::ReadAccessor< Data<VecCoord> > x = out_Points;
        sofa::helper::ReadAccessor< Data<SeqTetrahedra> > tetrahedra = out_tetrahedra;
        vparams->drawTool()->setLightingEnabled(false);
        std::vector<Coord> points[4];
        for(unsigned int i=0; i<tetrahedra.size(); ++i)
        {
            int a = tetrahedra[i][0];
            int b = tetrahedra[i][1];
            int c = tetrahedra[i][2];
            int d = tetrahedra[i][3];
            Coord center = (x[a]+x[b]+x[c]+x[d])*0.125;
            Coord pa = (x[a]+center)*(float)0.666667;
            Coord pb = (x[b]+center)*(float)0.666667;
            Coord pc = (x[c]+center)*(float)0.666667;
            Coord pd = (x[d]+center)*(float)0.666667;

            points[0].push_back(pa);
            points[0].push_back(pb);
            points[0].push_back(pc);

            points[1].push_back(pb);
            points[1].push_back(pc);
            points[1].push_back(pd);

            points[2].push_back(pc);
            points[2].push_back(pd);
            points[2].push_back(pa);

            points[3].push_back(pd);
            points[3].push_back(pa);
            points[3].push_back(pb);
        }

        vparams->drawTool()->drawTriangles(points[0], sofa::defaulttype::Vec<4,float>(0.0,0.0,1.0,1.0));
        vparams->drawTool()->drawTriangles(points[1], sofa::defaulttype::Vec<4,float>(0.0,0.5,1.0,1.0));
        vparams->drawTool()->drawTriangles(points[2], sofa::defaulttype::Vec<4,float>(0.0,1.0,1.0,1.0));
        vparams->drawTool()->drawTriangles(points[3], sofa::defaulttype::Vec<4,float>(0.5,1.0,1.0,1.0));
    }

}


SOFA_DECL_CLASS(MeshGenerationFromImplicitShape)
SOFA_LINK_CLASS(MeshGenerationFromImplicitShape)

} /// namespace _meshgenerationfromimplicit_
} /// namespace engine
} /// namespace component
} /// namespace sofa
