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
#ifndef SOFA_CGAL_MESHGENERATIONFROMIMPLICIT_H
#define SOFA_CGAL_MESHGENERATIONFROMIMPLICIT_H

#include <CGALPlugin/config.h>


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

using sofa::component::geometry::ScalarField ;
typedef sofa::core::topology::BaseMeshTopology::SeqTetrahedra SeqTetrahedra;
typedef sofa::core::topology::BaseMeshTopology::Tetra Tetra;
typedef sofa::defaulttype::Vector3 Coord;
typedef sofa::helper::vector<Coord> VecCoord;

using namespace sofa::core;
using namespace sofa::core::visual;
using namespace sofa::core::objectmodel;

class SOFA_CGALPLUGIN_API MeshGenerationFromImplicitShape : public BaseObject
{
public:
    SOFA_CLASS(MeshGenerationFromImplicitShape, BaseObject);
    void draw(const VisualParams* vparams);
    MeshGenerationFromImplicitShape() ;
    virtual ~MeshGenerationFromImplicitShape() { }
    int volumeMeshGeneration(float facet_size, float approximation, float cell_size);
    virtual void init();

private:
    CGAL::Bbox_3 BoundingBox(double x_min, double y_min, double z_min,
                             double x_max, double y_max, double z_max);

    /// Inputs and atritbutes
    Data<float> in_facetsize;
    Data<float> in_approximation;
    Data<float> in_cellsize;
    Data<double> xmin_box, ymin_box, zmin_box, xmax_box, ymax_box, zmax_box;

    /// Display
    Data<bool> drawTetras;

    /// Output
    Data<VecCoord> out_Points;
    Data<SeqTetrahedra> out_tetrahedra;

    /// Link
    typedef SingleLink< MeshGenerationFromImplicitShape, ScalarField, BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> LinkGrid;
    LinkGrid in_scalarfield;
};

} /// namespace _meshgenerationfromimplicit_
    using _meshgenerationfromimplicit_::MeshGenerationFromImplicitShape ;
} /// namespace engine
} /// namespace component
} /// namespace sofa

#endif //SOFA_CGAL_MESHGENERATIONFROMIMPLICIT_H
