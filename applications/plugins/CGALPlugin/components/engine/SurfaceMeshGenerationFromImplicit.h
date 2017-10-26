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
#ifndef SOFA_CGAL_MESHGENERATIONFROMIMPLICIT_H
#define SOFA_CGAL_MESHGENERATIONFROMIMPLICIT_H

#include <CGALPlugin/config.h>
#include <future>

/// Topology
#include <SofaBaseTopology/MeshTopology.h>

/// Shapes
#include <SofaImplicitField/components/geometry/ScalarField.h>

namespace sofa
{
namespace component
{
namespace engine
{
namespace _surfacemeshgenerationfromimplicit_
{
using sofa::defaulttype::Vec4f ;
using sofa::component::geometry::ScalarField ;
typedef sofa::core::topology::BaseMeshTopology::SeqTriangles SeqTriangles;
typedef sofa::core::topology::BaseMeshTopology::Triangle Triangle;
typedef sofa::defaulttype::Vector3 Coord;
typedef sofa::defaulttype::Vector3 Vector3;
typedef sofa::helper::vector<Coord> VecCoord;

using namespace sofa::core;
using namespace sofa::core::visual;
using namespace sofa::core::objectmodel;

class SOFA_CGALPLUGIN_API SurfaceMeshGenerationFromImplicitShape : public BaseObject,
        public ComponentTracker,
        public TrackedComponent
{
public:
    SOFA_CLASS(SurfaceMeshGenerationFromImplicitShape, BaseObject);
    SurfaceMeshGenerationFromImplicitShape() ;
    virtual ~SurfaceMeshGenerationFromImplicitShape() { }

    virtual void init() override ;
    virtual void reinit() override ;
    virtual void handleEvent(sofa::core::objectmodel::Event *event) override ;
    virtual void draw(const VisualParams* vparams) override ;
    virtual void computeBBox(const ExecParams *, bool) override ;

    virtual void update(bool forceUpdate=false) ;
    void generateSurfaceMesh() ;

    Data<sofa::defaulttype::BoundingBox> d_box ;

private:
    CGAL::Bbox_3 BoundingBox(double x_min, double y_min, double z_min,
                             double x_max, double y_max, double z_max);

    /// Link
    typedef SingleLink< SurfaceMeshGenerationFromImplicitShape, ScalarField, BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> LinkGrid;
    LinkGrid l_in_scalarfield;

    /// Output
    Data<VecCoord>      d_out_points;
    Data<SeqTriangles>  d_out_triangles;

    Data<double> d_facetangle;
    Data<double> d_facetsize;
    Data<double> d_facetdistance;

    Data<double> d_radius ;
    Data<sofa::defaulttype::Vec3d>  d_center ;

    std::shared_future<unsigned int> m_com;
};

} /// namespace _meshgenerationfromimplicit_
    using _surfacemeshgenerationfromimplicit_::SurfaceMeshGenerationFromImplicitShape ;
} /// namespace engine
} /// namespace component
} /// namespace sofa

#endif //SOFA_CGAL_MESHGENERATIONFROMIMPLICIT_H
