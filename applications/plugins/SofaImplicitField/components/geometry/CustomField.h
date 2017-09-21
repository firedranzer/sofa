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
#ifndef SOFAIMPLICITFIELD_COMPONENT_CUSTOMFIELD_H
#define SOFAIMPLICITFIELD_COMPONENT_CUSTOMFIELD_H
#include <SofaImplicitField/config.h>

#include <SofaImplicitField/components/geometry/ScalarField.h>

#include <SofaPython/PythonEnvironment.h>
#include <map>

///////////////////////////// FORWARD DEFINITIONS //////////////////////////////////////////////////
namespace sofa {
namespace helper {
namespace system {
    class FileEventListener ;
}
}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
namespace sofa
{

namespace component
{

namespace geometry
{

namespace _customfield_
{

using sofa::component::geometry::ScalarField ;
using sofa::defaulttype::Vec3d ;
using sofa::core::objectmodel::Data ;
using sofa::helper::system::FileEventListener ;




//////////////////////////// CLASS DEFINITION //////////////////////////////////////////////////////
typedef  double (*FieldFunction)(void*, double x, double y, double z) ;

class SOFA_SOFAIMPLICITFIELD_API CustomField : public ScalarField
{
public:
    SOFA_CLASS(CustomField, BaseObject);

public:
    virtual void init() override ;
    virtual void reinit() override ;
    virtual void handleEvent(sofa::core::objectmodel::Event *event) override ;

    using ScalarField::getValue ;
    using ScalarField::getGradient ;

    virtual double getValue(Vec3d& pos, int& domain) override ;
    virtual Vec3d getGradient(Vec3d& pos, int& i) override ;

    /// Returns a map with "id" and "glsl" textual code of the fragment.
    /// The idea is that we can tune what we take from python and how
    /// it is included in the glsl shader. Some convention have to be defined
    /// on what python should return.
    /// eg:
    ///    map["eval"] = "return min(p.x, p.y)"
    ///    map["eval"] = "return vec3(1.0,1.0,1.0)"
    /// when the map is empty means the component cannot makes a GLSL version of the
    /// distance function (eg no python function found)
    const std::map<std::string, std::string>& getGLSLCode() ;

    Data<std::string> d_function ;
    Data<std::string> d_glslFunction ;
    Data<std::string> d_gradient ;
    PyObject*         m_evalFunction ;
    PyObject*         m_gradFunction ;


protected:
    CustomField( ) ;
    virtual ~CustomField() { }

private:
    CustomField(const CustomField& n) ;
    CustomField& operator=(const CustomField& n) ;

    PyObject* getPythonFunction(const std::string& attribname,
                                const std::string& attribvalue,
                                PyObject*& module, bool doReload) const ;
    void getCythonHook(PyObject*& module) ;
    void updateGLSLCodeCacheFromPython() ;


    PyObject* m_functionModule {nullptr} ;
    PyObject* m_gradientModule {nullptr} ;
    PyObject* m_glslFunctionModule {nullptr} ;

    FieldFunction  m_rawFunction {nullptr} ;
    PyObject*      m_rawShape {nullptr} ;

    FileEventListener* m_sourcefile ;

    Data<int> d_state ;

    std::map<std::string, std::string> m_glslcodes ;
};

} /// namespace _scalarfield_

using _customfield_::CustomField ;

} /// namespace geometry

} /// namespace component

} /// namespace sofa

#endif

