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

        /// TODO se mettre d'accord sur le contenu des maps :/
        //        std::map<std::string, std::string> glslMap = l_field->getGLSLCode();
        //        std::map<std::string, std::string>::iterator it = glslMap.iterator();
        //        for (it = glslMap.begin(); it != glslMap.end(); ++it)
        //        {
        //            if ((std::string)(tmp->first).compare("eval") != 0)
        //            {
        //                tmp.append("uniform ");
        //                tmp.append(tmp->first);
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

std::string ImplicitFieldShaderVisualization::rotationFunction()
{
    std::string tmp;
    tmp.append(
                "mat3 rotateX(float theta) {\n"
                "    float c = cos(theta);\n"
                "    float s = sin(theta);\n"
                "    return mat3(\n"
                "        vec3(1, 0, 0),\n"
                "        vec3(0, c, -s),\n"
                "        vec3(0, s, c)\n"
                "    );\n"
                "}\n"
                "\n"
                "mat3 rotateY(float theta) {\n"
                "    float c = cos(theta);\n"
                "    float s = sin(theta);\n"
                "    return mat3(\n"
                "        vec3(c, 0, s),\n"
                "        vec3(0, 1, 0),\n"
                "        vec3(-s, 0, c)\n"
                "    );\n"
                "}\n");
    return tmp;
}

std::string ImplicitFieldShaderVisualization::implicitFunction()
{
    std::map<std::string, std::string> glslMap = l_field->getGLSLCode();
    std::map<std::string, std::string>::iterator it = glslMap.find("eval");
    std::string implicitFunction = "0.0";
    if(it != glslMap.end())
        implicitFunction = it->second;
    std::string tmp;
    tmp.append(
                "float sceneSDF(vec3 p) {\n"
                "    p = rotateY(10.*mouse.x/resolution.x) * p;\n"
                "    p = -rotateX(10.*mouse.y/resolution.y) * p;\n"
                "    float x = p.x;\n"
                "    float y = p.y;\n"
                "    float z = p.z;\n"
                "    float value = 0.0;\n"
                "    vec3 h = vec3(1.0, 2.0, 1.0);\n"
                "    vec3 q = abs(p);\n"
                "    return ");
    tmp.append(implicitFunction);
    tmp.append(";\n"
               "}\n");
    return tmp;
}

std::string ImplicitFieldShaderVisualization::viewFunction()
{
    std::string tmp;
    tmp.append(
                "mat3 viewMatrix(vec3 eye, vec3 center, vec3 up) {\n"
                "    // Based on gluLookAt man page\n"
                "    vec3 f = normalize(center - eye);\n"
                "    vec3 s = normalize(cross(f, up));\n"
                "    vec3 u = cross(s, f);\n"
                "    return mat3(s, u, -f);\n"
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
                "    vec3 viewDir = rayDirection(90.0, resolution, gl_FragCoord.xy);\n"
                "    vec3 eye = vec3(1.0, 1.0 , 7.0);\n"
                "     mat3 viewToWorld = viewMatrix(eye, vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));\n"
                "     vec3 worldDir = viewToWorld * viewDir;\n"
                "     float dist = shortestDistanceToSurface(eye, worldDir, MIN_DIST, MAX_DIST);\n"
                "     if (dist > MAX_DIST - EPSILON) {\n"
                "         gl_FragColor = vec4(.0, 0.0, 0.0, 0.0);\n"
                "         return;\n"
                "     }\n"
                "     vec3 p = eye + dist * worldDir;\n"
                "     vec3 K_a = (estimateNormal(p) + vec3(1.0)) / 2.0;\n"
                "     vec3 K_d = K_a;\n"
                "     vec3 K_s = vec3(1.0, 1.0, 1.0);\n"
                "     float shininess = 10.0;\n"
                "     vec3 color = phongIllumination(K_a, K_d, K_s, shininess, p, eye);\n"
                "     gl_FragColor = vec4(color, 1.0);\n"
                "}\n"
                );
    return tmp;
}

std::string ImplicitFieldShaderVisualization::phongFunction()
{
    std::string tmp;
    tmp.append(

                "vec3 phongContribForLight(vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye, vec3 lightPos, vec3 lightIntensity) {\n"
                "    vec3 N = estimateNormal(p);\n"
                "    vec3 L = normalize(lightPos - p);\n"
                "    vec3 V = normalize(eye - p);\n"
                "    vec3 R = normalize(reflect(-L, N));\n"
                "    float dotLN = dot(L, N);\n"
                "    float dotRV = dot(R, V);\n"
                "    if (dotLN < 0.0) {\n"
                "        // Light not visible from this point on the surface\n"
                "        return vec3(0.0, 0.0, 0.0);\n"
                "    }\n"
                "    if (dotRV < 0.0) {\n"
                "        return lightIntensity * (k_d * dotLN);\n"
                "    }\n"
                "    return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));\n"
                "}\n"
                "\n"
                "vec3 phongIllumination(vec3 k_a, vec3 k_d, vec3 k_s, float alpha, vec3 p, vec3 eye) {\n"
                "    const vec3 ambientLight = 0.5 * vec3(1.0, 1.0, 1.0);\n"
                "    vec3 color = ambientLight * k_a;\n"
                "    vec3 light1Pos = vec3(4.0, 2.0, 4.0);\n"
                "    vec3 light1Intensity = vec3(0.4, 0.4, 0.4);\n"
                "    color += phongContribForLight(k_d, k_s, alpha, p, eye,\n"
                "    light1Pos,\n"
                "    light1Intensity);\n"
                "    return color;\n"
                "}\n"
                );
    return tmp;
}

std::string ImplicitFieldShaderVisualization::rayMarchingFunction()
{
    /// TODO replace gradient function by l_field gradient function from map
    std::string tmp;
    tmp.append(

                "float shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, float start, float end) {\n"
                "    float depth = start;\n"
                "    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {\n"
                "        float dist = sceneSDF(eye + depth * marchingDirection);\n"
                "        if (dist < EPSILON) {\n"
                "            return depth;\n"
                "        }\n"
                "        depth += dist;\n"
                "        if (depth >= end) {\n"
                "            return end;\n"
                "        }\n"
                "    }\n"
                "    return end;\n"
                "}\n"

                "vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {\n"
                "    vec2 xy = fragCoord - size / 2.0;\n"
                "    float z = size.y / tan(radians(fieldOfView) / 2.0);\n"
                "    return normalize(vec3(xy, -z));\n"
                "}\n"

                "vec3 estimateNormal(vec3 p) {\n"
                "    return normalize(vec3(\n"
                "    sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),\n"
                "    sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),\n"
                "    sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))\n"
                "    ));\n"
                "}\n"
                );
    return tmp;
}

std::string ImplicitFieldShaderVisualization::uniformsAndConst()
{
    /// TODO fetch data from l_field map
    ///
    std::map<std::string, std::string> glslMap = l_field->getGLSLCode();
    std::string tmp;
    tmp.append(
                "uniform vec2 resolution;\n"
                "uniform vec2 mouse;\n"

                "const int MAX_MARCHING_STEPS = 255;\n"
                "const float MIN_DIST = 0.0;\n"
                "const float MAX_DIST = 100.0;\n"
                "const float EPSILON = 0.00001;\n"
                );
    std::map<std::string, std::string>::iterator it = glslMap.begin();

    for (it = glslMap.begin(); it != glslMap.end(); ++it)
    {
        if (std::string(it->first).compare("eval") != 0)
        {
            tmp.append("uniform ");
            tmp.append(it->first);
            tmp.append(";\n");
        }
    }

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
