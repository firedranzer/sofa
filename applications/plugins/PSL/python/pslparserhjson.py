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
import os

def objectToString(object, nspace):
    res = ""
    res += nspace+object.getClassName() + " : { "+"\n"
    res += nspace+"    name : "+str(object.name)+"\n"

    for data in object.getDataFields():
        datafield = object.findData(data)
        if datafield.isPersistant():
            res += nspace + "    " + data + " : " + str(datafield.getValueString()) + "\n"

    res += nspace+" } "+"\n"
    return res


def treeToString(rootNode, space):
        res = space+"Node : {"+"\n"
        nspace=space+"    "

        for data in rootNode.getDataFields():
            datafield = rootNode.findData(data)
            if datafield.isPersistant():
                res += nspace + data + " : " + str(datafield.getValueString()) + "\n"

        for object in rootNode.getObjects():
            res += objectToString(object, space)

        for child in rootNode.getChildren():
            res += treeToString(child, nspace)

        res+=space+"}"+"\n"
        return res

def toText(rootNode):
        print("PSL DUMP")
        return treeToString(rootNode, "")

def parse(hjsoncontent):
    '''Takes a string containing a scene using HJSON syntax and convert it into the AST structured used by PSL Engine.'''
    return None
