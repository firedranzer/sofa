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
int SurfaceMeshGenerationFromImplicitShapeClass = RegisterObject("This component mesh a domain defined from a scalar field.").add< SurfaceMeshGenerationFromImplicitShape >();

//create a bbox 3 for meshing in volume
CGAL::Bbox_3 SurfaceMeshGenerationFromImplicitShape::BoundingBox(double x_min, double y_min, double z_min, double x_max, double y_max, double z_max) {

    CGAL::Bbox_3 bbox(x_min,y_min,z_min,x_max,y_max,z_max);
    return bbox;

}

SurfaceMeshGenerationFromImplicitShape::SurfaceMeshGenerationFromImplicitShape()
    : TrackedComponent(this)
    , in_scalarfield(initLink("scalarfield", "The scalar field that defined the iso-function"))
    , d_box(initData(&d_box, sofa::defaulttype::BoundingBox(0,1,0,1,0,1), "box", "min - max coordinate of the grid where to sample the function. "))
{
    f_listening.setValue(true);
}

void SurfaceMeshGenerationFromImplicitShape::init()
{
    if(in_scalarfield.empty())
    {
        m_componentstate = ComponentState::Invalid ;
        msg_error() << "Component non linked" ;
        return ;
    }

    /// Start tracking the component
    addComponent(in_scalarfield.get());

    if(in_scalarfield->getStatus() != CStatus::Valid) {
        m_componentstate = ComponentState::Invalid;
        msg_warning() << "Lazy Init()";
        return ;
    }

    /// std::cout << "LINK STATE " << in_scalarfield->findData("state") << std::endl ;
    /// volumeMeshGeneration(in_facetsize.getValue(),in_approximation.getValue(),in_cellsize.getValue());

    /// future from an async()
    /*
    m_com = std::async(std::launch::async, [this](){
        unsigned int countervalue = this->in_scalarfield->getCounter() ;
        this->volumeMeshGeneration(this->in_facetangle.getValue(),
                                   this->in_facetsize.getValue(),
                                   this->in_facetdistance.getValue(),
                                   this->in_cellsize.getValue(),
                                   this->in_cell_radiusedge_ratio.getValue());
        return countervalue; });
    */
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

void SurfaceMeshGenerationFromImplicitShape::update(bool forceUpdate)
{
    /// The inputs have changed
    if(  !m_com.valid() && hasChanged() || forceUpdate )
    {
         /// The inputs are not valid
        if(in_scalarfield->getStatus() != CStatus::Valid)
            return ;

        /// The inputs are valid & we should grab them.
        m_componentstate = ComponentState::Invalid ;
        /*
        m_com = std::async(std::launch::async, [this](){
            unsigned int countervalue = this->in_scalarfield->getCounter() ;
            this->volumeMeshGeneration(this->in_facetangle.getValue(),
                                       this->in_facetsize.getValue(),
                                       this->in_facetdistance.getValue(),
                                       this->in_cellsize.getValue(),
                                       this->in_cell_radiusedge_ratio.getValue());
            return countervalue; });
            */
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


//visualisation of the shape before mesh
void SurfaceMeshGenerationFromImplicitShape::draw(const sofa::core::visual::VisualParams* vparams)
{
    if(m_componentstate != ComponentState::Valid)
        return;

    auto& box = d_box.getValue();
    vparams->drawTool()->drawBoundingBox(box.minBBox(), box.maxBBox()) ;
}


SOFA_DECL_CLASS(SurfaceMeshGenerationFromImplicitShape)
SOFA_LINK_CLASS(SurfaceMeshGenerationFromImplicitShape)

} /// namespace _meshgenerationfromimplicit_
} /// namespace engine
} /// namespace component
} /// namespace sofa
