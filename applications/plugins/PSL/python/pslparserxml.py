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
import xml
import xml.etree.ElementTree as ET

def totext(rootNode):
        print("PSL XML SAVE: "+str(rootNode))

def dprint(p,spaces):
    for n,v in p:
        print(spaces+str(n)+" : "),
        if isinstance(v, str):
            print(v)
        else:
            print(spaces+"{")
            dprint(v, spaces+"    ")
            print(spaces+"}")

def toAst(xmlnode):
    childList = []
    for k in xmlnode.attrib:
        childList.append( (k, xmlnode.attrib[k] ) )

    for child in xmlnode:
        for j in toAst(child):
            childList.append( j )

    if not len(childList) == 0:
        return [(xmlnode.tag, childList)]
    return [(xmlnode.tag, xmlnode.text)]


def parse(xmlcontent):
    xmlroot = ET.fromstring(xmlcontent)
    try:
        dprint(toAst(xmlroot),"")
    except:
        print("STR: "+str(toAst(xmlroot)))
    return toAst(xmlroot)
