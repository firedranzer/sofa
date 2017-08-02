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
from __future__ import with_statement
import contextlib

import hjson
import Sofa
import os
import pslparserxml
import pslparserhjson
import pslengine



@contextlib.contextmanager
def SetPath(newpath):
    curdir= os.getcwd()
    try: yield
    finally: os.chdir(curdir)

def save(rootNode, filename):
        filename = os.path.abspath(filename)
        dirname = os.path.dirname(filename)

        with SetPath(dirname):
            os.chdir(dirname)
            file = open(filename, "w")
            if filename.endswith(".psl") or filename.endswith(".pyson"):
                print(pslparserhjson.toText(rootNode))
            elif filename.endswith(".pslx"):
                print(pslparserxml.toText(rootNode))

def preProcess(ast):
    version = None

    ## Check in the ast for specific directives.
    for cmd, value in ast:
        if cmd == "PSLVersion":
            if version == None:
                if isinstance(value, str) or isinstance(value, unicode):
                    version = str(value)
                else:
                    raise Exception("PSLVersion must be a string in format '1.0'")
            else:
                raise Exception("There is two PSLVersion directive in the same file.")

    if version == None:
        version = "1.0"

    return {"version": version}

def load(filename):
        filename = os.path.abspath(filename)
        dirname = os.path.dirname(filename)
        with SetPath(dirname):
            os.chdir(dirname)
            f = open(filename).read()
            if filename.endswith(".psl") or filename.endswith(".pyson"):
                ast = pslparserhjson.parse(f)
            elif filename.endswith(".pslx"):
                ast = pslparserxml.parse(f)

            if len(ast) == 0:
                rootNode = Sofa.createNode("root")
                Sofa.msg_error(rootNode, "The file '"+filename+"' does not contains PSL(X) content")
                return rootNode

            directives = preProcess(ast[0][1])

            if not directives["version"] in ["1.0"]:
                rootNode = Sofa.createNode("root")
                Sofa.msg_error(rootNode, "Unsupported PSLVersion"+str(directives["version"]))
                return rootNode

            ret = pslengine.processTree("", ast, directives)
            print(pslparserhjson.toText(ret))
            return ret

        return None
