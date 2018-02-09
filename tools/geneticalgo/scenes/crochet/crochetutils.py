#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 CNRS, INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - damien.marchal@univ-lille1.fr
##     - thomas.morzadec@inria.fr
##
####################################################################################################

import sys, os
import geneticalgo.expression as expression
import geneticalgo.expressionToString as expressionToString
import geneticalgo.primitives as primitives
sys.path.append(os.path.dirname(os.path.realpath(__file__)) + "/../../")

#import primitives


def create(self):

    self.listOfDrawnPoints=[]


