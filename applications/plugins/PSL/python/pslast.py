#!/usr/bin/python
# -*- coding: utf-8 -*-
#/******************************************************************************
#*       SOFA, Simulation Open-Framework Architecture, development version     *
#*                (c) 2006-2015 INRIA, USTL, UJF, CNRS, MGH                    *
#*                                                                             *
#* This library is free software; you can redistribute it and/or modify it     *
#* under the terms of the GNU Lesser General Public License as published by    *
#* the Free Software Foundation; either version 2.1 of the License, or (at     *
#* your option) any later version.                                             *
#*                                                                             *
#* This library is distributed in the hope that it will be useful, but WITHOUT *
#* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
#* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
#* for more details.                                                           *
#*                                                                             *
#* You should have received a copy of the GNU Lesser General Public License    *
#* along with this library; if not, write to the Free Software Foundation,     *
#* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
#*******************************************************************************
#*                              SOFA :: Framework                              *
#*                                                                             *
#* Contact information: contact@sofa-framework.org                             *
#******************************************************************************/
#*******************************************************************************
#* Contributors:                                                               *
#*    - damien.marchal@univ-lille1.fr Copyright (C) CNRS                       *
#*                                                                             *
#******************************************************************************/
import Sofa

def objectToAst(object):
    childList = []

    ### Add a list of child to this Node.
    for datafield in object.getListOfDataFields():
        if datafield.isPersistant():
            if datafield.hasParent():
                childList.append((datafield.name, datafield.getParentPath()))
            else:
                childList.append((datafield.name, datafield.getValueString()))

    return (object.getName(), childList)


def nodeToAst(node):
    childList = []

    ### Add a list of child to this Node.
    for datafield in node.getListOfDataFields():
        if datafield.isPersistant():
            if datafield.hasParent():
                childList.append((datafield.name, datafield.getParentPath()))
            else:
                childList.append((datafield.name, datafield.getValueString()))

    for object in node.getObjects():
        o, param  = objectToAst(object)
        childList.append( (o, param)  )

    for child in node.getChildren():
        c,l = nodeToAst(child)
        childList.append( (c, l) )

    return ("Node", childList)

def sceneToAst(rootNode):
    return [nodeToAst(rootNode)]
