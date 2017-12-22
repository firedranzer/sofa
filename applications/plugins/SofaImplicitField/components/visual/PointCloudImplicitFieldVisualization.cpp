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
#include <sofa/config/build_option_opengl.h>

#include <sofa/core/ObjectFactory.h>
using sofa::core::RegisterObject ;

#include "PointCloudImplicitFieldVisualization.h"
using sofa::core::objectmodel::ComponentState ;

#include <sofa/core/visual/DrawToolGL.h>
using sofa::core::visual::DrawToolGL ;

#include <sofa/core/visual/VisualParams.h>
using sofa::core::visual::VisualParams ;

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
  ,d_gridresolution(initData(&d_gridresolution, (unsigned int)128, "resolution", "The amount of samples per axis"))
  ,m_box(initData(&m_box, sofa::defaulttype::BoundingBox(0,1,0,1,0,1), "box", "min - max coordinate of the grid where to sample the function. "))
  ,m_color(initData(&m_color, sofa::helper::types::RGBAColor::white(), "color", "..."))
{
}

void PointCloudImplicitFieldVisualization::computeBBox(const core::ExecParams *, bool t)
{
    SOFA_UNUSED(t) ;
    f_bbox = m_box.getValue() ;
}


PointCloudImplicitFieldVisualization::~PointCloudImplicitFieldVisualization()
{
}

void PointCloudImplicitFieldVisualization::init()
{
    m_componentstate = ComponentState::Invalid;
    if(l_field.getSize() == 0)
    {
        msg_error() << "Missing the 'field' attribute. The component is disabled until properly set." ;
        return ;
    }
    m_componentstate = ComponentState::Valid ;
    m_colors.clear();
    m_field.clear();
    m_points.clear();
    m_normals.clear();
    f_bbox.setValue(sofa::defaulttype::BoundingBox(0,1,0,1,0,1));
    f_bbox = m_box.getValue() ;

    /// If the connected component is implementing the state protocol.
    /// We start tracking the predecessor state.
    if( l_field.get()->findData("state") )
    {
        m_datatracker.trackData(*l_field.get()->findData("state"));
    }

    /// Compute the initial geometry of the object.s
    asyncCompute() ;
    updateBufferFromComputeKernel();
}

void PointCloudImplicitFieldVisualization::reinit()
{
    init() ;
}

void PointCloudImplicitFieldVisualization::updateBufferFromComputeKernel()
{
    for(unsigned int i=0;i<m_cpoints.size();i++)
    {
        m_points.push_back( m_cpoints[i] );
        m_normals.push_back( m_cnormals[i] );
        m_field.push_back (m_cfield[i] ) ;
        double value = m_cfield[i] ;
        double absvalue = fabs(value) ;
        if(absvalue < 0.001 ){
            const sofa::helper::types::RGBAColor& color = m_color.getValue() ;
            m_colors.push_back( Vec3d( color.r(), color.g(), color.b() ) ) ;
        }else if(value < 0.0){
            const sofa::helper::types::RGBAColor& color = m_color.getValue() ;
            m_colors.push_back( Vec3d( color.r(), color.g(), color.b() ) ) ;
        }
    }
    m_cpoints.clear();
    m_cnormals.clear();
    m_cfield.clear();
    m_minv = +1000;
    m_maxv = 0.0;
}


double locateZero(double cv, Vec3d& cpos, Vec3d& cgrad, Vec3d& npos, Vec3d& outP, Vec3d& outG, ScalarField* f)
{
    double nv = f->getValue(npos) ;
    if( nv > cv ){
        outP = cpos ;
        outG = cgrad ;
        return cv ;
    }

    Vec3d ngrad = f->getGradient(npos) ;
    Vec3d nnpos = npos - (ngrad) ;
    return locateZero(nv, npos, ngrad, nnpos, outP, outG, f);
}

void PointCloudImplicitFieldVisualization::asyncCompute()
{

    uint32_t rndval = 1;
    uint16_t ix,iy;
    double x=0,y=0,z=0;

    const Vec3d& minbox = m_box.getValue().minBBox() ;
    unsigned int res = d_gridresolution.getValue() ;
    Vec3d scalebox = (m_box.getValue().maxBBox() - minbox) / res ;
    do
    {
        iy =  rndval & 0x000FF;        /* Y = low 8 bits */
        ix = (rndval & 0x1FF00) >> 8;  /* X = High 9 bits */
        unsigned lsb = rndval & 1;    /* Get the output bit. */
        rndval >>= 1;                 /* Shift register */
        if (lsb) {                    /* If the output is 0, the xor can be skipped. */
            rndval ^= 0x00012000;
        }
        if( ix < res && iy < res ){
            x = minbox.x()+(scalebox.x()*ix) ;
            y = minbox.y()+(scalebox.y()*iy) ;

            z=0;
            for(unsigned int k=0;k<res;k++)
            {
                z = minbox.z()+scalebox.z()*k;
                Vec3d pos { x, y, z }  ;

                double dd = l_field.get()->getValue(pos) ;
                double d = fabs(dd) ;
                if( dd < 0.001 )
                {
                    m_cpoints.push_back(pos);
                    m_cfield.push_back(dd) ;
                    m_cnormals.push_back( l_field.get()->getGradient(pos).normalized() );
                }

                if(m_minv > d)
                    m_minv = d;
                if(m_maxv < d)
                    m_maxv = d;
            }
        }
    } while (rndval != 1);

}

void PointCloudImplicitFieldVisualization::draw(const core::visual::VisualParams *params)
{
    if( m_componentstate != ComponentState::Valid )
        return ;

    /// If the data have changed
    if(m_datatracker.isDirty())
    {
        m_points.clear();
        m_colors.clear();
        m_field.clear();
        m_normals.clear();
        m_datatracker.clean();
        asyncCompute() ;
        updateBufferFromComputeKernel();
    }


#ifndef SOFA_NO_OPENGL
    auto& box = f_bbox.getValue();
    params->drawTool()->drawBoundingBox(box.minBBox(), box.maxBBox()) ;

    glPointSize(3);
    glEnable(GL_COLOR_MATERIAL) ;
    glEnable(GL_LIGHTING) ;
    glEnable(GL_COLOR);
    glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
    glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1) ;

    glBegin(GL_POINTS);
    for(unsigned int i=0;i<m_points.size();i++)
    {
        glColor3dv(m_colors[i].data()) ;
        glNormal3dv(m_normals[i].data());
        glVertex3dv(m_points[i].data());
    }
    glEnd();
    glNormal3f(1.0,0.0,0.0);
    glDisable(GL_COLOR_MATERIAL) ;

#endif //SOFA_NO_OPENGL
}


SOFA_DECL_CLASS(PointCloudImplicitFieldVisualization)

} /// namespace _scalarfield_

} /// namespace geometry

} /// namespace component

} /// namespace sofa
