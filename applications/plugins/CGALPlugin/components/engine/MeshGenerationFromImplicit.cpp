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
/******************************************************************************
 * Contributors:                                                              *
 *   - damien.marchal@univ-lille1.fr                                          *
 *                                                                            *
 *****************************************************************************/

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
#include <sofa/core/objectmodel/IdleEvent.h>

#include <fstream>
#include <future>
#include <thread>
#include <chrono>

namespace sofa
{
namespace component
{
namespace engine
{
namespace _meshgenerationfromimplicit_
{

using sofa::core::objectmodel::CStatus ;

using namespace CGAL::parameters;
using namespace sofa::core;
using namespace sofa::core::visual;
using namespace sofa::core::objectmodel;
using sofa::defaulttype::Vec3d ;

// Domain
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
using namespace CGAL::parameters;
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


// Sizing field
struct Spherical_sizing_field
{
  typedef Mesh_domain::Index Index;
  typedef K::FT FT ;
  K::FT operator()(const K::Point_3& p, const int, const Index&) const
  {
    return abs(cos(p.x()*10))+0.3;
  }
};


//factory register
int MeshGenerationFromImplicitShapeClass = RegisterObject("This component mesh a domain defined from a scalar field.").add< MeshGenerationFromImplicitShape >();

//create a bbox 3 for meshing in volume
CGAL::Bbox_3 MeshGenerationFromImplicitShape::BoundingBox(double x_min, double y_min, double z_min, double x_max, double y_max, double z_max) {

    CGAL::Bbox_3 bbox(x_min,y_min,z_min,x_max,y_max,z_max);
    return bbox;

}

MeshGenerationFromImplicitShape::MeshGenerationFromImplicitShape()
    : TrackedComponent(this)
    , d_facetangle(initData(&d_facetangle,(double)30.0,"facet_angle",
                                                          "This parameter controls the shape of surface facets. Actually, "
                                                          "it is a lower bound for the angle (in degree) of surface facets.(Default 30.0)"))
    , d_facetsize(initData(&d_facetsize,(double)1.0, "facet_size",
                                                       "This parameter controls the size of surface facets. Actually, each "
                                                       "surface facet has a surface Delaunay ball which is a ball circumscribing "
                                                       "the surface facet and centered on the surface patch. (Default 1.0)"))
    , d_facetdistance(initData(&d_facetdistance,(double)1.0,"facet_distance",
                                                                   "This parameter controls the approximation error of boundary and subdivision surfaces. "
                                                                   "Actually, it is either a constant or a spatially variable scalar field. It provides an "
                                                                   "upper bound for the distance between the circumcenter of a surface facet and the center "
                                                                   "of a surface Delaunay ball of this facet. (Default 1.0)"))
    , d_cellsize(initData(&d_cellsize, (double)1.0, "cell_size",
                                                    "This parameter controls the size of mesh tetrahedra. It is either a scalar or a spatially "
                                                    "variable scalar field. It provides an upper bound on the circumradii of the mesh tetrahedra. (Default 1.0)"))
    , d_cell_radiusedge_ratio(initData(&d_cell_radiusedge_ratio, (double)2.1, "cell_radius_edge_ratio",
                                                                  "This parameter controls the shape of mesh cells (but can't filter slivers, as we discussed earlier)."
                                                                  " Actually, it is an upper bound for the ratio between the circumradius of a mesh tetrahedron "
                                                                  "and its shortest edge. There is a theoretical bound for this parameter: the Delaunay refinement "
                                                                  "process is guaranteed to terminate for values of cell_radius_edge_ratio bigger than 2. (Default 2.1)"))
    , d_center(initData(&d_center, Vec3d(0,0,0), "meshingzone_center", "The center of the sphere where the meshing is done. (Default = 0,0,0)"))
    , d_radius(initData(&d_radius, (double)2.0,  "meshingzone_radius", "The radius of the sphere where the meshing is done. (Default = 2)"))
    , d_drawtetras(initData(&d_drawtetras,false,"drawTetras","display generated tetra mesh"))
    , d_out_points(initData(&d_out_points, "outputPoints", "position coordinates from the tetrahedral generation"))
    , d_out_tetrahedra(initData(&d_out_tetrahedra, "outputTetras", "list of tetrahedra"))
    , l_scalarfield(initLink("scalarfield", "The scalar field that defined the iso-function"))
{
    f_listening.setValue(true);
}

void MeshGenerationFromImplicitShape::init()
{
    if(l_scalarfield.empty())
    {
        m_componentstate = ComponentState::Invalid ;
        msg_error() << "Component non linked" ;
        return ;
    }

    /// Start tracking the component
    addComponent(l_scalarfield.get());

    if(l_scalarfield->getStatus() != CStatus::Valid) {
        m_componentstate = ComponentState::Invalid;
        msg_warning() << "Lazy Init()";
        return ;
    }

    /// std::cout << "LINK STATE " << in_scalarfield->findData("state") << std::endl ;
    /// volumeMeshGeneration(in_facetsize.getValue(),in_approximation.getValue(),in_cellsize.getValue());

    /// future from an async()
    m_com = std::async(std::launch::async, [this](){
        unsigned int countervalue = this->l_scalarfield->getCounter() ;
        this->volumeMeshGeneration(this->d_facetangle.getValue(),
                                   this->d_facetsize.getValue(),
                                   this->d_facetdistance.getValue(),
                                   this->d_cellsize.getValue(),
                                   this->d_cell_radiusedge_ratio.getValue());
        return countervalue; });
}

void MeshGenerationFromImplicitShape::reinit()
{
    update(true) ;
    computeBBox(ExecParams::defaultInstance(), false) ;
}

void MeshGenerationFromImplicitShape::computeBBox(const ExecParams *, bool)
{
    const Vec3d& c = d_center.getValue() ;
    const double r = d_radius.getValue() ;
    f_bbox.setValue(sofa::defaulttype::BoundingBox(c.x()-r, c.x()+r,
                                                   c.y()-r, c.y()+r,
                                                   c.z()-r, c.z()+r)) ;
}


void MeshGenerationFromImplicitShape::handleEvent(sofa::core::objectmodel::Event *event)
{
    if (dynamic_cast<sofa::core::objectmodel::IdleEvent *>(event))
        update() ;
    else
        BaseObject::handleEvent(event);


}

void MeshGenerationFromImplicitShape::update(bool forceUpdate)
{
    /// The inputs have changed
    if(  !m_com.valid() && hasChanged() || forceUpdate )
    {
         /// The inputs are not valid
        if(l_scalarfield->getStatus() != CStatus::Valid)
            return ;

        /// The inputs are valid & we should grab them.
        m_componentstate = ComponentState::Invalid ;
        m_com = std::async(std::launch::async, [this](){
            unsigned int countervalue = this->l_scalarfield->getCounter() ;
            this->volumeMeshGeneration(this->d_facetangle.getValue(),
                                       this->d_facetsize.getValue(),
                                       this->d_facetdistance.getValue(),
                                       this->d_cellsize.getValue(),
                                       this->d_cell_radiusedge_ratio.getValue());
            return countervalue; });
    }

    if( !m_com.valid() )
        return ;

    if( m_com.wait_for(std::chrono::microseconds::zero()) == std::future_status::ready )
    {
        m_componentstate = ComponentState::Valid ;
        updateCounterAt(m_com.get());
        dmsg_warning() << "Update component from source at " << m_com.get() ;
        m_com = std::future<unsigned int>() ;
    }
}


//mesh the implicit domain
int MeshGenerationFromImplicitShape::volumeMeshGeneration(double facet_angleP, double facet_sizeP, double facet_distanceP,
                                                          double cell_sizeP, double cell_radius_edge_ratioP)
{
    //Domain
    Mesh_domain *domain = NULL;
    if(l_scalarfield.empty())
        return 0;

    while(l_scalarfield->getComponentState() != ComponentState::Valid ) ;

    ImplicitFunction function(l_scalarfield);
    Function_vector v;
    v.push_back(function);
    double dtmp=d_radius.getValue();
    const Vec3d& ctmp = d_center.getValue() ;
    domain = new Mesh_domain(v, K::Sphere_3(K::Point_3(ctmp.x(), ctmp.y(), ctmp.z()), dtmp*dtmp), 1e-3);

    //Criteria
    Spherical_sizing_field fsize ;
    //Facet_criteria facet_criteria(30, facet_size2, approximation); // angle, size, approximation
    //Cell_criteria cell_criteria(2., cell_size=fsize); // radius-edge ratio, size
    ///Mesh_criteria criteria(facet_criteria, cell_criteria);
    Mesh_criteria criteria(facet_angle=facet_angleP, facet_size=facet_sizeP, facet_distance=facet_distanceP,
                           cell_radius_edge_ratio=cell_radius_edge_ratioP, cell_size=cell_sizeP);

    //Mesh generation
    C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(*domain, criteria, no_exude(), no_perturb());
    delete domain;

    //Topology
    sofa::helper::WriteAccessor< Data<VecCoord> > newPoints = d_out_points;
    sofa::helper::WriteAccessor< Data<SeqTetrahedra> > tetrahedra = d_out_tetrahedra;

    newPoints.clear();
    tetrahedra.clear();

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
void MeshGenerationFromImplicitShape::draw(const sofa::core::visual::VisualParams* vparams)
{
    auto& box = f_bbox.getValue();
    vparams->drawTool()->drawBoundingBox(box.minBBox(), box.maxBBox()) ;

    if(m_componentstate != ComponentState::Valid)
        return;

    if(d_drawtetras.getValue()) {
        sofa::helper::ReadAccessor< Data<VecCoord> > x = d_out_points;
        sofa::helper::ReadAccessor< Data<SeqTetrahedra> > tetrahedra = d_out_tetrahedra;
        vparams->drawTool()->setLightingEnabled(false);
        std::vector<Coord> points[4];
        for(unsigned int i=0; i<tetrahedra.size(); ++i)
        {
            int a = tetrahedra[i][0];
            int b = tetrahedra[i][1];
            int c = tetrahedra[i][2];
            int d = tetrahedra[i][3];
            Coord center = (x[a]+x[b]+x[c]+x[d])*0.125;
            Coord pa = (x[a]+center)*(double)0.666667;
            Coord pb = (x[b]+center)*(double)0.666667;
            Coord pc = (x[c]+center)*(double)0.666667;
            Coord pd = (x[d]+center)*(double)0.666667;

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

        vparams->drawTool()->drawTriangles(points[0], sofa::defaulttype::Vec<4,double>(0.0,0.0,1.0,1.0));
        vparams->drawTool()->drawTriangles(points[1], sofa::defaulttype::Vec<4,double>(0.0,0.5,1.0,1.0));
        vparams->drawTool()->drawTriangles(points[2], sofa::defaulttype::Vec<4,double>(0.0,1.0,1.0,1.0));
        vparams->drawTool()->drawTriangles(points[3], sofa::defaulttype::Vec<4,double>(0.5,1.0,1.0,1.0));
    }
}


SOFA_DECL_CLASS(MeshGenerationFromImplicitShape)
SOFA_LINK_CLASS(MeshGenerationFromImplicitShape)

} /// namespace _meshgenerationfromimplicit_
} /// namespace engine
} /// namespace component
} /// namespace sofa
