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
import math




square=primitives2D.closedPolygonalChain("0",[(0.0,0.0),(0.0,1.0),(1.0,1.0),(1.0,0.0)])

print square.eval(primitives2D.Point2D(0.5,0.5))

cube=primitives.ExtrusionOfShape2D(square,1.0, 0.0,0.0,primitives.Point(0.0,0.0,0.0))



print ""+str(cube.eval(primitives.Point(0.5,0.5,0.5)))


print ""+str(cube.eval(primitives.Point(1.5,0.5,0.5)))


print ""+str(cube.eval(primitives.Point(1.0,0.5,0.5)))


print ""+str(cube.eval(primitives.Point(0.7,0.7,0.2)))




ellipse=primitives2D.Ellipse("0", 1.0,1.0, 0.0, primitives2D.Point2D(0.0,0.0))

print ellipse.eval(primitives2D.Point2D(0.5,0.0))

ellipsoid=primitives.ExtrusionOfShape2D(ellipse,1.0, 0.0,0.0,primitives.Point(0.0,0.0,0.0))

print ellipsoid.eval(primitives.Point(0.5,0.0,0.5))

print ""+str(ellipsoid.eval(primitives.Point(0.5,0.5,0.5)))


print ""+str(ellipsoid.eval(primitives.Point(1.5,0.5,0.5)))


print ""+str(ellipsoid.eval(primitives.Point(1.0,0.5,0.5)))


print ""+str(ellipsoid.eval(primitives.Point(0.7,0.7,0.2)))


cube.geometric_transformation("translation", vect=(1.0,1.0,1.0))

print cube.center.display()

cube.geometric_transformation("rotationAroundXYPlane", center=primitives.Point(0.0,0.0,0.0), phi=math.pi/2.0)

print cube.center.display()

cube.geometric_transformation("rotationAroundAxisZ", center=primitives.Point(0.0,0.0,0.0), theta=math.pi/2.0)

print cube.center.display()


print "ESSAI"

square=primitives2D.closedPolygonalChain("0",[(0.0,0.0),(0,1.0),(1.0,1.0),(1.0,0.0)])

#ellipse=primitives2D.Ellipse("0", 1.0,1.0, 0.0, primitives2D.Point2D(0.0,0.0))

cube=primitives.ExtrusionOfShape2D(square,1.0, 0.0,0.0,primitives.Point(0.0,0.0,0.0))

print cube.eval(primitives.Point(0.877648,-3.02927,0.000333671))

cube=primitives.ExtrusionOfShape2D(square,1.0, 0.0,0.0,primitives.Point(0.0,0.0,0.0))
cube.geometric_transformation("rotationAroundXYPlane", center=primitives.Point(1.0,0.0,0.0), phi=math.pi/3.0)
cube.geometric_transformation("rotationAroundAxisZ", center=primitives.Point(1.0,0.0,0.0), theta=math.pi/3.0)
cube.geometric_transformation("translation", vect=(0.5,0.5,0.5))
cube.geometric_transformation("rotationAroundXYPlane", center=primitives.Point(0.0,0.0,0.0), phi=math.pi/3.0)


print "PARALLEPIPED"
cube=primitives.Parallepiped("+",1.0,0.5,0.5,math.pi/4.0,0.0,primitives.Point(1.0,1.0,0.0))

print ""+str(cube.eval(primitives.Point(0.7,0.7,0.2)))

print "TWIST"

cylinder=primitives.Cylinder("+",0.5,0.5,1.0,0.0,0.0, primitives.Point(0.0,0.0,1.0))

triangle=primitives2D.closedPolygonalChain("0",[(-0.3,-0.2),(0.0,0.1),(0.3,-0.2)])
prism=primitives.ExtrusionOfShape2D(triangle,2.0,0.0,0.0,primitives.Point(0.0,-0.5,0.0))

shape=primitives.Difference(cylinder, prism)

#shape=primitives.Twist(shape,0.0,0.0,primitives.Point(0.0,0.0,0.0),3.0)

print shape.eval(primitives.Point(0.5,0.5,0.5))

shape=primitives.Union(primitives.Union(shape, cube), ellipsoid)

newShape=shape.duplicate()
print "DUPLICATE"
print shape.eval(primitives.Point(1.0,0.0,0.0))

print newShape.eval(primitives.Point(1.0,0.0,0.0))
