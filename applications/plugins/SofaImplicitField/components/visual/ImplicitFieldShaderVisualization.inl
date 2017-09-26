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
#include "ImplicitFieldShaderVisualization.h"

#include <sofa/helper/ArgumentParser.h>
#include <SofaSimulationCommon/common.h>
#include <sofa/simulation/Node.h>
#include <sofa/helper/system/PluginManager.h>
#include <sofa/simulation/config.h> // #defines SOFA_HAVE_DAG (or not)
#include <SofaSimulationCommon/init.h>
#include <SofaSimulationTree/init.h>
#include <SofaSimulationTree/TreeSimulation.h>

using sofa::simulation::Simulation;

namespace sofa
{

namespace component
{

namespace visual
{

namespace _pointcloudimplicitfieldvisualization_
{

ImplicitFieldShaderVisualization::ImplicitFieldShaderVisualization() :
    l_field(initLink("shape", "The shape to render.")),
    d_vertFilename(initData(&d_vertFilename, std::string("shaders/implicitShape.vert"), "fileVertexShaders", "Set the vertex shader filename to load")),
    d_fragFilename(initData(&d_fragFilename, std::string("shaders/implicitShape.frag"), "fileFragmentShaders", "Set the fragment shader filename to load"))
{
    f_listening.setValue(true);
    mouseX = 0;
    mouseY = 0;
}

ImplicitFieldShaderVisualization::~ImplicitFieldShaderVisualization()
{
    shader->TurnOff();
    shader->Release();
    delete shader;
}

void ImplicitFieldShaderVisualization::init()
{
    shader = new sofa::helper::gl::GLSLShader();

    shader->SetVertexShaderFileName(d_vertFilename.getFullPath());
    shader->SetFragmentShaderFileName(d_fragFilename.getFullPath());
    m_datatracker.trackData(*l_field.get()->findData("state"));
}

void ImplicitFieldShaderVisualization::reinit()
{
}

void ImplicitFieldShaderVisualization::initVisual()
{
    if (!sofa::helper::gl::GLSLShader::InitGLSL())
    {
        serr << "InitGLSL failed" << sendl;
        return;
    }
    shader->InitShaders();
}

void ImplicitFieldShaderVisualization::stop()
{
    if(shader->IsReady())
    {
        glDisable(GL_VERTEX_PROGRAM_TWO_SIDE);
        glClampColorARB(GL_CLAMP_VERTEX_COLOR, GL_TRUE);
        shader->TurnOff();
    }
}

void ImplicitFieldShaderVisualization::start()
{

    if(m_datatracker.isDirty())
    {
        std::ofstream myfile;
        myfile.open (d_fragFilename.getFullPath());
        myfile << generateFragmentShaderFrom();
        myfile.close();
        m_datatracker.clean();
    }

    if(shader->IsReady())
    {
        shader->TurnOn();

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float pixelSize[2];
        pixelSize[0] = viewport[2];
        pixelSize[1] = viewport[3];

        shader->SetFloat2(shader->GetVariable("resolution"), pixelSize[0], pixelSize[1]);
        shader->SetFloat2(shader->GetVariable("mouse"), mouseX, mouseY);
        shader->SetFloat(shader->GetVariable("wheelDelta"), wheelDelta);


        Simulation* simu = sofa::simulation::getSimulation();
        simu->findData("");

        /// TODO se mettre d'accord sur le contenu des maps :/

        //        std::map<std::string, std::string> glslMap = l_field->getGLSLCode();
        //        std::map<std::string, std::string>::iterator itFind = glslMap.find("uniforms");
        //        if(itFind != glslMap.end())
        //        {
        //            std::vector<GLSLData> uniforms = itFind->second;
        //            for( std::vector<GLSLData>::iterator it = uniforms.begin(); it != uniforms.end(); it++)
        //            {
        //                if(*it->type.compare("float") == 0)
        //                    shader->SetFloat(shader->GetVariable(*it->name), *it->value);
        //                else if(*it->type.compare("float2") == 0)
        //                {
        //                    float[2] data = *it->value;
        //                    shader->SetFloat2(shader->GetVariable(*it->name), data[0], data[1]);
        //                }
        //                else if(*it->type.compare("float3") == 0)
        //                {
        //                    float[3] data = *it->value;
        //                    shader->SetFloat3(shader->GetVariable(*it->name), data[0], data[1], data[2]);
        //                }
        //            }
        //        }

        glClampColorARB(GL_CLAMP_VERTEX_COLOR, GL_TRUE);
        glEnable(GL_VERTEX_PROGRAM_TWO_SIDE);
    }
}

bool ImplicitFieldShaderVisualization::isActive()
{
    return true;
}

void ImplicitFieldShaderVisualization::handleEvent(core::objectmodel::Event * event)
{
    if(MouseEvent* ev = dynamic_cast< MouseEvent *>(event))
    {
        if (ev->getState() == MouseEvent::Move)
        {
            mouseX = ev->getPosX();
            mouseY = ev->getPosY();
        }
        if (ev->getState() == MouseEvent::Wheel)
        {
            wheelDelta = ev->getWheelDelta();
        }
    }
}

bool ImplicitFieldShaderVisualization::drawScene(VisualParams* vp)
{
    return true;
}

std::string ImplicitFieldShaderVisualization::generateVertexShaderFrom()
{
    std::string vertexShaderText;
    vertexShaderText = std::string() +
            "attribute vec2 g;\n"
            "void main() \n"
            "{\n"
            "    gl_Position = vec4(g.xy, 1.0, 1.0);\n"
            "}\n";
    return vertexShaderText;
}

std::string ImplicitFieldShaderVisualization::implicitFunction()
{
    std::map<std::string, std::string> glslMap = l_field->getGLSLCode();
    std::map<std::string, std::string>::iterator it = glslMap.find("eval");
    std::string implicitFunction;
    implicitFunction.append(
                "    res = minVec2(\n"
                "        vec2(sdPlane(pos), 1.0),\n"
                "        vec2(sdSphere(pos-vec3( -.0, 0.75, 0.0), .5), 70.)\n"
                "   );    \n";
            ); /// Default value if eval is empty

    if(it != glslMap.end())
    {
        GLSLData data = *it->second;
        implicitFunction.clear();
        implicitFunction.append(
                    "    res = minVec2(\n"
                    "        vec2(sdPlane(pos), 1.0),\n"
                    );
        implicitFunction.append(data->value);
        implicitFunction.append(
                    "   );    \n";)
    }
    std::string tmp;
    tmp.append(
                "float sdPlane( vec3 p )\n"
                "{\n"
                "   return p.y;\n"
                "}\n"

                "float sdSphere( vec3 p, float s )\n"
                "{\n"
                "   return length(p)-s;\n"
                "}\n"

                "vec2 minVec2( vec2 d1, vec2 d2 )\n"
                "{\n"
                "    return (d1.x<d2.x) ? d1 : d2;\n"
                "}\n"
                );

    tmp.append(
                "vec2 map( in vec3 pos )\n"
                "{\n"
                "   vec2 res;\n"
                );

    tmp.append(implicitFunction);

    tmp.append(
                "   return res;\n"
                "}\n"
                );
    return tmp;
}

std::string ImplicitFieldShaderVisualization::viewFunction()
{
    std::string tmp;
    tmp.append(
                "mat3 setCamera( in vec3 ro, in vec3 ta, float cr )\n"
                "{\n"
                "        vec3 cw = normalize(ta-ro);\n"
                "        vec3 cp = vec3(sin(cr), cos(cr),0.0);\n"
                "        vec3 cu = normalize( cross(cw,cp) );\n"
                "        vec3 cv = normalize( cross(cu,cw) );\n"
                "    return mat3( cu, cv, cw );\n"
                "}\n"
                );
    return tmp;
}

std::string ImplicitFieldShaderVisualization::mainFragmenShaderFunction()
{
    std::string tmp;
    tmp.append(
                "void main()\n"
                "{\n"
                "    vec2 mouseR = mouse.xy/resolution.xy;\n"
                "    vec2 p = (-resolution.xy + 2.0*gl_FragCoord)/resolution.y;\n"
                "    vec3 ro = vec3( -0.5+3.5*cos(7.0*mouseR.x), .0 + 4.0*mouseR.y, 0.5 + 4.0*sin(7.0*mouseR.x) );\n"
                "    vec3 ta = vec3( 0.0, 0.0, 0.0 );\n"
                "    mat3 ca = setCamera( ro, ta, 0.0 );\n"
                "    vec3 rayDir = ca * normalize( vec3(p.xy, 2.0) );\n"
                "    gl_FragColor = vec4( render( ro, rayDir ), 1.0 );\n"
                "}\n"
                );
    return tmp;
}

std::string ImplicitFieldShaderVisualization::renderFunction()
{
    std::string tmp;
    tmp.append(
                "float res = 1.0;\n"
                "float t = mint;\n"
                "   for( int i=0; i<16; i++ )\n"
                "   {\n"
                "       float h = map( ro + rd*t ).x;\n"
                "       res = min( res, 8.0*h/t );\n"
                "       t += clamp( h, 0.02, 0.10 );\n"
                "       if( h<0.001 || t>tmax ) break;\n"
                "   }\n"
                "return clamp( res, 0.0, 1.0 );\n"
                "}\n"


                "vec3 estimateNormal(vec3 p) {\n"
                "    return normalize(vec3(\n"
                "        map(vec3(p.x + EPSILON, p.y, p.z)).x - map(vec3(p.x - EPSILON, p.y, p.z)).x,\n"
                "        map(vec3(p.x, p.y + EPSILON, p.z)).x - map(vec3(p.x, p.y - EPSILON, p.z)).x,\n"
                "        map(vec3(p.x, p.y, p.z  + EPSILON)).x - map(vec3(p.x, p.y, p.z - EPSILON)).x\n"
                "    ));\n"
                "}\n"

                "vec3 render( in vec3 ro, in vec3 rd )\n"
                "{\n"
                "    vec3 col = vec3(1.0, 1.0, 1.0) + rd.y;\n"
                "    vec2 res = castRay(ro,rd);\n"
                "    float t = res.x;\n"
                "    float m = res.y;\n"

                "    vec3 pos = ro + t * rd;\n"
                "    vec3 nor = estimateNormal( pos );\n"
                "    vec3 ref = reflect( rd, nor );\n"

                "    col = 0.45 + 0.35*sin( vec3(0.05,0.08,0.10)*(m-1.0) );\n"

                "    vec3  lig = normalize( vec3(-0.4, 0.7, -0.6) );\n"
                "    float amb = clamp( 0.5+0.5*nor.y, 0.0, 1.0 );\n"
                "    float dif = clamp( dot( nor, lig ), 0.0, 1.0 );\n"
                "    float bac = clamp( dot( nor, normalize(vec3(-lig.x,0.0,-lig.z))), 0.0, 1.0 )*clamp( 1.0-pos.y,0.0,1.0);\n"
                "    float dom = smoothstep( -0.1, 0.1, ref.y );\n"
                "    float fre = pow( clamp(1.0+dot(nor,rd),0.0,1.0), 2.0 );\n"
                "    float spe = pow(clamp( dot( ref, lig ), 0.0, 1.0 ),16.0);\n"

                "    dif *= softshadow( pos, lig, 0.02, 2.5 );\n"
                "    dom *= softshadow( pos, ref, 0.02, 2.5 );\n"

                "    vec3 lin = vec3(0.0);\n"
                "    lin += 1.30*dif*vec3(1.00,0.80,0.55);\n"
                "    lin += 2.00*spe*vec3(1.00,0.90,0.70)*dif;\n"
                "    lin += 0.40*amb*vec3(0.40,0.60,1.00);\n"
                "    lin += 0.50*dom*vec3(0.40,0.60,1.00);\n"
                "    col = col*lin;\n"
                "    col = mix( col, vec3(0.9,0.9,1.0), 1.0-exp( -0.0002*t*t*t ) );\n"

                "    return vec3( clamp(col,0.0,1.0) );\n"
                "}\n"
                );
    return tmp;
}

std::string ImplicitFieldShaderVisualization::rayMarchingFunction()
{
    /// TODO replace gradient function by l_field gradient function from map
    std::string tmp;
    tmp.append(
                "vec2 castRay( in vec3 ro, in vec3 rd )\n"
                "{\n"
                "    float depth = MIN_DIST;\n"
                "    float m = -1.0;\n"
                "    for( int i = 0; i < MAX_MARCHING_STEPS; i++ )\n"
                "    {\n"
                "            vec2 res = map( ro+rd*depth );\n"
                "        if( res.x < EPSILON || depth > MAX_DIST ) break;\n"
                "        depth += res.x;\n"
                "            m = res.y;\n"
                "    }\n"
                "    return vec2( depth, m );\n"
                "}\n"
                );
    return tmp;
}

std::string ImplicitFieldShaderVisualization::uniformsAndConst()
{
    std::string tmp;
    tmp.append(
                "uniform vec2 resolution;\n"
                "uniform vec2 mouse;\n"
                "uniform float wheelDelta;\n"

                "const int MAX_MARCHING_STEPS = 255;\n"
                "const float MIN_DIST = 0.0;\n"
                "const float MAX_DIST = 100.0;\n"
                "const float EPSILON = 0.00001;\n"
                );
    /// TODO
    //    std::map<std::string, std::string> glslMap = l_field->getGLSLCode();
    //    std::map<std::string, std::string>::iterator itFind = glslMap.find("uniforms");
    //    if(itFind != glslMap.end())
    //    {
    //        std::vector<GLSLData> uniforms = itFind->second;
    //        for( std::vector<GLSLData>::iterator it = uniforms.begin(); it != uniforms.end(); it++)
    //            tmp.append(*it->variable + " " + *it->type + " " *it->name);
    //    }

    return tmp;
}

std::string ImplicitFieldShaderVisualization::generateFragmentShaderFrom()
{
    std::string fragmentShaderText;
    fragmentShaderText = std::string() + "";

    fragmentShaderText += uniformsAndConst();
    fragmentShaderText += rotationFunction();
    fragmentShaderText += implicitFunction();
    fragmentShaderText += rayMarchingFunction();
    fragmentShaderText += phongFunction();
    fragmentShaderText += viewFunction();
    fragmentShaderText += mainFragmenShaderFunction();

    return fragmentShaderText;
}


} /// namespace _pointcloudimplicitfieldvisualization_
} /// namespace visual
} /// namespace component
} /// namespace sofa
