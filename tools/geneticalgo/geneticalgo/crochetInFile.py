#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - damien.marchal@inria.fr
##     - thomas.morzadec@inria.fr
##
####################################################################################################

import os
import primitives
import primitives2D
import piecewisePolynom2D
import math
import numpy as np
import random

def getDefaultHeader():
    return "import primitives\nimport primitives2D\nimport piecewisePolynom2D\nimport crochet\n\n\n"

def toPythonString(ind):
    temp=getDefaultHeader()
    temp+="listOfDrawnPoints = "+str(ind.listOfDrawnPoints)+"\n"
    temp+="crochet = crochet.generateCrochetManually(listOfDrawnPoints)\n"
    temp+="shape = crochet.getShape()"+"\n\n"

    return temp

