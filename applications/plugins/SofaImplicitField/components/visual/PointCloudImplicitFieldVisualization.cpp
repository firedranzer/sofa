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

#include <sofa/core/visual/DrawToolGL.h>
using sofa::core::visual::DrawToolGL ;

#include <sofa/core/visual/VisualParams.h>
using sofa::core::visual::VisualParams ;


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
    if(l_field.getSize() == 0)
    {
        m_componentstate = ComponentState::Invalid;
        msg_error() << "Missing the 'field' attribute. The component is disabled until properly set." ;
        return ;
    }
    m_componentstate = ComponentState::Valid ;

    m_colors.clear();
    m_field.clear();
    m_points.clear();
    m_normals.clear();
    f_bbox.setValue(sofa::defaulttype::BoundingBox(0,1,0,1,0,1));

    m_datatracker.trackData(*l_field.get()->findData("state"));
}

void PointCloudImplicitFieldVisualization::reinit()
{
   init() ;
}

void PointCloudImplicitFieldVisualization::draw(const core::visual::VisualParams *params)
{
    if( m_componentstate != ComponentState::Valid )
        return ;

    static double minv = +1000;
    static double maxv = 0.0;

    std::vector<float> size = {256,256,256,8,4,2,2,2,2,2,2};
    static int m_step = 0 ;
    static int m_sliceX = 0 ;

    if(m_datatracker.isDirty()){
        m_points.clear();
        m_colors.clear();
        m_field.clear();
        m_normals.clear();
        m_datatracker.clean();
        m_step = 0;
        m_sliceX = 0;
    }

    if(m_step < 8)
    {
        minv = +1000;
        maxv = -1000;

        int numpoints = (1 << m_step)*3 ;
        double incr = 1.0/numpoints ;
        double x  = m_sliceX*incr ;
        double y  = 0 ;
        double z  = 0 ;

        int previousstep = m_points.size() ;
        for(;m_sliceX<numpoints && m_points.size()<previousstep+1000;m_sliceX++)
        {
            x += incr ;
            y = 0 ;
            for(unsigned int j=0;j<numpoints;j++)
            {
                y += incr;
                z = 0;
                for(unsigned int k=0;k<numpoints;k++)
                {
                    z += incr;
                    Vec3d pos { x, y, z }  ;
                    double dd = l_field.get()->getValue(pos) ;
                    double d = fabs(dd) ;
                    if( d < 0.001 )
                    {
                        m_points.push_back(pos);
                        m_field.push_back(dd) ;
                        m_normals.push_back( l_field.get()->getGradient(pos).normalized() );
                    }
                    if (dd < 0.0 ) {
                        //m_points.push_back(pos);
                        //m_field.push_back(dd) ;
                        //m_normals.push_back( Vec3d(0,0,0) );
                    }

                    if(minv > d)
                        minv = d;
                    if(maxv < d);
                        maxv = d;
                }
            }
        }

        double colorScale = maxv-minv ;
        for(int i=previousstep;i<m_points.size();i++)
        {
            double value = m_field[i] ;
            double absvalue = fabs(value) ;
            if(absvalue < 0.001 )
                m_colors.push_back(Vec3d(1.0,0.0,0.0));
            else if(value < 0.0)
                m_colors.push_back(Vec3d(1.0*absvalue/colorScale,0.8*absvalue/colorScale,0.2));
        }
        if(m_sliceX>=numpoints){
            m_step+=1;
            m_sliceX=0;
        }
    }

#ifndef SOFA_NO_OPENGL
    auto& box = f_bbox.getValue();
    params->drawTool()->drawBoundingBox(box.minBBox(), box.maxBBox()) ;

    glPointSize(size[m_step]);
    glEnable(GL_COLOR);
    glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
    glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1) ;

    glBegin(GL_POINTS);
    for(unsigned int i=0;i<m_points.size();i++)
    {
        glColor3dv(m_colors[i].data() ) ;
        glNormal3dv(m_normals[i].data() );
        glVertex3dv( m_points[i].data() );
    }
    glEnd();
#endif //SOFA_NO_OPENGL
}


SOFA_DECL_CLASS(PointCloudImplicitFieldVisualization)

} /// namespace _scalarfield_

} /// namespace geometry

} /// namespace component

} /// namespace sofa
