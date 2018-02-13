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
#ifndef SOFA_COMPONENT_ENGINE_MESHBARYCENTRICMAPPERENGINE_H
#define SOFA_COMPONENT_ENGINE_MESHBARYCENTRICMAPPERENGINE_H
#include "config.h"

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/defaulttype/Vec.h>
#include <sofa/core/DataEngine.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/defaulttype/Vec3Types.h>

namespace sofa
{

namespace component
{

namespace engine
{

/**
 * This class extrudes a surface
 */
template <class DataTypes>
class MeshBarycentricMapperEngine : public core::DataEngine
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(MeshBarycentricMapperEngine,DataTypes),core::DataEngine);
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::Real Real;
    typedef defaulttype::Vec<3,Real> Vec3;
    typedef typename sofa::helper::vector<unsigned int>  VecIndices;

protected:

    MeshBarycentricMapperEngine();

    ~MeshBarycentricMapperEngine() {}
public:
    void init() override;

    void reinit() override;

    void update() override;

    void draw(const core::visual::VisualParams* vparams) override;

    void addPointInLine(const int /*lineIndex*/, const SReal* /*baryCoords*/);

    void addPointInTriangle(const int /*triangleIndex*/, const SReal* /*baryCoords*/, const unsigned int /*pointIndex*/);

    void addPointInQuad(const int /*quadIndex*/, const SReal* /*baryCoords*/);

    void addPointInTetra(const int /*tetraIndex*/, const SReal* /*baryCoords*/, const unsigned int /*pointIndex*/);

    void addPointInCube(const int /*cubeIndex*/, const SReal* /*baryCoords*/);


    virtual std::string getTemplateName() const override
    {
        return templateName(this);
    }

    static std::string templateName(const MeshBarycentricMapperEngine<DataTypes>* = NULL)
    {
        return DataTypes::Name();
    }
    bool initialized;
    Data<std::string> InputMeshName;
    Data<VecCoord> InputPositions;
    Data<VecCoord> MappedPointPositions;
    Data<VecCoord> BarycentricPositions;
    Data< VecIndices> TableElements;
    Data<bool> computeLinearInterpolation;

    Data< sofa::helper::vector<sofa::helper::vector< unsigned int > > > f_interpolationIndices;
    Data< sofa::helper::vector<sofa::helper::vector< Real > > > f_interpolationValues;

private:

    void clear1d ( int reserve );
    void clear2d ( int reserve );
    void clear3d ( int reserve );

    sofa::core::topology::BaseMeshTopology* TopoInput ;

    VecCoord* baryPos ;
    VecIndices* tableElts;

    sofa::helper::vector<sofa::helper::vector< unsigned int > >* linearInterpolIndices;
    sofa::helper::vector<sofa::helper::vector< Real > >* linearInterpolValues;
};



#if defined(SOFA_EXTERN_TEMPLATE) && !defined(SOFA_COMPONENT_ENGINE_MESHBARYCENTRICMAPPERENGINE_CPP)
#ifndef SOFA_FLOAT
extern template class SOFA_GENERAL_ENGINE_API MeshBarycentricMapperEngine<defaulttype::Vec3dTypes>;
#endif //SOFA_FLOAT
#ifndef SOFA_DOUBLE
extern template class SOFA_GENERAL_ENGINE_API MeshBarycentricMapperEngine<defaulttype::Vec3fTypes>;
#endif //SOFA_DOUBLE
#endif

} // namespace engine

} // namespace component

} // namespace sofa

#endif
