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
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/output_surface_facets_to_polyhedron.h>

#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/Complex_2_in_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>


#include <sofa/core/objectmodel/IdleEvent.h>

#include "SurfaceMeshGenerationFromImplicit.h"

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
namespace _surfacemeshgenerationfromimplicit_
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

// default triangulation for Surface_mesher
typedef CGAL::Surface_mesh_default_triangulation_3 SurfaceTriangulation;
// c2t3
//typedef CGAL::Simple_cartesian<double> Kernel ;
//typedef Kernel::Point_3 KPoint_3;

typedef CGAL::Complex_2_in_triangulation_3<SurfaceTriangulation> C2t3;
typedef SurfaceTriangulation::Geom_traits GT;
typedef GT::Sphere_3 Sphere_3;
typedef GT::Point_3  Point_3;
typedef GT::FT FT;
typedef FT (*Function)(GT::Point_3);
typedef CGAL::Polyhedron_3<GT> Polyhedron ;


FT sphere_function (Point_3 p) {
    const FT x2=p.x()*p.x(), y2=p.y()*p.y(), z2=p.z()*p.z();
    return x2+y2+z2-2;
}

class ImplicitFunction  //: public std::unary_function<GT::Point_3, GT::FT> {
{
private:
    ScalarField* m_shape;

public:
    //typedef result_type return_type;
    typedef GT::Point_3 Point;
    std::vector<Point_3> m_seeds;

    ImplicitFunction(ScalarField* shape, const std::vector<GT::Point_3> &seeds)
    {
        m_seeds=seeds;
        m_shape=shape;
    }

    GT::FT operator()(GT::Point_3 p) const
    {
        //return sphere_function(p) ;
        Vec3d tmp(p.x(),p.y(),p.z());
        double x = m_shape->getValue(tmp) ;
        return x;
    }


    struct Construct_initial_points
      {
        Construct_initial_points(const std::vector<Point_3>& seeds)
          : seeds(seeds)
        {}

        template<class OutputIterator>
        OutputIterator operator()(OutputIterator pts, const int n = 8) const
        {
          BOOST_FOREACH(Point_3 p, seeds){
            *pts++ = std::make_pair(p,0);
          }
          return pts;
        }

      private:
        const std::vector<Point_3>& seeds;
      };

      Construct_initial_points construct_initial_points_object() const
      {
        return Construct_initial_points(m_seeds);
    }
};

typedef CGAL::Implicit_surface_3<GT, ImplicitFunction> Surface_3;


//typedef CGAL::Implicit_multi_domain_to_labeling_function_wrapper<ImplicitFunction> Function_wrapper;
//typedef Function_wrapper::Function_vector Function_vector;
//typedef CGAL::Labeled_mesh_domain_3<Function_wrapper, K> Mesh_domain;



//factory register
int SurfaceMeshGenerationFromImplicitShapeClass = RegisterObject("This component mesh a domain defined from a scalar field.").add< SurfaceMeshGenerationFromImplicitShape >();

//create a bbox 3 for meshing in volume
CGAL::Bbox_3 SurfaceMeshGenerationFromImplicitShape::BoundingBox(double x_min, double y_min, double z_min, double x_max, double y_max, double z_max) {

    CGAL::Bbox_3 bbox(x_min,y_min,z_min,x_max,y_max,z_max);
    return bbox;
}

SurfaceMeshGenerationFromImplicitShape::SurfaceMeshGenerationFromImplicitShape()
    : TrackedComponent(this)
    , l_in_scalarfield(initLink("scalarfield", "The scalar field that defined the iso-function"))
    , d_out_points(initData(&d_out_points, "outputPoints", "position of the tiangles vertex"))
    , d_out_triangles(initData(&d_out_triangles, "outputTriangles", "list of triangles"))
    , d_box(initData(&d_box, sofa::defaulttype::BoundingBox(0,1,0,1,0,1), "box", "min - max coordinate of the grid where to sample the function. "))
    , d_facetangle(initData(&d_facetangle,(double)30.0,"facet_angle",
                            "This parameter controls the shape of surface facets. Actually, "
                            "it is a lower bound for the angle (in degree) of surface facets.(Default 30.0)"))
    , d_facetsize(initData(&d_facetsize,(double)0.1, "facet_size",
                           "This parameter controls the size of surface facets. Actually, each "
                           "surface facet has a surface Delaunay ball which is a ball circumscribing "
                           "the surface facet and centered on the surface patch. (Default 1.0)"))
    , d_facetdistance(initData(&d_facetdistance,(double)0.1,"facet_distance",
                               "This parameter controls the approximation error of boundary and subdivision surfaces. "
                               "Actually, it is either a constant or a spatially variable scalar field. It provides an "
                               "upper bound for the distance between the circumcenter of a surface facet and the center "
                               "of a surface Delaunay ball of this facet. (Default 1.0)"))
    , d_center(initData(&d_center, Vec3d(0,0,0), "meshingzone_center", "The center of the sphere where the meshing is done. (Default = 0,0,0)"))
    , d_radius(initData(&d_radius, (double)2.0,  "meshingzone_radius", "The radius of the sphere where the meshing is done. (Default = 2)"))
    , d_visuCavity(initData(&d_visuCavity,true, "visuCavity","Allows to display the cavity surface"))
{
    f_listening.setValue(true);
}

void SurfaceMeshGenerationFromImplicitShape::init()
{
    if(l_in_scalarfield.empty())
    {
        m_componentstate = ComponentState::Invalid ;
        msg_error() << "Component non linked" ;
        return ;
    }

    /// Start tracking the component
    addComponent(l_in_scalarfield.get());

    if(l_in_scalarfield->getStatus() != CStatus::Valid) {
        m_componentstate = ComponentState::Invalid;
        msg_warning() << "Lazy Init()";
        return ;
    }

    /// future from an async()
    m_com = std::async(std::launch::async, [this](){
        unsigned int countervalue = this->l_in_scalarfield->getCounter() ;
        this->generateSurfaceMesh();
        return countervalue;
    });
}

void SurfaceMeshGenerationFromImplicitShape::reinit()
{
    update(true) ;
    f_bbox = d_box.getValue() ;
}

void SurfaceMeshGenerationFromImplicitShape::computeBBox(const ExecParams *, bool)
{
    f_bbox = d_box.getValue() ;
}

void SurfaceMeshGenerationFromImplicitShape::handleEvent(sofa::core::objectmodel::Event *event)
{
    if (dynamic_cast<sofa::core::objectmodel::IdleEvent *>(event))
        update() ;
    else
        BaseObject::handleEvent(event);
}

void SurfaceMeshGenerationFromImplicitShape::generateSurfaceMesh()
{
    if(l_in_scalarfield.empty())
        return ;

    while(l_in_scalarfield->getComponentState() != ComponentState::Valid ) ;


    SurfaceTriangulation tr;            // 3D-Delaunay triangulation
    C2t3 c2t3 (tr);                     // 2D-complex in 3D-Delaunay triangulation

    std::cout << "GENERATE SURFACE MESH FROM IMPLICIT FIELD" << std::endl ;

    /// defining the surface
    ImplicitFunction function(l_in_scalarfield, {
                                  {0,0,0},
                                  {0.5,0.0,0.0},
                                  {-0.5,0.0,0.0},
                                  {0.5,0.0,-0.5},
                                  {0.0,0.0,0.5}
                              });
    Vec3d ctmp=d_center.getValue();
    Surface_3 surface(function,             // pointer to function
                      Sphere_3(GT::Point_3(ctmp.x(), ctmp.y(), ctmp.z()), d_radius.getValue())); // bounding sphere

    /// Note that "2." above is the *squared* radius of the bounding sphere!
    /// defining meshing criteria
    CGAL::Surface_mesh_default_criteria_3<SurfaceTriangulation> criteria(d_facetangle.getValue(),     // angular bound
                                                                         d_facetsize.getValue(),      // radius bound
                                                                         d_facetdistance.getValue()); // distance bound

    /// meshing surface
    CGAL::make_surface_mesh(c2t3, surface, criteria, CGAL::Manifold_tag()) ;

    /// Convert that into a polyhedron
    Polyhedron polyhedron;
    CGAL::output_surface_facets_to_polyhedron(c2t3, polyhedron);

    /// Copy the surface as a Sofa topology
    sofa::helper::WriteOnlyAccessor< Data<VecCoord> >      newPoints = d_out_points;
    sofa::helper::WriteOnlyAccessor< Data<SeqTriangles> >  newTriangles = d_out_triangles;

    newPoints.clear();
    newTriangles.clear();
    std::map<Polyhedron::Vertex_handle, int> handler2index ;
    Coord bbmin, bbmax;
    for( Polyhedron::Vertex_iterator vit = polyhedron.vertices_begin();
         vit != polyhedron.vertices_end(); ++vit)
    {
        Point_3 pointCgal = vit->point();
        Coord p;
        p[0] = CGAL::to_double(pointCgal.x());
        p[1] = CGAL::to_double(pointCgal.y());
        p[2] = CGAL::to_double(pointCgal.z());
        handler2index[vit] = newPoints.size();
        newPoints.push_back(p);

        /// Let's recompute the bounding box.
        if (newPoints.empty())
            bbmin = bbmax = p;
        else
            for (unsigned int c=0; c<p.size(); c++)
                if (p[c] < bbmin[c]) bbmin[c] = p[c];
                else
                    if (p[c] > bbmax[c]) bbmax[c] = p[c];
    }

    for( Polyhedron::Facet_iterator faceIt = polyhedron.facets_begin() ;
                                    faceIt != polyhedron.facets_end() ; ++ faceIt)
    {
        Triangle triangle;

        int i=0 ;
        Polyhedron::Halfedge_around_facet_circulator j=faceIt->facet_begin();
        do
        {
            triangle[i] = handler2index[j->vertex()];
            ++i ;
            ++j ;
        }while(j != faceIt->facet_begin() && i < 3) ;

        newTriangles.push_back(triangle);
    }
}

void SurfaceMeshGenerationFromImplicitShape::update(bool forceUpdate)
{
    /// The inputs have changed
    if(  !m_com.valid() && hasChanged() || forceUpdate )
    {
        /// The inputs are not valid
        if(l_in_scalarfield->getStatus() != CStatus::Valid)
            return ;

        /// The inputs are valid & we should grab them.
        m_componentstate = ComponentState::Invalid ;

        m_com = std::async(std::launch::async, [this](){
            unsigned int countervalue = this->l_in_scalarfield->getCounter() ;
            this->generateSurfaceMesh();
            return countervalue;
        });

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

void SurfaceMeshGenerationFromImplicitShape::draw(const sofa::core::visual::VisualParams* vparams)
{
    if(m_componentstate != ComponentState::Valid)
    {
        update();
        return;
    }

    if(!d_visuCavity.getValue())
    {
        return;
    }

    auto& box = d_box.getValue();
    vparams->drawTool()->drawBoundingBox(box.minBBox(), box.maxBBox()) ;

    sofa::helper::ReadAccessor< Data<VecCoord> > x = d_out_points;
    sofa::helper::ReadAccessor< Data<SeqTriangles> > triangles = d_out_triangles;
    vparams->drawTool()->setLightingEnabled(true);

    for(const Triangle& triangle : triangles)
    {
        int a = triangle[0];
        int b = triangle[1];
        int c = triangle[2];
        Coord center = (x[a]+x[b]+x[c])*0.333333;
        Coord pa = (0.9*x[a]+0.1*center) ;
        Coord pb = (0.9*x[b]+0.1*center) ;
        Coord pc = (0.9*x[c]+0.1*center) ;

        Vector3 a1 = x[c]-x[b] ;
        Vector3 a2 = x[a]-x[b] ;

        vparams->drawTool()->drawTriangles({pa,pb,pc},
                                           a1.cross(a2),
                                           sofa::defaulttype::Vec<4,double>(0.0,0.0,1.0,1.0));
    }

    vparams->drawTool()->drawSpheres(d_out_points.getValue(), 0.01, Vec4f(1.0,1.0,0.0,0.2)) ;
}

} /// namespace _meshgenerationfromimplicit_
} /// namespace engine
} /// namespace component
} /// namespace sofa
