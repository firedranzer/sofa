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

#include "PythonEnvironment.h"
#include "PythonScriptForceField.inl"
#include <sofa/core/objectmodel/DataFileName.h>
#include <sofa/core/ObjectFactory.h>


namespace sofa
{

namespace core
{

namespace behavior
{

int PythonScriptForceFieldClass = sofa::core::RegisterObject("A Sofa forcefield scripted in python")
#ifdef SOFA_WITH_DOUBLE
        .add< PythonScriptForceField<Vec1dTypes> >()
//        .add< PythonScriptForceField<Vec2dTypes> >()
//        .add< PythonScriptForceField<Vec3dTypes> >()
//        .add< PythonScriptForceField<Vec6dTypes> >()
//        .add< PythonScriptForceField<Rigid3dTypes> >()
//        .add< PythonScriptForceField<Rigid2dTypes> >()
#endif
#ifdef SOFA_WITH_FLOAT
//        .add< PythonScriptForceField<Vec1fTypes> >()
//        .add< PythonScriptForceField<Vec2fTypes> >()
//        .add< PythonScriptForceField<Vec3fTypes> >()
//        .add< PythonScriptForceField<Vec6fTypes> >()
//        .add< PythonScriptForceField<Rigid3fTypes> >()
//        .add< PythonScriptForceField<Rigid2fTypes> >()
#endif
        ;

SOFA_DECL_CLASS(PythonScriptForceField)


using namespace sofa::defaulttype;
#ifdef SOFA_WITH_DOUBLE
template class SOFA_SOFAPYTHON_API PythonScriptForceField<Vec1dTypes>;
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Vec2dTypes>;
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Vec3dTypes>;
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Vec6dTypes>;
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Rigid3dTypes>;
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Rigid2dTypes>;
#endif
#ifdef SOFA_WITH_FLOAT
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Vec3fTypes>;
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Vec2fTypes>;
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Vec1fTypes>;
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Vec6fTypes>;
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Rigid3fTypes>;
//template class SOFA_SOFAPYTHON_API PythonScriptForceField<Rigid2fTypes>;
#endif


} // namespace behavior

} // namespace core

} // namespace sofa

