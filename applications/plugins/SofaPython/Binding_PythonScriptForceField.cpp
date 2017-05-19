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

#include "Binding_PythonScriptForceField.h"
#include "Binding_Base.h"

using namespace sofa::core::behavior;

#include <sofa/simulation/Node.h>
using namespace sofa::simulation;
using namespace sofa::core::objectmodel;

#include <sofa/helper/logging/Messaging.h>






// These functions are empty ones;
// they are meant to be overriden by real python controller scripts


#define LOG_UNIMPLEMENTED_METHODS   // prints a message each time a non-implemented (in the script) method is called

extern "C" PyObject * PythonScriptForceField1d_addForce(PyObject * self, PyObject * args)
{
    SOFA_UNUSED(args) ;
#ifdef LOG_UNIMPLEMENTED_METHODS
    PythonScriptForceField1d* obj=dynamic_cast<PythonScriptForceField1d*>(((PySPtr<Base>*)self)->object.get());
     msg_error("PythonScriptForceField1") << obj->m_classname.getValueString() << ".addForce not implemented in " << obj->name.getValueString();
#endif
    Py_RETURN_NONE;
}

extern "C" PyObject * PythonScriptForceField1d_addDForce(PyObject * self, PyObject * args)
{
    SOFA_UNUSED(args) ;
#ifdef LOG_UNIMPLEMENTED_METHODS
    PythonScriptForceField1d* obj=dynamic_cast<PythonScriptForceField1d*>(((PySPtr<Base>*)self)->object.get());
     msg_error("PythonScriptForceField1d") << obj->m_classname.getValueString() << ".addDForce not implemented in " << obj->name.getValueString();
#endif
    Py_RETURN_NONE;
}

extern "C" PyObject * PythonScriptForceField1d_getPotentialEnergy(PyObject * self, PyObject * args)
{
    SOFA_UNUSED(args) ;
#ifdef LOG_UNIMPLEMENTED_METHODS
    PythonScriptForceField1d* obj=dynamic_cast<PythonScriptForceField1d*>(((PySPtr<Base>*)self)->object.get());
     msg_error("PythonScriptForceField1d") << obj->m_classname.getValueString() << ".getPotentialEnergy not implemented in " << obj->name.getValueString();
#endif
    Py_RETURN_NONE;
}

extern "C" PyObject * PythonScriptForceField1d_addKToMatrix(PyObject * self, PyObject * args)
{
    SOFA_UNUSED(args) ;
#ifdef LOG_UNIMPLEMENTED_METHODS
    PythonScriptForceField1d* obj=dynamic_cast<PythonScriptForceField1d*>(((PySPtr<Base>*)self)->object.get());
     msg_error("PythonScriptForceField1d") << obj->m_classname.getValueString() << ".addKToMatrix not implemented in " << obj->name.getValueString();
#endif
    Py_RETURN_NONE;
}



SP_CLASS_METHODS_BEGIN(PythonScriptForceField1d)
SP_CLASS_METHOD(PythonScriptForceField1d,addForce)
SP_CLASS_METHOD(PythonScriptForceField1d,addDForce)
SP_CLASS_METHOD(PythonScriptForceField1d,getPotentialEnergy)
SP_CLASS_METHOD(PythonScriptForceField1d,addKToMatrix)
SP_CLASS_METHODS_END


SP_CLASS_TYPE_SPTR(PythonScriptForceField1d,PythonScriptForceField1d,Base)
