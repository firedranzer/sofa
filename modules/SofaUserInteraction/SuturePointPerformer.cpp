/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
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
#ifndef SOFA_COMPONENT_COLLISION_SUTUREPOINTPERFORMER_CPP
#define SOFA_COMPONENT_COLLISION_SUTUREPOINTPERFORMER_CPP

#include <SofaUserInteraction/SuturePointPerformer.inl>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/helper/Factory.inl>

namespace sofa
{

namespace component
{

namespace collision
{
#ifndef SOFA_DOUBLE
template class SOFA_USER_INTERACTION_API  SuturePointPerformer<defaulttype::Vec3fTypes>;
#endif
#ifndef SOFA_FLOAT
template class SOFA_USER_INTERACTION_API  SuturePointPerformer<defaulttype::Vec3dTypes>;
#endif

#ifndef SOFA_DOUBLE
helper::Creator<InteractionPerformer::InteractionPerformerFactory, SuturePointPerformer<defaulttype::Vec3fTypes> >  SuturePointPerformerVec3fClass("SuturePoints",true);
#endif
#ifndef SOFA_FLOAT
helper::Creator<InteractionPerformer::InteractionPerformerFactory, SuturePointPerformer<defaulttype::Vec3dTypes> >  SuturePointPerformerVec3dClass("SuturePoints",true);
#endif

}
}
}
#endif
