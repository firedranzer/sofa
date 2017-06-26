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
#ifndef SCRIPTFORCEFIELD_H
#define SCRIPTFORCEFIELD_H

#include "ScriptFunction.h"
#include <sofa/core/behavior/ForceField.h>

namespace sofa
{

namespace core
{

namespace behavior
{

template <class DataTypes>
class SOFA_SOFAPYTHON_API ScriptForceField : public ForceField<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(ScriptForceField,DataTypes),SOFA_TEMPLATE(ForceField,DataTypes));

    typedef typename DataTypes::Real             Real;
    typedef typename DataTypes::Coord            Coord;
    typedef typename DataTypes::Deriv            Deriv;
    typedef typename DataTypes::VecCoord         VecCoord;
    typedef typename DataTypes::VecDeriv         VecDeriv;
    typedef core::objectmodel::Data<VecCoord>    DataVecCoord;
    typedef core::objectmodel::Data<VecDeriv>    DataVecDeriv;


protected:
    ScriptForceField();

public:
    /// @name control
    ///   Basic control (from BaseObject)
    /// @{

    /// Parse the given description to assign values to this object's fields and potentially other parameters
    virtual void parse ( sofa::core::objectmodel::BaseObjectDescription* arg );

    /// @}

    /// @name Vector operations
    /// @{

    virtual void addForce(const MechanicalParams* mparams, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v);
    // appelle en python : f += addForce(x,v);
    virtual void addDForce(const MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx );
    // appelle en python : df += addDForce(kFactor, bFactor, dx);
    virtual SReal getPotentialEnergy(const MechanicalParams* mparams, const DataVecCoord& x) const;
    // appelle en python : e = getPotentialEnergy(x);

    /// @}

    /// @name Matrix operations
    /// @{

    virtual void addKToMatrix(const MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix );
    // appelle en python : dMatrix = addKToMatrix(kFactor);
    // avec dMatrix= { {i,j,valueToAdd},{....}.....} la liste des valeurs à ajouter à matrix

    /// @}


protected:
    /// @name Script interface
    ///   Function that need to be implemented for each script language
    /// Typically, all "script_*" functions call the corresponding "*" function of the script, if it exists
    /// @{

    virtual void loadScript() = 0;      // load & bind functions

    virtual void script_addForce(const MechanicalParams* mparams, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v) = 0;
    virtual void script_addDForce(const MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx ) = 0;
    virtual SReal script_getPotentialEnergy(const MechanicalParams* mparams, const DataVecCoord& x) const = 0;
    virtual void script_addKToMatrix(const MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix ) = 0;

    /// @}

};


} // namespace behavior

} // namespace core

} // namespace sofa

#endif // SCRIPTFORCEFIELD_H
