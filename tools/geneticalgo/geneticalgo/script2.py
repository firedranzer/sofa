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


import primitives2D

X = primitives2D.Point2D(1.0,2.0)
Y = primitives2D.Point2D(2.0, 3.0)

vect = primitives2D.Vector2D(X, Y)


plane = primitives2D.HalfPlaneGivenByAVector2D(vect)

point =  primitives2D.Point2D(1.0, 2.50)

print "value= "+str(plane.evalPython(point))
