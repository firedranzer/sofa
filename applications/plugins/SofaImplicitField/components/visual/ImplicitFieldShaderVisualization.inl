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
    l_xShape(initLink("shape", "The shape to render.")),
    d_vertFilename(initData(&d_vertFilename, std::string("shaders/implicitShape.vert"), "fileVertexShaders", "Set the vertex shader filename to load")),
    d_fragFilename(initData(&d_vertFilename, std::string("shaders/implicitShape.frag"), "fileFragmentShaders", "Set the fragment shader filename to load"))
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
    if(shader->IsReady())
    {
        shader->TurnOn();
        shader->SetFloat3(shader->GetVariable("color"), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0)), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0)), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0)));

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float pixelSize[2];
        pixelSize[0] = viewport[2];
        pixelSize[1] = viewport[3];

        shader->SetFloat2(shader->GetVariable("resolution"), pixelSize[0], pixelSize[1]);
        shader->SetFloat2(shader->GetVariable("mouse"), mouseX, mouseY);

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
    // Mouse Events
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

}

std::string ImplicitFieldShaderVisualization::generateFragmentShaderFrom()
{
    std::string fragmentShaderText;
    fragmentShaderText =
            R"SHADER_DELIM(
            )SHADER_DELIM";
    return fragmentShaderText;
}

std::string ImplicitFieldShaderVisualization::generateVertexShaderFrom()
{
    std::string vertexShaderText;
    vertexShaderText =
            R"SHADER_DELIM(
            )SHADER_DELIM";
    return vertexShaderText;
}


} /// namespace _pointcloudimplicitfieldvisualization_
} /// namespace visual
} /// namespace component
} /// namespace sofa
