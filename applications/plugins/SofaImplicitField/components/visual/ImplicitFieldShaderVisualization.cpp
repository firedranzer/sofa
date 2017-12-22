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
/*****************************************************************************
 * Contributors:
 *      erwan.douaille@inria.fr
 *      damien.marchal@univ-lille1.fr
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <regex>
#include <stdlib.h>

#include "ImplicitFieldShaderVisualization.h"

#include <sofa/helper/gl/GLSLShader.h>
#include <sofa/helper/system/Locale.h>

#include <sofa/core/ObjectFactory.h>
using sofa::core::RegisterObject;

#include <sofa/helper/system/FileRepository.h>
using sofa::helper::system::DataRepository ;

namespace sofa
{

namespace component
{

namespace visual
{

namespace _implicitfieldshadervisualization_
{

ImplicitFieldShaderVisualization::ImplicitFieldShaderVisualization() :
    l_field(initLink("field", "The field to render.")),
    d_vertFilename(initData(&d_vertFilename, std::string("shaders/implicitFieldRenderer.vert"), "fileVertexShaders", "Set the vertex shader filename to load")),
    d_fragFilename(initData(&d_fragFilename, std::string("shaders/implicitFieldRenderer.frag"), "fileFragmentShaders", "Set the fragment shader filename to load")),
    m_templateShaderFileName("shaders/templates/implicitFieldRenderer"),
    m_templateFileName("shaders/implicitFieldRenderer")
{
    f_listening.setValue(true);
    d_vertFilename.setValue( m_templateFileName+"01.vert" );
    d_fragFilename.setValue( m_templateFileName+"01.frag" );
}

ImplicitFieldShaderVisualization::~ImplicitFieldShaderVisualization()
{
    shader->TurnOff();
    shader->Release();
    delete shader;
}

void ImplicitFieldShaderVisualization::init()
{
    if(l_field.get()->findData("state"))
    {
        m_datatracker.trackData(*l_field.get()->findData("state"));
        shaderGenerationCodeHasChanged();
    }

    shader = new GLSLShader();
    shader->SetVertexShaderFileName(d_vertFilename.getFullPath());
    shader->SetFragmentShaderFileName(d_fragFilename.getFullPath());
}

void ImplicitFieldShaderVisualization::reinit()
{
}

void ImplicitFieldShaderVisualization::initVisual()
{
    if (!GLSLShader::InitGLSL())
    {
        msg_error() << "InitGLSL failed." ;
        return;
    }
    shader->InitShaders();
}

void ImplicitFieldShaderVisualization::shaderGenerationCodeHasChanged()
{
    m_datatracker.clean() ;
    initComponentShaderValue();
    std::ofstream myfile;
    std::cout << "OUTPUT FILENAME: " << d_fragFilename.getFullPath() << std::endl ;
    myfile.open (d_fragFilename.getFullPath());
    generateFragmentShader(myfile);
    myfile.close();

    myfile.open (d_vertFilename.getFullPath());
    generateVertexShader(myfile);
    myfile.close();
}

void ImplicitFieldShaderVisualization::initComponentShaderValue()
{
    std::map<std::string, std::vector<GLSLCodeFragment>> glslMap = l_field->getGLSLCode();
    for(std::map<std::string, std::vector<GLSLCodeFragment>>::iterator it = glslMap.begin(); it != glslMap.end(); it++)
    {
        std::vector<GLSLCodeFragment> GLSLCodeFragments = it->second;
        for( std::vector<GLSLCodeFragment>::iterator itGLSLCode = GLSLCodeFragments.begin(); itGLSLCode != GLSLCodeFragments.end(); itGLSLCode++)
        {
            GLSLCodeFragment tmpGLSLCode = *itGLSLCode;
            std::string uniformName = tmpGLSLCode.m_name;
            std::string uniformValue = tmpGLSLCode.m_value;
            std::cout << "initComponentShaderValue: " << name << " " << tmpGLSLCode.m_type << std::endl ;

            BaseData* data = fetchData(uniformName, tmpGLSLCode.m_type);
            data->read(uniformValue);
            DataTracker* tmp = new DataTracker();
            tmp->trackData(*data);
            m_datatrackerList.push_back(tmp);
        }
    }
}

BaseData* ImplicitFieldShaderVisualization::fetchData(const std::string& argumentName, const std::string& type)
{
    MapData dataMap = getDataAliases();
    MapData::const_iterator itData = dataMap.find(argumentName);
    BaseData* data=nullptr;

    if (itData == dataMap.end())
    {
        std::cout << "DATA TYPE: " << type << std::endl ;
        if(type=="vec3")
           data = new sofa::core::objectmodel::Data<sofa::defaulttype::Vec3d>();
        else if(type=="float")
           data = new sofa::core::objectmodel::Data<float>();
        else
            data = new sofa::core::objectmodel::Data<std::string>();

        if (data == nullptr)
        {
            msg_warning() << " Something went wrong while creating data";
            return nullptr ;
        }
        data->setName(argumentName);
        data->setHelp("Auto generated help from ImplicitFieldShaderVisualization");
        addData(data, argumentName);
        msg_info(getName()) << " data field named : " << argumentName << " has been created";
    } else
        data = itData->second;
    return data;
}

void ImplicitFieldShaderVisualization::uploadUniformsValues()
{
    std::map<std::string, std::vector<GLSLCodeFragment>> glslMap = l_field->getGLSLCode();
    std::map<std::string, std::vector<GLSLCodeFragment>>::iterator itFind = glslMap.find("variable");
    if(itFind != glslMap.end())
    {
        std::vector<GLSLCodeFragment> uniforms = itFind->second;
        for( std::vector<GLSLCodeFragment>::iterator it = uniforms.begin(); it != uniforms.end(); it++)
        {
            GLSLCodeFragment tmpGLSLCode = *it;
            std::string uniformType = tmpGLSLCode.m_type;
            std::string uniformName = tmpGLSLCode.m_name;
            std::string uniformValue = tmpGLSLCode.m_value;

            BaseData* d = findData(tmpGLSLCode.m_name) ;
            const sofa::defaulttype::AbstractTypeInfo* nfo = d->getValueTypeInfo() ;

            if( nfo->Scalar() && nfo->Container() && nfo->size() == 3 )
            {
                const void* dv = d->getValueVoidPtr();
                shader->SetFloat3(shader->GetVariable(tmpGLSLCode.m_name),
                                  nfo->getScalarValue(dv,0),
                                  nfo->getScalarValue(dv,1),
                                  nfo->getScalarValue(dv,2));
            }
            else if( nfo->Scalar() && nfo->size() == 1 )
            {
                const void* dv = d->getValueVoidPtr();
                shader->SetFloat(shader->GetVariable(tmpGLSLCode.m_name),
                                  nfo->getScalarValue(dv,0));
            }
        }
    }
}

void ImplicitFieldShaderVisualization::drawVisual(const VisualParams* vp)
{
    if (m_datatracker.isDirty())
    {
        shaderGenerationCodeHasChanged();
        m_datatracker.clean();
    }

#ifdef SOFA_WITH_OPENGL
    shader->TurnOn();
    shader->SetFloat(shader->GetVariable("zNear"), vp->zNear());
    shader->SetFloat(shader->GetVariable("zFar"), vp->zFar());

    uploadUniformsValues();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBegin(GL_QUADS) ;
    glTexCoord2d(-1.0,-1.0);
    glVertex2d(  -1.0,-1.0) ;

    glTexCoord2d(-1.0,1.0);
    glVertex2d(  -1.0,1.0) ;

    glTexCoord2d(1.0, 1.0);
    glVertex2d(  1.0, 1.0) ;

    glTexCoord2d(1.0,-1.0);
    glVertex2d(  1.0,-1.0) ;
    glEnd();
    shader->TurnOff();
#endif /// SOFA_WITH_OPENGL
}

//////////////////////////////////////////////////////////////////////////
/// VERTEX SHADER GENERATION PART                                      ///
//////////////////////////////////////////////////////////////////////////

std::ostream& ImplicitFieldShaderVisualization::generateVertexShader(std::ostream& out)
{
    std::ostringstream istr ;
    std::ifstream input;
    std::string fullpath = DataRepository.getFile(m_templateShaderFileName+".vert");
    input.open(fullpath);
    if(!input.is_open()){
        msg_error() << "Unable to load the shader template file";
        return out;
    }

    istr << input.rdbuf() ;

    out << "//-------autogenerated ----------------" << std::endl;
    out << "// from: " << fullpath << std::endl ;
    out << istr.str() ;

    return out;
}

//////////////////////////////////////////////////////////////////////////
/// FRAGMENT SHADER GENERATION PART                                    ///
//////////////////////////////////////////////////////////////////////////

std::string ImplicitFieldShaderVisualization::uniformsAndConst()
{
    std::string tmp;

    std::map<std::string, std::vector<GLSLCodeFragment>> glslMap = l_field->getGLSLCode();
    std::map<std::string, std::vector<GLSLCodeFragment>>::iterator itFind = glslMap.find("variable");
    if(itFind != glslMap.end())
    {
        std::vector<GLSLCodeFragment> uniforms = itFind->second;
        for( std::vector<GLSLCodeFragment>::iterator it = uniforms.begin(); it != uniforms.end(); it++)
        {
            GLSLCodeFragment tmpGLSLCode = *it;
            tmp.append("uniform " + tmpGLSLCode.m_type + " " + tmpGLSLCode.m_dataname + ";\n");
        }
    }
    tmp.append("\n");

    return tmp;
}

std::string ImplicitFieldShaderVisualization::implicitFunction()
{    
    std::string implicitFunction;
    implicitFunction.append(
                "   res = minVec4(\n"
                "       vec4(sdPlane(pos), vec3(0.45, 0.45, 0.45)),\n"
                "       vec4(sdSphere(pos-vec3( -.0, 0.75, 0.0), .5), vec3(1.0, 1.0, 0.0))\n"
                "   );    \n"
                ); /// Default value if eval is empty

    ///CORRECT VERSION
    std::map<std::string, std::vector<GLSLCodeFragment>> glslMap = l_field->getGLSLCode();
    std::map<std::string, std::vector<GLSLCodeFragment>>::iterator itFind = glslMap.find("eval");

    if(itFind != glslMap.end())
    {
        implicitFunction.clear();
        std::vector<GLSLCodeFragment> evals = itFind->second;
        for( std::vector<GLSLCodeFragment>::iterator it = evals.begin(); it != evals.end(); it++)
        {
            GLSLCodeFragment shaderCode = *it;
            std::string name = shaderCode.m_name;
            std::string dataName = shaderCode.m_dataname;
            std::string value = shaderCode.m_value;
            std::cout << "IMPLICTI FUNCT: " << name << " " << shaderCode.m_type << std::endl ;

            if (changedFromDataField)
            {
                BaseData* data = fetchData(name, shaderCode.m_type);
                value = data->getValueString();
            }
            implicitFunction.append( value );
        }
    }
    return implicitFunction;
}

std::ostream& ImplicitFieldShaderVisualization::generateFragmentShader(std::ostream& out)
{
    std::ostringstream istr ;
    std::ifstream input;
    std::string fullpath = DataRepository.getFile(m_templateShaderFileName+".frag");
    input.open(fullpath);
    if(!input.is_open()){
        msg_error() << "Unable to load the shader template file";
        return out;
    }

    istr << input.rdbuf() ;

    std::string tmp;

    std::regex r1 ("REPLACE_WITH_UNIFORMS");   // matche UNIFORMS
    tmp = std::regex_replace (istr.str(), r1, uniformsAndConst());

    std::regex r2 ("REPLACE_WITH_SHAPE");
    tmp = std::regex_replace(tmp, r2, implicitFunction());

    out << "//-------autogenerated ----------------" << std::endl;
    out << "// from: " << fullpath << std::endl ;
    out << tmp ;


    return out ;
}

int ImplicitFieldShaderVisualizationClass = sofa::core::RegisterObject("Render an implicit field using a ray marching shader").add<ImplicitFieldShaderVisualization>();

} /// namespace _pointcloudimplicitfieldvisualization_
} /// namespace visual
} /// namespace component
} /// namespace sofa
