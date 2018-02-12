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

#include <sofa/defaulttype/Vec.h>

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
namespace _meshgenerationfromimplicit_
{

using sofa::defaulttype::Vec3d ;
using sofa::component::geometry::ScalarField ;
typedef sofa::core::topology::BaseMeshTopology::SeqTetrahedra SeqTetrahedra;
typedef sofa::core::topology::BaseMeshTopology::Tetra Tetra;
typedef sofa::defaulttype::Vector3 Coord;
typedef sofa::helper::vector<Coord> VecCoord;

using namespace sofa::core;
using namespace sofa::core::visual;
using namespace sofa::core::objectmodel;

class SOFA_CGALPLUGIN_API MeshGenerationFromImplicitShape : public BaseObject,
        public ComponentTracker,
        public TrackedComponent
{
public:
    SOFA_CLASS(MeshGenerationFromImplicitShape, BaseObject);
    MeshGenerationFromImplicitShape() ;
    virtual ~MeshGenerationFromImplicitShape() { }
    int volumeMeshGeneration(double facet_angle, double facet_size, double facet_distance,
                             double cell_size, double cell_radius_edge_ratio);

    virtual void init() override ;
    virtual void reinit() override ;
    virtual void handleEvent(sofa::core::objectmodel::Event *event) override ;
    virtual void draw(const VisualParams* vparams) override ;
    virtual void computeBBox(const ExecParams *, bool) override ;

    virtual void update(bool forceUpdate=false) ;

private:
    CGAL::Bbox_3 BoundingBox(double x_min, double y_min, double z_min,
                             double x_max, double y_max, double z_max);

    /// Inputs and atritbutes
    Data<double> d_facetangle;
    Data<double> d_facetsize;
    Data<double> d_facetdistance;
    Data<double> d_cell_radiusedge_ratio;
    Data<double> d_cellsize;

    Data<double> d_radius ;
    Data<Vec3d>  d_center ;

    /// Display
    Data<bool> d_drawtetras;

    /// Output
    Data<VecCoord> d_out_points;
    Data<SeqTetrahedra> d_out_tetrahedra;

    /// Link
    typedef SingleLink< MeshGenerationFromImplicitShape, ScalarField, BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> LinkGrid;
    LinkGrid l_scalarfield;

    std::shared_future<unsigned int> m_com;
};

} /// namespace _meshgenerationfromimplicit_
    using _meshgenerationfromimplicit_::MeshGenerationFromImplicitShape ;
} /// namespace engine
} /// namespace component
} /// namespace sofa

#endif //SOFA_CGAL_MESHGENERATIONFROMIMPLICIT_H
