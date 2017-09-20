#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - thomas.morzadec@inria.fr
##
####################################################################################################

import pyximport; pyximport.install()

import accordion
import primitives

(accordion1,accordion1Int)=accordion.accordionUniform(10.0,2.0,0.5,"ellipsoid",5,7.0,7.0,0.0)
point=primitives.Point(2.0,3.0,5.0)


print accordion1.eval(point)
