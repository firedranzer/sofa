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

#include <sofa/core/ObjectFactory.h>
using sofa::core::RegisterObject ;

#include "PointCloudImplicitFieldVisualization.h"
using sofa::core::objectmodel::ComponentState ;

#ifndef SOFA_NO_OPENGL
#include <GL/gl.h>
#endif //SOFA_NO_OPENGL

namespace sofa
{

namespace component
{

namespace visual
{

namespace _pointcloudimplicitfieldvisualization_
{

///factory register
int Class = RegisterObject("A cheap visualization of implicit field using colored points.")
        .add< PointCloudImplicitFieldVisualization >() ;

PointCloudImplicitFieldVisualization::PointCloudImplicitFieldVisualization() :
     l_field(initLink("field", "The field to render."))
{

}

void PointCloudImplicitFieldVisualization::init()
{
    if(l_field.empty() == false)
    {
        m_componentstate = ComponentState::Invalid;
        msg_error() << "Missing the 'field' attribute. The component is disabled until properly set." ;
        return ;
    }
    m_componentstate = ComponentState::Valid ;
}

void PointCloudImplicitFieldVisualization::reinit()
{
   init() ;
}


void PointCloudImplicitFieldVisualization::draw(const core::visual::VisualParams *)
{
    if( m_componentstate != ComponentState::Valid )
        return ;

#ifndef SOFA_NO_OPENGL
    glBegin(GL_POINTS);
    for(unsigned int i=0;i<10;i++)
    {
        for(unsigned int j=0;j<10;j++)
        {
            for(unsigned int k=0;k<10;k++)
            {
                Vec3d pos { i,j,k }  ;
                l_field.get()->getValue(pos) ;
                glColor4d(1.0,1.0,1.0,1.0) ;
                glVertex3d(i,j,0) ;
            }
        }
    }
    glEnd();

#endif //SOFA_NO_OPENGL
}


SOFA_DECL_CLASS(PointCloudImplicitFieldVisualization)

} /// namespace _scalarfield_

} /// namespace geometry

} /// namespace component

} /// namespace sofa
