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
    ,d_gridresolution(initData(&d_gridresolution, (unsigned int)128, "resolution", "The amount of samples per axis"))
    ,m_asyncthread(&PointCloudImplicitFieldVisualization::asyncCompute, this)
    ,m_box(initData(&m_box, sofa::defaulttype::BoundingBox(0,1,0,1,0,1), "box", "min - max coordinate of the grid where to sample the function. "))
    ,m_color(initData(&m_color, sofa::helper::types::RGBAColor::white(), "color", "..."))
{
}

void PointCloudImplicitFieldVisualization::computeBBox(const core::ExecParams *, bool t)
{
    f_bbox = m_box.getValue() ;
}


PointCloudImplicitFieldVisualization::~PointCloudImplicitFieldVisualization()
{
    {
        std::lock_guard<std::mutex> lk(m_cmdmutex);
        m_cmd = CMD_STOP ;
        m_cmdcond.notify_one();
    }
    std::cout << "Waiting job to terminated" << std::endl ;
    m_asyncthread.join() ;
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

    /// WE RESET THE COMPUTATION.

    std::lock_guard<std::mutex> lk(m_cmdmutex);
    m_cmd = CMD_START ;
    m_cmdcond.notify_one();
    std::cout << "NOTIFY ALL" << std::endl ;

    f_bbox = m_box.getValue() ;
}

void PointCloudImplicitFieldVisualization::reinit()
{
    init() ;
}

void PointCloudImplicitFieldVisualization::updateBufferFromComputeKernel()
{
    std::unique_lock<std::mutex> lock(m_datamutex, std::defer_lock) ;

    if( lock.try_lock() ){
        double colorScale = m_maxv - m_minv ;
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
}


double locateZero(double cv, Vec3d& cpos, Vec3d& cgrad, Vec3d& npos, Vec3d& outP, Vec3d& outG, ScalarField* f)
{
    double nv = f->getValue(npos) ;
    if( nv > cv ){
        outP = cpos ;
        outG = cgrad ;
        return cv ;
    }
    //std::cout << "ITERATE" << std::endl ;
    Vec3d ngrad = f->getGradient(npos) ;
    Vec3d nnpos = npos - (ngrad) ;
    return locateZero(nv, npos, ngrad, nnpos, outP, outG, f);
}

void PointCloudImplicitFieldVisualization::asyncCompute()
{
    while(true)
    {
        // Wait until main() sends data
        std::cout << "THREAD IS WAITING TO START" << std::endl ;
        {
            std::unique_lock<std::mutex> lk(m_cmdmutex);
            m_cmdcond.wait(lk, [this]{return m_cmd == CMD_START || m_cmd == CMD_STOP;});
            if(m_cmd==CMD_STOP)
                return ;
            m_cmd = CMD_PROCESS ;
        }
        std::cout << "THREAD STARTED.." << std::endl ;

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
                    if(m_cmd==CMD_STOP)
                        return;
                    if(m_cmd==CMD_START)
                        goto endl;

                    z = minbox.z()+scalebox.z()*k;
                    Vec3d pos { x, y, z }  ;

                    double dd = l_field.get()->getValue(pos) ;
                    double d = fabs(dd) ;
                    if( dd < 0.001 )
                    {

                        std::lock_guard<std::mutex> guard(m_datamutex) ;
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
        endl:
            std::cout << "THREAD DONE" << std::endl ;
    }
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

        /// WE RESET THE COMPUTATION.
        std::lock_guard<std::mutex> lk(m_cmdmutex);
        m_cmd = CMD_START ;
        m_cmdcond.notify_one();
    }


    updateBufferFromComputeKernel();

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
