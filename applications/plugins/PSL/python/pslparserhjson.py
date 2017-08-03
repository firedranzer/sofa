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
import hjson

class MyObjectHook(object):
        def __call__(self, s):
                return s

def objectToString(object, nspace):
    res = ""

    for datafield in object.getListOfDataFields():
        if datafield.isPersistant():
            if datafield.hasParent():
                res += "\n" + nspace + '    ' + datafield.name + ' : "' + datafield.getParentPath() + '"'
            else:
                res += "\n" + nspace + "    " + datafield.name + " : \"" + datafield.getValueString() + "\""

    ores = ""
    ores += nspace+object.getClassName() + " : { "
    ores += res
    if res == "":
        ores += "}"+"\n"
    else:
        ores += "\n" + nspace+"} "+"\n"
    return ores


def treeToString(node, space):
        nspace=space+"    "
        res = ""
        res += space+"Node : { "

        ores = ""
        for datafield in node.getListOfDataFields():
            if datafield.isPersistant():
                if datafield.hasParent():
                    ores += "\n" + nspace + datafield.name + ' : "' + datafield.getParentPath() + '"'
                else:
                    ores += "\n" + nspace + datafield.name + " : \"" + datafield.getValueString() + "\""

        if ores != "":
            ores += "\n"

        dres = ""
        for object in node.getObjects():
            dres += objectToString(object, space+"    ")

        cres = ""
        for child in node.getChildren():
            cres += treeToString(child, space+"    ")
        ores = ores + dres + cres
        res += ores
        if ores == "":
            res += "}"+"\n"
        else:
            res += space+"} "+"\n"
        return res

def toText(rootNode):
        return treeToString(rootNode, "")

def parse(hjsoncontent):
    '''Takes a string containing a scene using HJSON syntax and convert it into the AST structured used by PSL Engine.'''
    return hjson.loads(hjsoncontent, object_pairs_hook=MyObjectHook())

