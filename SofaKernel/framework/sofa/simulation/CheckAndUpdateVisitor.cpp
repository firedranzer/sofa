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
//
// C++ Implementation: CheckAndUpdateVisitor
//
// Description:
//
//
// Author: The SOFA team </www.sofa-framework.org>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <sofa/simulation/CheckAndUpdateVisitor.h>

namespace sofa
{

namespace simulation
{


CheckAndUpdateVisitor::CheckAndUpdateVisitor(const core::ExecParams* params, sofa::core::objectmodel::Event* e)
    : sofa::simulation::Visitor(params)
    , m_event(e)
{}


CheckAndUpdateVisitor::~CheckAndUpdateVisitor()
{}

Visitor::Result CheckAndUpdateVisitor::processNodeTopDown(simulation::Node* node)
{
    for_each(this, node, node->object, &CheckAndUpdateVisitor::processObject);

    if( m_event->isHandled() )
        return Visitor::RESULT_PRUNE;
    else
        return Visitor::RESULT_CONTINUE;
}

void CheckAndUpdateVisitor::processObject(simulation::Node*, core::objectmodel::BaseObject* obj)
{
    //if( obj-
    //    obj->handleEvent( m_event );
}


}

}

