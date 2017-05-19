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
#include "PythonMacros.h"
#include "PythonEnvironment.h"
#include "ScriptForceField.h"

#include <sofa/core/objectmodel/GUIEvent.h>
#include <sofa/core/objectmodel/MouseEvent.h>
#include <sofa/core/objectmodel/KeypressedEvent.h>
#include <sofa/core/objectmodel/KeyreleasedEvent.h>


#include <sofa/core/objectmodel/IdleEvent.h>
using sofa::core::objectmodel::IdleEvent ;

using namespace sofa::simulation;
using namespace sofa::core::objectmodel;

namespace sofa
{

namespace core
{

namespace behavior
{

template <class DataTypes>
ScriptForceField<DataTypes>::ScriptForceField()
    : ForceField<DataTypes>()
{
}

/*
virtual void addForce(const MechanicalParams* mparams, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v);
virtual void addDForce(const MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx );
virtual SReal getPotentialEnergy(const MechanicalParams* mparams, const DataVecCoord& x);

virtual void addKToMatrix(const MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix );
*/

template <class DataTypes>
void ScriptForceField<DataTypes>::parse(sofa::core::objectmodel::BaseObjectDescription *arg)
{
    ForceField<DataTypes>::parse(arg);

    // load & bind script
    loadScript();
}

template <class DataTypes>
void ScriptForceField<DataTypes>::addForce(const MechanicalParams* mparams, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v)
{
    script_addForce(mparams,f,x,v);
}

template <class DataTypes>
void ScriptForceField<DataTypes>::addDForce(const MechanicalParams* mparams, DataVecDeriv& f, const DataVecDeriv& dx)
{
    script_addDForce(mparams,f,dx);
}

template <class DataTypes>
SReal ScriptForceField<DataTypes>::getPotentialEnergy(const MechanicalParams* mparams, const DataVecCoord& x)
{
    return script_getPotentialEnergy(mparams,x);
}

template <class DataTypes>
void ScriptForceField<DataTypes>::addKToMatrix(const MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix )
{
    script_addKToMatrix(mparams, matrix );
}



} // namespace behavior

} // namespace core

} // namespace sofa




