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
#include <SofaPython/PythonFactory.h>

#include <sofa/core/ObjectFactory.h>
using sofa::core::RegisterObject ;

#include <sofa/helper/system/FileMonitor.h>
using sofa::helper::system::FileMonitor ;

#include <sofa/core/objectmodel/IdleEvent.h>
using sofa::core::objectmodel::IdleEvent ;

#include <sofa/simulation/Node.h>
using sofa::simulation::Node ;

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
    d_glslFunction (initData(&d_glslFunction, (std::string)"", "glslFunction", "Use a python function to return glsl implicit field description.")),
    d_state (initData(&d_state, 0, "state", "This is a number indicating change in this component."))
{
    m_sourcefile = new MyFileListener(this) ;
}



void CustomField::getCythonHook(PyObject*& module)
{
    if(module==nullptr)
    {
        msg_error() << "Invalid module" ;
        return ;
    }

    PyObject* pDict = PyModule_GetDict(module);
    if(pDict==nullptr)
    {
        msg_error() << "Missing dict" ;
        return ;
    }

    PyObject* fct = PyDict_GetItemString(pDict, "getCythonFunction");
    if (fct==nullptr)
    {
        msg_warning() << "No getCythonFunction() found. Use slow path instead." ;
        return ;
    }

    if (!PyCallable_Check(fct))
    {
        msg_error() << "The object does not have a callable 'getCythonFunction'" ;
        return ;
    }
    PyObject* res = PyObject_CallFunction(fct, "");

    if( res != nullptr )
    {
        PyObject* f=PyTuple_GetItem(res, 0) ;
        PyObject* s=PyTuple_GetItem(res, 1) ;

        if( PyCapsule_CheckExact(f) ){
            m_rawFunction = (FieldFunction)PyCapsule_GetPointer(f, "evalFunction") ;
            m_rawShape = s ;
       }
    }
}

PyObject* CustomField::getPythonFunction(const std::string& attribname, const std::string& attribvalue, PyObject*& module, bool doReload) const
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

    if(module==nullptr){
        module = PyImport_ImportModule((char*)modulename.c_str()) ;
    }else if(doReload)
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

void CustomField::updateGLSLCodeCacheFromPython()
{
    PythonEnvironment::gil lock {__func__} ;

    msg_info() << "Search for glsl rendering map" ;
    PyObject* glslFunction = getPythonFunction("function", d_glslFunction.getValue(), m_glslFunctionModule, true) ;
    if(glslFunction==nullptr)
    {
        msg_error() << "Unable to find a required callable object from attribute 'function=\""<< d_glslFunction.getValue() <<"\"'" ;
        m_glslcodes.clear() ;
        return ;
    }
    std::cout << glslFunction << std::endl;
    std::cout << glslFunction->ob_type << std::endl;
    std::cout << glslFunction->ob_refcnt << std::endl;
    //TODO(dmarchal) add here the call to the python code and the conversion from the dict to the
    // right function.

    /// Nom, type, valeur.
    //m_glslcodes["VARIABLES"] = "pos0 = vec4(0,0,0,1); " ;

    ///
    //m_glslcodes["EVALFUNCTION"] = "return pos0*min(x,y); " ;
    //m_glslcodes["GRADFUNCTION"] = "return vec3(1.0,0.0,0.0)" ;
}

const std::map<std::string, std::vector<GLSLCodeFragment> > &CustomField::getGLSLCode()
{
//    updateGLSLCodeCacheFromPython();
    m_glslcodes.clear();
    std::vector<GLSLCodeFragment> evalList;
    std::vector<GLSLCodeFragment> variableList;

    GLSLCodeFragment eval1;
    eval1.m_dataname = "Sphere1";
    eval1.m_name = "Sphere1";
    eval1.m_type = "anEval";
    eval1.m_value = "min(sqrt(x*x+y*y+z*z) -1.0, sqrt((x+.5)*(x+0.5)+y*y+z*z) -1.0)";

    GLSLCodeFragment eval2;
    eval2.m_dataname = "Sphere2";
    eval2.m_name = "Sphere2";
    eval2.m_type = "anEval";
    eval2.m_value = "sqrt(x*x+y*y+z*z) -1.0";

    GLSLCodeFragment eval3;
    eval3.m_dataname = "Sphere3";
    eval3.m_name = "Sphere3";
    eval3.m_type = "anEval";
    eval3.m_value = "sqrt(x*x+y*y+z*z) -1.0";

    GLSLCodeFragment var1;
    var1.m_dataname = "evalColorSphere1";
    var1.m_name = "evalColorSphere1";
    var1.m_type = "vec3";
    var1.m_value = "1.0 0.0 0.0";

    GLSLCodeFragment var2;
    var2.m_dataname = "evalPositionSphere1";
    var2.m_name = "evalPositionSphere1";
    var2.m_type = "vec3";
    var2.m_value = "0.0 1.0 0.0";

    GLSLCodeFragment var3;
    var3.m_dataname = "evalColorSphere2";
    var3.m_name = "evalColorSphere2";
    var3.m_type = "vec3";
    var3.m_value = "0.0 1.0 0.0";

    GLSLCodeFragment var4;
    var4.m_dataname = "evalPositionSphere2";
    var4.m_name = "evalPositionSphere2";
    var4.m_type = "vec3";
    var4.m_value = "1.0 1.0 0.0";

    GLSLCodeFragment var5;
    var5.m_dataname = "evalColorSphere3";
    var5.m_name = "evalColorSphere3";
    var5.m_type = "vec3";
    var5.m_value = "0.0 0.0 1.0";

    GLSLCodeFragment var6;
    var6.m_dataname = "evalPositionSphere3";
    var6.m_name = "evalPositionSphere3";
    var6.m_type = "vec3";
    var6.m_value = "2.0 1.0 0.0";

    evalList.push_back(eval1);
    evalList.push_back(eval2);
    evalList.push_back(eval3);

    variableList.push_back(var1);
    variableList.push_back(var2);
    variableList.push_back(var3);
    variableList.push_back(var4);
    variableList.push_back(var5);
    variableList.push_back(var6);

    m_glslcodes["eval"] = evalList;
    m_glslcodes["variable"] = variableList;
    return m_glslcodes ;
}

void CustomField::init()
{
    setStatus(CStatus::Busy) ;
    PythonEnvironment::gil lock(__func__) ;

    m_evalFunction = getPythonFunction("function", d_function.getValue(), m_functionModule, true) ;
    if(m_evalFunction==nullptr)
    {
        msg_error() << "Unable to find a required callable object from attribute 'function=\""<< d_function.getValue() <<"\"'" ;
        m_componentstate = ComponentState::Invalid ;
        setStatus(CStatus::Invalid) ;
        return ;
    }

    m_gradFunction = getPythonFunction("gradient", d_gradient.getValue(), m_gradientModule, true) ;
    if(m_gradFunction==nullptr)
    {
        msg_info() << "No gradient function found from attribute 'gradient=\"" << d_gradient.getValue() <<"\"'. Falling back to finite difference implementation" ;
    }    

    //TODO(dmarchal) N'importe quoi ! Codé en dure pour testé et ça reste !!!!Au cachot.
    PyObject* loadShape = getPythonFunction("function", "customfield.loadShape", m_functionModule, false) ;
    if (PyCallable_Check(loadShape)){
        Node* me = (Node*)getContext() ;
        Node* root = (Node*)me->getRoot() ;

        PyObject* res = PyObject_CallFunction(loadShape, "O", PythonFactory::toPython(root));
        if(!res)
        {
            PyErr_Print() ;
            m_componentstate = ComponentState::Invalid ;
            setStatus(CStatus::Invalid) ;
            return ;
        }
    }else{
        dmsg_warning() << "No loadShape function found. use evalFunction." ;
    }

    msg_info() << "Search for getCythonHook" ;
    getCythonHook(m_functionModule) ;

    m_componentstate = ComponentState::Valid ;
    d_state.setValue(d_state.getValue()+1);
    setStatus(CStatus::Valid) ;
}


void CustomField::reinit()
{
    init() ;
}


double CustomField::getValue(Vec3d& pos, int& domain)
{
    SOFA_UNUSED(domain);
    assert(m_evalFunction!=nullptr) ;

    if(m_componentstate!=ComponentState::Valid)
        return std::nan("") ;

    if(m_rawFunction)
        return m_rawFunction(m_rawShape, pos.x(), pos.y(), pos.z()) ;

    PythonEnvironment::gil lock(__func__) ;

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
    Vec3d tmp(std::nan(""),std::nan(""),std::nan("")) ;

    /// The component is not valid. We return nan.
    if(m_componentstate!=ComponentState::Valid)
        return tmp ;

    //// The component is valid but we have no gradient function. Thus we use the finite difference version.
    if(m_gradFunction==nullptr)
        return ScalarField::getGradient(pos, domain) ;

    PythonEnvironment::gil lock(__func__) ;

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
