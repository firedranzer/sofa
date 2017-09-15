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

#include <boost/algorithm/string.hpp>
#include <sofa/core/ObjectFactory.h>
#include <SofaPython/PythonEnvironment.h>

#include <sofa/core/ObjectFactory.h>
using sofa::core::RegisterObject ;

#include <sofa/helper/system/FileMonitor.h>
using sofa::helper::system::FileMonitor ;

#include <sofa/core/objectmodel/IdleEvent.h>
using sofa::core::objectmodel::IdleEvent ;

#include <cmath>
using std::max;
using sofa::core::objectmodel::ComponentState ;

#include "ScalarField.h"
using sofa::core::objectmodel::CStatus ;

#include "CustomField.h"
namespace sofa
{

namespace component
{

namespace geometry
{

namespace _customfield_
{

using sofa::simulation::PythonEnvironment ;


class MyFileListener : public sofa::helper::system::FileEventListener
{
        CustomField* m_cf ;
public:
        MyFileListener(CustomField* cf)
        {
            m_cf = cf ;
        }

        void fileHasChanged(const std::string& sourcefile) override
        {
            SOFA_UNUSED(sourcefile) ;
            m_cf->reinit() ;
        }
};

void CustomField::handleEvent(sofa::core::objectmodel::Event *event)
{
    if (dynamic_cast<sofa::core::objectmodel::IdleEvent *>(event))
        FileMonitor::updates() ;
    else
        BaseObject::handleEvent(event);
}

CustomField::CustomField() :
    d_function (initData(&d_function, (std::string)"", "function", "Use a python function to implement the implicit field.")),
    d_gradient (initData(&d_gradient, (std::string)"", "gradient", "Use a python function to implement the gradient field. If not provided returns gradient using finite difference.")),
    d_state (initData(&d_state, 0, "state", "This is a number indicating change in this component."))
{
    m_sourcefile = new MyFileListener(this) ;
}



void CustomField::getCythonHook(PyObject*& module) const
{
     if(module==nullptr)
    {
        PyErr_Print();
        return ;
    }

    PyObject* pDict = PyModule_GetDict(module);
    if(pDict==nullptr)
    {
        PyErr_Print();
        return ;
    }

    PyObject* fct = PyDict_GetItemString(pDict, "getEvalFunction");
    if (fct==nullptr)
    {
        PyErr_Print();
        return ;
    }

    if (!PyCallable_Check(fct))
    {
        msg_error() << "The object does not have getEvalFunction" ;
        return ;
    }
}

PyObject* CustomField::getPythonFunction(const std::string& attribname, const std::string& attribvalue, PyObject*& module) const
{
    /// Parsing.
    if(attribvalue.empty())
    {
       return nullptr ;
    }

    std::vector<std::string> fields ;
    boost::split( fields, attribvalue, boost::is_any_of( "." ) );

    if(fields.size() != 2)
    {
        msg_error() << "Cannot parse '"<< attribname <<"="<< attribvalue <<" '. Eg: 'modulename.functionname'" ;
        return nullptr ;
    }

    std::string modulename = fields[0] ;
    std::string functionname = fields[1] ;

    if(modulename.empty() || functionname.empty())
    {
        msg_error() << "Module's name and function's name cannot be empty." ;
        return nullptr ;
    }

    if(module==nullptr)
        module = PyImport_ImportModule(modulename.c_str()) ;
    else
        module = PyImport_ReloadModule(module) ;

    if(module==nullptr)
    {
        PyErr_Print();
        return nullptr;
    }

    PyObject* pDict = PyModule_GetDict(module);
    if(pDict==nullptr)
    {
        PyErr_Print();
        return nullptr;
    }

    PyObject* fct = PyDict_GetItemString(pDict, functionname.c_str());
    if (fct==nullptr)
    {
        PyErr_Print();
        return nullptr;
    }

    if (!PyCallable_Check(fct))
    {
        msg_error() << "The object '"<< functionname <<"' is not callable '"<< modulename <<"'" ;
        return nullptr;
    }

    modulename += ".py" ;
    if( FileMonitor::addFile(modulename, m_sourcefile) < 0 )
        msg_error() << "Unable to register file: " << modulename ;
    return fct ;
}

void CustomField::init()
{
    setStatus(CStatus::Busy) ;
    PythonEnvironment::gil lock(__func__) ;

    m_evalFunction = getPythonFunction("function", d_function.getValue(), m_functionModule) ;
    if(m_evalFunction==nullptr)
    {
        msg_error() << "Unable to find a required callable object from attribute 'function=\""<< d_function.getValue() <<"\"'" ;
        m_componentstate = ComponentState::Invalid ;
        setStatus(CStatus::Invalid) ;
        return ;
    }

    m_gradFunction = getPythonFunction("gradient", d_gradient.getValue(), m_gradientModule) ;
    if(m_gradFunction==nullptr)
    {
        msg_info() << "No gradient function found from attribute 'gradient=\"" << d_gradient.getValue() <<"\"'. Falling back to finite difference implementation" ;
    }    

    getCythonHook(m_functionModule) ;


    m_componentstate = ComponentState::Valid ;
    d_state.setValue(d_state.getValue()+1);

    dmsg_warning() << "UPDATED THE STATUS !! " ;
    setStatus(CStatus::Valid) ;
}


void CustomField::reinit()
{
    init() ;
}


double CustomField::getValue(Vec3d& pos, int& domain)
{
    PythonEnvironment::gil lock(__func__) ;

    SOFA_UNUSED(domain);
    assert(m_evalFunction!=nullptr) ;

    if(m_componentstate!=ComponentState::Valid)
        return std::nan("") ;

    PyObject* res = PyObject_CallFunction(m_evalFunction, "fff", pos.x(), pos.y(), pos.z());
    if(!res)
    {
        PyErr_Print() ;
        return std::nan("") ;
    }

    if(!PyFloat_Check(res))
    {
        msg_error() << "The evalFunction does not returns a Floating point value.";
        return std::nan("") ;
    }
    double tmp = PyFloat_AsDouble(res) ;
    Py_DecRef(res) ;
    return tmp;
}

Vec3d CustomField::getGradient(Vec3d& pos, int& domain)
{
    SOFA_UNUSED(domain);
    PythonEnvironment::gil lock(__func__) ;
    Vec3d tmp(std::nan(""),std::nan(""),std::nan("")) ;

    /// The component is not valid. We return nan.
    if(m_componentstate!=ComponentState::Valid)
        return tmp ;

    //// The component is valid but we have no gradient function. Thus we use the finite difference version.
    if(m_gradFunction==nullptr)
        return ScalarField::getGradient(pos, domain) ;

    PyObject* res = PyObject_CallFunction(m_gradFunction, "fff", pos.x(), pos.y(), pos.z());
    if(!res)
    {
        PyErr_Print() ;
        return tmp ;
    }

    if(!PyList_Check(res))
    {
        msg_error() << "The gradient function does not returns a list." ;
        return tmp ;
    }

    if(PyList_Size(res) != 3)
    {
        msg_error() << "The gradient function does not returns a list with three elements." ;
        return tmp ;
    }

    for(unsigned int i=0;i<3;i++){
        if(!PyFloat_Check( PyList_GetItem(res, i)))
        {
            msg_error() << "The gradient function returned a list with non floating point values at indice " << i ;
            return tmp ;
        }
    }

    tmp.set(PyFloat_AsDouble(PyList_GetItem(res,0)),
            PyFloat_AsDouble(PyList_GetItem(res,1)),
            PyFloat_AsDouble(PyList_GetItem(res,2))) ;

    Py_DecRef(res) ;

    return tmp ;
}

///factory register
int CustomFieldClass = RegisterObject("A custom scalar field. The scalar function is implemented with python.")
        .add< CustomField >() ;

SOFA_DECL_CLASS(CustomField)

} /// namespace _customfield_

} /// namespace geometry

} /// namespace component

} /// namespace sofa
