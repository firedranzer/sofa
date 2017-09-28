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
#ifndef SOFA_ImplicitFieldShaderVisualization_H
#define SOFA_ImplicitFieldShaderVisualization_H

#include <SofaImplicitField/config.h>

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <regex>
#include <stdlib.h>


#include <sofa/core/ObjectFactory.h>
using sofa::core::RegisterObject;

#include <sofa/core/visual/VisualModel.h>
#include <sofa/core/visual/Shader.h>
#include <sofa/helper/gl/GLSLShader.h>

#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/Link.h>

#include <SofaImplicitField/components/geometry/ScalarField.h>
using  sofa::component::geometry::ScalarField;

#include <sofa/core/objectmodel/MouseEvent.h>
using sofa::core::objectmodel::MouseEvent;

#include <sofa/core/objectmodel/DataFileName.h>

using sofa::core::visual::VisualParams;

#include <SofaImplicitField/components/geometry/CustomField.h>
using sofa::component::geometry::_customfield_::CustomField;
using sofa::component::geometry::_customfield_::GLSLCodeFragment;

#include <sofa/core/DataEngine.h>
using sofa::core::DataEngine ;
using sofa::core::DataTracker ;

#include <sofa/helper/system/Locale.h>

namespace sofa
{

namespace component
{

namespace visual
{

namespace _pointcloudimplicitfieldvisualization_
{

class SOFA_SOFAIMPLICITFIELD_API ImplicitFieldShaderVisualization: public core::visual::Shader, public core::visual::VisualModel
{
public:

    SOFA_CLASS2(ImplicitFieldShaderVisualization, core::visual::Shader, core::visual::VisualModel);

    ImplicitFieldShaderVisualization() ;
    virtual ~ImplicitFieldShaderVisualization() ;

    void initVisual();
    void init();
    void reinit();

    void start();
    void stop();
    bool isActive();
    bool drawScene(core::visual::VisualParams* vp);

    void handleEvent(core::objectmodel::Event * event);

    virtual bool insertInNode( core::objectmodel::BaseNode* node ) { Inherit1::insertInNode(node); Inherit2::insertInNode(node); return true; }
    virtual bool removeInNode( core::objectmodel::BaseNode* node ) { Inherit1::removeInNode(node); Inherit2::removeInNode(node); return true; }

    SingleLink<ImplicitFieldShaderVisualization,  CustomField, BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> l_field ;
    void shaderGenerationCodeHasChanged();

    sofa::core::objectmodel::DataFileName d_vertFilename;
    sofa::core::objectmodel::DataFileName d_fragFilename;


protected:
    int mouseX, mouseY;
    float wheelDelta;
    sofa::helper::gl::GLSLShader* shader;

    std::string generateFragmentShader();
    std::string generateVertexShader();


    std::string uniformsAndConst();
    std::string implicitFunction();
    std::string rayMarchingFunction();
    std::string renderFunction();
    std::string viewFunction();
    std::string mainFragmenShaderFunction();

private:
    DataTracker m_datatracker ;


};

} /// namespace _pointcloudimplicitfieldvisualization_
} /// namespace visual
} /// namespace component
} /// namespace sofa

#endif // SOFA_ImplicitFieldShaderVisualization_H
