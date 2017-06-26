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
#ifndef PYTHONFORCEFIELD_INL
#define PYTHONFORCEFIELD_INL

#include "PythonEnvironment.h"
#include "PythonScriptForceField.h"
//#include <sofa/core/objectmodel/DataFileName.h>


namespace sofa
{

namespace core
{

namespace behavior
{

template <class DataTypes>
PythonScriptForceField<DataTypes>::PythonScriptForceField()
{

}

template <class DataTypes>
PythonScriptForceField<DataTypes>::~PythonScriptForceField()
{

}



template <class DataTypes>
void PythonScriptForceField<DataTypes>::loadScript()
{

}

template <class DataTypes>
void PythonScriptForceField<DataTypes>::script_addForce(const MechanicalParams* mparams, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v)
// appel en python : f += addForce(x,v);
{

}

template <class DataTypes>
void PythonScriptForceField<DataTypes>::script_addDForce(const MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx )
// appel en python : df += addDForce(kFactor, bFactor, dx);
{

}

template <class DataTypes>
SReal PythonScriptForceField<DataTypes>::script_getPotentialEnergy(const MechanicalParams* mparams, const DataVecCoord& x) const
// appel en python : e = getPotentialEnergy(x);
{
    return 0.f;
}

template <class DataTypes>
void PythonScriptForceField<DataTypes>::script_addKToMatrix(const MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix )
// appel en python : dMatrix = addKToMatrix(kFactor);
// avec dMatrix= { {i,j,valueToAdd},{....}.....} la liste des valeurs à ajouter à matrix
{

}


} // namespace behavior

} // namespace core

} // namespace sofa

#endif // PYTHONFORCEFIELD_INL
