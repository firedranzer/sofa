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
from hjson import decoder

class MyObjectHook(object):
        def __call__(self, s):
                return s

def objectToString(object, nspace):
    instanceof = object.getData("psl_instanceof")
    if instanceof != None:
        instanceof = instanceof.getValue()
        if instanceof == "Template":
            source = object.psl_source
            ores = ""
            ores += nspace+instanceof+ " : {"
            ores += nspace+"    "+source
            ores += "}"+"\n"
            return ores

    res = ""
    for datafield in object.getListOfDataFields():
        if datafield.isPersistant():
            if datafield.hasParent():
                res += "\n" + nspace + '    ' + datafield.name + ' : "' + datafield.getParentPath() + '"'
            else:
                res += "\n" + nspace + "    " + datafield.name + " : \"" + datafield.getValueString() + "\""

    for link in object.getListOfLinks():
        if link.isPersistant():
            res += "\n" + nspace + "    " + link.name + " : \"" + link.getValueString() + "\""

    ores = ""
    ores += nspace+object.getClassName() + " : {"
    ores += res
    if res == "":
        ores += "}"+"\n"
    else:
        ores += "\n" + nspace+"} "+"\n"
    return ores


def treeToString(node, space):
        nspace=space+"    "
        res = ""
        instanceof = node.getData("psl_instanceof")
        if instanceof != None:
            res += space+str(node.psl_instanceof)+" : {"+ "\n"
            for k,v in eval(node.psl_properties):
                res += space+"    "+k+" : "+str(v)+ "\n"
            res += space+"}"+ "\n"
            return res

        res += space+"Node : {"

        ores = ""
        for datafield in node.getListOfDataFields():
            if datafield.isPersistant():
                if datafield.hasParent():
                    ores += "\n" + nspace + datafield.name + ' : "' + datafield.getParentPath() + '"'
                else:
                    ores += "\n" + nspace + datafield.name + " : \"" + datafield.getValueString() + "\""

        for link in node.getListOfLinks():
            if link.isPersistant():
                ores += "\n" + nspace + link.name + " : \"" + link.getValueString() + "\""

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
            res += "}\n"
        else:
            res += space+"}\n"
        return res

def toText(rootNode):
        return treeToString(rootNode, "")

def scanstring(s, end, encoding=None, strict=True):
    r=decoder.scanstring(s,end,encoding,strict)
    return (('s', str(r[0])), r[1])

def scantfnns(context, s, end):
    r=decoder.scantfnns(context, s,end)
    return (('p', str(r[0])), r[1])

def mlscanstring(s, end):
    r=decoder.mlscanstring(s,end)
    return (('m', str(r[0])), r[1])

class HJsonParser(hjson.HjsonDecoder):
    def __init__(self, encoding=None, object_hook=None, parse_float=None,
                parse_int=None, strict=True, object_pairs_hook=None):
        super(HJsonParser, self).__init__(encoding, object_hook, parse_float, parse_int, strict, object_pairs_hook)

        self.parse_tfnns = scantfnns
        self.parse_string = scanstring
        self.parse_mlstring = mlscanstring

        (self.scan_once, self.scan_object_once) = hjson.decoder.make_scanner(self)

    def parse_string(self, s, end):
        r = self.oldparse_string(s, end)
        return (('s', r[0]), r[1])

    def parse_tfnns(self, s, end):
        r = self.oldparse_tfnns(self, s, end)
        return (('p', r[0]), r[1])

def parse(hjsoncontent):
    '''Takes a string containing a scene using HJSON syntax and convert it into the AST structured used by PSL Engine.'''
    return HJsonParser(object_pairs_hook=MyObjectHook()).decode(hjsoncontent)

