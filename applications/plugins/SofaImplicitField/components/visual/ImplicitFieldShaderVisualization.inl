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
    l_field(initLink("field", "The field to render.")),
    d_vertFilename(initData(&d_vertFilename, std::string("shaders/implicitShape.vert"), "fileVertexShaders", "Set the vertex shader filename to load")),
    d_fragFilename(initData(&d_fragFilename, std::string("shaders/implicitShape.frag"), "fileFragmentShaders", "Set the fragment shader filename to load"))
{
    f_listening.setValue(true);
    wheelDelta = 0.5;
    changedFromDataField = false;
}

ImplicitFieldShaderVisualization::~ImplicitFieldShaderVisualization()
{
    shader->TurnOff();
    shader->Release();
    delete shader;
}

void ImplicitFieldShaderVisualization::init()
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float pixelSize[2];
    pixelSize[0] = viewport[2];
    pixelSize[1] = viewport[3];
    mouseX = pixelSize[0] /2.0;
    mouseY = pixelSize[1] /2.0;

    shader = new sofa::helper::gl::GLSLShader();

    shader->SetVertexShaderFileName(d_vertFilename.getFullPath());
    shader->SetFragmentShaderFileName(d_fragFilename.getFullPath());
    m_datatracker.trackData(*l_field.get()->findData("state"));
    shaderGenerationCodeHasChanged();

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
    for(std::vector<DataTracker*>::iterator it = m_datatrackerList.begin(); it != m_datatrackerList.end(); it++)
    {

        DataTracker* tmp = *it;
        if(tmp->isDirty())
        {
            changedFromDataField = true;
            tmp->clean();
            shaderGenerationCodeHasChanged();
            return;
        }
    }
}

void ImplicitFieldShaderVisualization::shaderGenerationCodeHasChanged()
{
    if (!changedFromDataField)
        initComponentShaderValue();
    std::ofstream myfile;
    myfile.open (d_fragFilename.getFullPath());
    myfile << generateFragmentShader();
    myfile.close();

    myfile;
    myfile.open (d_vertFilename.getFullPath());
    myfile << generateVertexShader();
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
            BaseData* data = fetchData(uniformName);
            data->read(uniformValue);
            DataTracker* tmp = new DataTracker();
            tmp->trackData(*data);
            m_datatrackerList.push_back(tmp);
        }
    }
}

void ImplicitFieldShaderVisualization::start()
{

    // Temporarily set the numeric formatting locale to ensure that
    // floating-point values are interpreted correctly by tinyXML. (I.e. the
    // decimal separator is a dot '.').
    helper::system::TemporaryLocale locale(LC_NUMERIC, "C");

    if(m_datatracker.isDirty())
    {
        std::ofstream myfile;
        myfile.open (d_fragFilename.getFullPath());
        myfile << generateFragmentShader();
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

        sofa::simulation::Node::SPtr root = down_cast<sofa::simulation::Node>( this->getContext()->getRootContext() );
        if(root)
        {
            sofa::component::visualmodel::InteractiveCamera * currentCamera = root->getNodeObject<sofa::component::visualmodel::InteractiveCamera>();

            if(currentCamera)
            {
                // Set the current camera's position in recorded camera for navigation
                currentCamera->getPosition().at(0);

                shader->SetFloat3(shader->GetVariable("position"),
                                  currentCamera->getPosition().at(0),
                                  currentCamera->getPosition().at(1),
                                  currentCamera->getPosition().at(2));
                shader->SetFloat3(shader->GetVariable("lookAt"),
                                  currentCamera->getLookAt().at(0),
                                  currentCamera->getLookAt().at(1),
                                  currentCamera->getLookAt().at(2));
                shader->SetFloat4(shader->GetVariable("rotation"),
                                  currentCamera->getOrientation()[0],
                                  currentCamera->getOrientation()[1],
                                  currentCamera->getOrientation()[2],
                                  currentCamera->getOrientation()[3]);
                shader->SetFloat(shader->GetVariable("fov"), currentCamera->getFieldOfView());
            } else
            {
                shader->SetFloat3(shader->GetVariable("position"), 0.0, 1.0, -5.0);
                shader->SetFloat3(shader->GetVariable("lookAt"), 0.0, 0.0, 0.0);
                shader->SetFloat4(shader->GetVariable("rotation"), 0.0, 0.0, 0.0, 0.0);
            }
        } else
        {
            shader->SetFloat3(shader->GetVariable("position"), 0.0, 1.0, -5.0);
            shader->SetFloat3(shader->GetVariable("lookAt"), 0.0, 0.0, 0.0);
            shader->SetFloat4(shader->GetVariable("rotation"), 0.0, 0.0, 0.0, 0.0);
        }



        shader->SetFloat2(shader->GetVariable("resolution"), pixelSize[0], pixelSize[1]);
        shader->SetFloat2(shader->GetVariable("mouse"), mouseX, mouseY);
        shader->SetFloat(shader->GetVariable("wheelDelta"), wheelDelta);

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

                BaseData* data = fetchData(uniformName);
                uniformValue = data->getValueString();

                std::regex rgx("\\s+");
                std::sregex_token_iterator iter(uniformValue.begin(), uniformValue.end(), rgx, -1);
                std::sregex_token_iterator end;

                if(uniformType.compare("float") == 0)
                    shader->SetFloat(shader->GetVariable(uniformName), std::atof(std::string((*iter)).c_str()));
                else if(uniformType.compare("vec2") == 0)
                {
                    float data[2];
                    data[0] = std::atof(std::string((*iter)).c_str());
                    data[1] = std::atof(std::string((*++iter)).c_str());
                    shader->SetFloat2(shader->GetVariable(uniformName), data[0], data[1]);
                }
                else if(uniformType.compare("vec3") == 0)
                {
                    float data[3];
                    data[0] = std::atof(std::string((*iter)).c_str());
                    data[1] = std::atof(std::string((*++iter)).c_str());
                    data[2] = std::atof(std::string((*++iter)).c_str());
                    shader->SetFloat3(shader->GetVariable(uniformName), data[0], data[1], data[2]);
                }
            }
        }

        glClampColorARB(GL_CLAMP_VERTEX_COLOR, GL_TRUE);
        glEnable(GL_VERTEX_PROGRAM_TWO_SIDE);
    }
}

BaseData* ImplicitFieldShaderVisualization::fetchData(std::string argumentName)
{
    MapData dataMap = getDataAliases();
    MapData::const_iterator itData = dataMap.find(argumentName);
    BaseData* data;

    if (itData == dataMap.end())
    {
        data = new sofa::core::objectmodel::Data<std::string>();
        if (data == nullptr)
            msg_warning() << " Something went wrong while creating data";
        else
        {
            data->setName(argumentName);
            data->setHelp("Auto generated help from ImplicitFieldShaderVisualization");
            addData(data, argumentName);
            msg_info(getName()) << " data field named : " << argumentName << " has been created";
        }
    } else
        data = itData->second;
    return data;
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
            wheelDelta += ev->getWheelDelta()/120 * 0.1;
    }
}

bool ImplicitFieldShaderVisualization::drawScene(VisualParams* vp)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////
/// VERTEX SHADER GENERATION PART                                      ///
//////////////////////////////////////////////////////////////////////////

std::string ImplicitFieldShaderVisualization::generateVertexShader()
{
    std::string vertexShaderText;
    vertexShaderText = std::string() +
            "attribute vec2 g;\n"
            "void main() \n"
            "{\n"
            "   gl_Position = vec4(g.xy, 1.0, 1.0);\n"
            "}\n";
    return vertexShaderText;
}

//////////////////////////////////////////////////////////////////////////
/// FRAGMENT SHADER GENERATION PART                                    ///
//////////////////////////////////////////////////////////////////////////

std::string ImplicitFieldShaderVisualization::uniformsAndConst()
{
    std::string tmp;
    tmp.append(
                "uniform vec2 resolution;\n"
                "uniform vec2 mouse;\n"
                "uniform float wheelDelta;\n"
                "uniform vec3 position;\n"
                "uniform vec3 lookAt;\n"
                "uniform vec4 rotation;\n"
                "uniform float fov;\n"
                "\n"
                "const int MAX_MARCHING_STEPS = 255;\n"
                "const float MIN_DIST = 0.0;\n"
                "const float MAX_DIST = 100.0;\n"
                "const float EPSILON = 0.00001;\n"
                "\n"
                );

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
    std::string tmp;
    std::string implicitFunction;
    implicitFunction.append(
                "   res = minVec4(\n"
                "       vec4(sdPlane(pos), vec3(0.45, 0.45, 0.45)),\n"
                "       vec4(sdSphere(pos-vec3( -.0, 0.75, 0.0), .5), vec3(1.0, 1.0, 0.0))\n"
                "   );    \n"
                ); /// Default value if eval is empty

    ///CORRECT VERSION
    //    std::map<std::string, std::vector<GLSLCodeFragment>> glslMap = l_field->getGLSLCode();
    //    std::map<std::string, std::vector<GLSLCodeFragment>>::iterator itFind = glslMap.find("eval");

    //    if(itFind != glslMap.end())
    //    {
    //        implicitFunction.clear();
    //        std::vector<GLSLCodeFragment> evals = itFind->second;
    //        for( std::vector<GLSLCodeFragment>::iterator it = evals.begin(); it != evals.end(); it++)
    //        {
    //            GLSLCodeFragment shaderCode = *it;
    //            std::string name = shaderCode.m_name;
    //            std::string dataName = shaderCode.m_dataname;
    //            std::string value = shaderCode.m_value;

    //            if (changedFromDataField)
    //            {
    //                BaseData* data = fetchData(name);
    //                value = data->getValueString();
    //            }
    //            implicitFunction.append(
    //                        "   x = pos.x - evalPosition" + dataName + ".x;\n"
    //                        "   y = pos.y - evalPosition" + dataName + ".y;\n"
    //                        "   z = pos.z - evalPosition" + dataName + ".z;\n"
    //                        );
    //            implicitFunction.append(
    //                        "   res = minVec4(\n"
    //                        "       res,\n"
    //                        );

    //            implicitFunction.append("\t\tvec4(" + value + ", evalColor" + dataName + ")\n");
    //            implicitFunction.append("   );    \n");
    //        }
    //    }


    /// THOMAS TEST
    implicitFunction.clear();
    implicitFunction.append(
                "   res = minVec4(\n"
                "       res,\n"
                );
    implicitFunction.append("\t\tvec4( thomasFunc(vec3(x,y,z)), evalColorSphere1)\n");
    implicitFunction.append("   );    \n");
    ///



    changedFromDataField = false;

    tmp.append(
                "float sdPlane( vec3 p )\n"
                "{\n"
                "   return p.y;\n"
                "}\n"
                "float sdSphere( vec3 p, float s )\n"
                "{\n"
                "   return length(p)-s;\n"
                "}\n"
                "\n"
                "float thomasFunc( vec3 p )\n"
                "{\n"
                "   float x = p.x;\n"
                "   float y = p.y;\n"
                "   float z = p.z;\n\n");
    /// :S only for thomas dev ... we need to remove it later
    std::string file = sofa::helper::system::SetDirectory::GetCurrentDir() + "/../../../../tools/geneticalgo/geneticalgo/erwan.txt";
    std::cout << file << std::endl;
    std::ifstream ifs(file);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    tmp.append(content);
    tmp.append(
                "}\n"
                "\n"
                "vec4 minVec4( vec4 d1, vec4 d2 )\n"
                "{\n"
                "   return (d1.x<d2.x) ? d1 : d2;\n"
                "}\n"
                "\n"
                );

    tmp.append(
                "vec4 map( in vec3 pos )\n"
                "{\n"
                "   float x = pos.x;\n"
                "   float y = pos.y;\n"
                "   float z = pos.z;\n"
                "   vec4 res = vec4(sdPlane(pos), vec3(0.45, 0.45, 0.45));\n"
                );

    tmp.append(implicitFunction);

    tmp.append(
                "   return res;\n"
                "}\n"
                );
    tmp.append("\n");

    return tmp;
}

std::string ImplicitFieldShaderVisualization::rayMarchingFunction()
{
    std::string tmp;
    tmp.append(
                "vec4 castRay( in vec3 ro, in vec3 rd )\n"
                "{\n"
                "   float depth = MIN_DIST;\n"
                "   vec3 m = vec3(-1.0, -1.0, -1.0);\n"
                "   for( int i = 0; i < MAX_MARCHING_STEPS; i++ )\n"
                "   {\n"
                "       vec4 res = map( ro+rd*depth );\n"
                "       if( res.x < EPSILON || depth > MAX_DIST ) break;\n"
                "       depth += res.x*0.2;\n" /// TODO OPTIMIZATION :s crappy stuff here
                "           m = res.yzw;\n"
                "   }\n"
                "   return vec4( depth, m );\n"
                "}\n"
                );
    tmp.append("\n");
    return tmp;
}

std::string ImplicitFieldShaderVisualization::renderFunction()
{
    std::string tmp;
    tmp.append(
                "float softshadow( in vec3 ro, in vec3 rd, in float mint, in float tmax )\n"
                "{\n"
                "   float res = 1.0;\n"
                "   float t = mint;\n"
                "   for( int i=0; i<16; i++ )\n"
                "   {\n"
                "      float h = map( ro + rd*t ).x;\n"
                "      res = min( res, 8.0*h/t );\n"
                "      t += clamp( h, 0.02, 0.10 );\n"
                "      if( h<0.001 || t>tmax ) break;\n"
                "   }\n"
                "   return clamp( res, 0.0, 1.0 );\n"
                "}\n"
                "\n"
                "vec3 estimateNormal(vec3 p) {\n"
                "   return normalize(vec3(\n"
                "       map(vec3(p.x + EPSILON, p.y, p.z)).x - map(vec3(p.x - EPSILON, p.y, p.z)).x,\n"
                "       map(vec3(p.x, p.y + EPSILON, p.z)).x - map(vec3(p.x, p.y - EPSILON, p.z)).x,\n"
                "       map(vec3(p.x, p.y, p.z  + EPSILON)).x - map(vec3(p.x, p.y, p.z - EPSILON)).x\n"
                "   ));\n"
                "}\n"
                "\n"
                "vec3 render( in vec3 ro, in vec3 rd )\n"
                "{\n"
                "   vec4 res = castRay(ro,rd);\n"
                "   if (res.x > MAX_DIST) return vec3(0.0, 0.0, 0.0)\n;"
                "\n"
                "   float dist = res.x;\n"
                "   vec3 col = res.yzw;\n"
                "   vec3 pos = ro + dist * rd;\n"
                "   vec3 nor = estimateNormal( pos );\n"
                "   vec3 ref = reflect( rd, nor );\n"
                "\n"
                "   vec3  lig = normalize(ro);\n"
                "   float amb = clamp( 0.5+0.5*nor.y, 0.0, 1.0 );\n"
                "   float dif = clamp( dot( nor, lig ), 0.0, 1.0 );\n"
                "   float dom = smoothstep( -0.1, 0.1, ref.y );\n"
                "   float spe = pow(clamp( dot( ref, lig ), 0.0, 1.0 ),16.0);\n"
                "   dif *= softshadow( pos, lig, 0.02, 2.5 );\n"
                "   dom *= softshadow( pos, ref, 0.02, 2.5 );\n"
                "\n"
                "   vec3 lin = vec3(0.0);\n"
                "   lin += 1.30*dif*vec3(1.00,0.80,0.55);\n"
                "   lin += 2.00*spe*vec3(1.00,0.90,0.70)*dif;\n"
                "   lin += 0.40*amb*vec3(0.40,0.60,1.00);\n"
                "   lin += 0.50*dom*vec3(0.40,0.60,1.00);\n"
                "   col = col*lin;\n"
                "\n"
                "   return vec3( clamp(col,0.0,1.0) );\n"
                "}\n"
                );
    tmp.append("\n");
    return tmp;
}

std::string ImplicitFieldShaderVisualization::viewFunction()
{
    std::string tmp;
    tmp.append(

                "vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {\n"
                "   vec2 xy = fragCoord - size / 2.0;\n"
                "   float z = size.y / tan(radians(fieldOfView));\n"
                "   return normalize(vec3(xy, -z));\n"
                "}\n"
                "\n"
                "mat3 viewMatrix(vec3 eye, vec3 center, vec3 up) {\n"
                "    vec3 f = normalize(center - eye);\n"
                "    vec3 s = normalize(cross(f, up));\n"
                "    vec3 u = cross(s, f);\n"
                "    return mat3(s, u, -f);\n"
                "}\n"
                );
    tmp.append("\n");
    return tmp;
}

std::string ImplicitFieldShaderVisualization::mainFragmenShaderFunction()
{
    std::string tmp;
    tmp.append(
                "void main()\n"
                "{\n"
                "   vec3 viewDir = rayDirection(fov, resolution.xy, gl_FragCoord.xy);\n"
                "   mat3 viewToWorld = viewMatrix(position, lookAt, rotation.xyz);\n"
                "   vec3 worldDir = viewToWorld * viewDir;\n"
                "   gl_FragColor = vec4( render( position, worldDir ), 1.0 );\n"
                "}\n"
                );
    tmp.append("\n");
    return tmp;
}

std::string ImplicitFieldShaderVisualization::generateFragmentShader()
{
    std::string fragmentShaderText;
    fragmentShaderText = std::string() + "";

    fragmentShaderText += uniformsAndConst();
    fragmentShaderText += implicitFunction();
    fragmentShaderText += rayMarchingFunction();
    fragmentShaderText += renderFunction();
    fragmentShaderText += viewFunction();
    fragmentShaderText += mainFragmenShaderFunction();

    return fragmentShaderText;
}


} /// namespace _pointcloudimplicitfieldvisualization_
} /// namespace visual
} /// namespace component
} /// namespace sofa
