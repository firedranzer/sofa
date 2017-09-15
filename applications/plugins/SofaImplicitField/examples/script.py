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


import accordion
import primitives

import math
from math import sqrt

niceaccordion=accordion.accordionUniform(1.0,0.2,0.01, "ellipsoid",5,0.6,0.6,0.5)


listheigthsJoiningPoints=[0.3,0.7,0.9]
listAxesX=[0.5,0.5,0.5,0.5]
listAxesY=[0.5,0.5,0.5,0.5]
Zrecovering=0.1
listcavities=[[0.25,"frisbee",0.5,0.5,0.2],[0.75,"frisbee",0.5,0.5,0.2],[0.5,"frisbee",0.5,0.5,0.2]]
#        niceaccordion=accordion.accordionFreeDimension(1.0,0.2,0.01, listcavities)
#        niceaccordion=accordion.accordionRecoveringGiven(1.0,0.2,0.01,"frisbee",listheigthsJoiningPoints, listAxesX, listAxesY,Zrecovering)
phrase=niceaccordion.toString()




x=1.0
y=3.0
z=5.0

frisbee=primitives.Frisbee("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(0.0,0.0,0.0))
estimFrisbee=abs(z)+sqrt(x*x+y*y)-1.0

print "estimFrisbee is "+str(estimFrisbee)

evalFrisbee=frisbee.eval(primitives.Point(1.0,3.0,5.0))
print "evalFrisbee is "+str(evalFrisbee)
print "They should be equal"
print "expression is "+ frisbee.toString()
print "\n"



ellipsoid1=primitives.Ellipsoid("+",1.0,2.0,7.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
ellipsoid2=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(1.0,0.0,0.0))
union2=primitives.Union(ellipsoid1,ellipsoid2)


estimEllipsoid=x*x+y*y+z*z-1.0

print "estimEllipsoid is "+str(estimEllipsoid)

evalEllipsoid=ellipsoid1.eval(primitives.Point(1.0,3.0,5.0))
print "evalEllipsoid is "+str(evalEllipsoid)
print "They should be equal"

print "expression is "+ellipsoid1.toString()
print "\n"

cylinder=primitives.Cylinder("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(0.0,0.0,0.0))
estimCylinder=max(abs(z)-1.0,x*x+y*y-1.0)

print "estimCylinder is "+str(estimCylinder)

evalCylinder=cylinder.eval(primitives.Point(1.0,3.0,5.0))
print "evalCylinder is "+str(evalCylinder)
print "They should be equal"

print "expression is"+ cylinder.toString()
print "\n"

union=primitives.Union(ellipsoid1,frisbee)
estimUnion=min(x*x+y*y+z*z-1.0,abs(z)+sqrt(x*x+y*y)-1.0)

print "estimUnion is "+str(estimUnion)

evalUnion=union.eval(primitives.Point(1.0,3.0,5.0))
print "evalUnion is "+str(evalUnion)
print "They should be equal"

print "expression is"+ union.toString()
print "\n"

intersection=primitives.Intersection(ellipsoid1,frisbee)
estimIntersection=max(x*x+y*y+z*z-1.0,abs(z)+sqrt(x*x+y*y)-1.0)

print "estimIntersection is "+str(estimIntersection)


evalIntersection=intersection.eval(primitives.Point(1.0,3.0,5.0))
print "evalIntersection is "+str(evalIntersection)
print "They should be equal"

print "expression is "+ intersection.toString()
print "\n"

difference=primitives.Difference(ellipsoid1,frisbee)
estimDifference=max(x*x+y*y+z*z-1.0,-(abs(z)+sqrt(x*x+y*y)-1.0))

print "estimDifference is "+str(estimDifference)

evalDifference=difference.eval(primitives.Point(1.0,3.0,5.0))
print "evalDifference is "+str(evalDifference)
print "They should be equal"

print "expression is "+ difference.toString()
print "\n"


acordion=accordion.accordionUniform(1.0,0.2,0.01, "ellipsoid",10,0.6,0.6,0.5)

evalAcordion=acordion.eval(primitives.Point(1.0,3.0,5.0))
estimAcordion=max(min(min(min(min(min(min(min(min(min(min(max(abs((z-0.5))-0.5,(x/0.2)*(x/0.2)+(y/0.2)*(y/0.2)-1.0),(x/0.6)*(x/0.6)+\
              (y/0.6)*(y/0.6)+((z-0.05)/0.075)*((z-0.05)/0.075)-1.0),(x/0.6)*(x/0.6)+(y/0.6)*(y/0.6)+((z-0.15)/0.075)*((z-0.15)/0.075)-1.0),\
              (x/0.6)*(x/0.6)+(y/0.6)*(y/0.6)+((z-0.25)/0.075)*((z-0.25)/0.075)-1.0),(x/0.6)*(x/0.6)+(y/0.6)*(y/0.6)+\
              ((z-0.35)/0.075)*((z-0.35)/0.075)-1.0),(x/0.6)*(x/0.6)+(y/0.6)*(y/0.6)+((z-0.45)/0.075)*((z-0.45)/0.075)-1.0),\
              (x/0.6)*(x/0.6)+(y/0.6)*(y/0.6)+((z-0.55)/0.075)*((z-0.55)/0.075)-1.0),(x/0.6)*(x/0.6)+(y/0.6)*(y/0.6)+((z-0.65)/0.075)*\
              ((z-0.65)/0.075)-1.0),(x/0.6)*(x/0.6)+(y/0.6)*(y/0.6)+((z-0.75)/0.075)*((z-0.75)/0.075)-1.0),(x/0.6)*(x/0.6)+(y/0.6)*(y/0.6)+\
              ((z-0.85)/0.075)*((z-0.85)/0.075)-1.0),(x/0.6)*(x/0.6)+(y/0.6)*(y/0.6)+((z-0.95)/0.075)*((z-0.95)/0.075)-1.0),\
              -(min(min(min(min(min(min(min(min(min(min(max(abs((z-0.5))-0.49,(x/0.19)*(x/0.19)+(y/0.19)*(y/0.19)-1.0),(x/0.59)*(x/0.59)+\
              (y/0.59)*(y/0.59)+((z-0.05)/0.065)*((z-0.05)/0.065)-1.0),(x/0.59)*(x/0.59)+(y/0.59)*(y/0.59)+((z-0.15)/0.065)*((z-0.15)/0.065)-1.0),\
              (x/0.59)*(x/0.59)+(y/0.59)*(y/0.59)+((z-0.25)/0.065)*((z-0.25)/0.065)-1.0),(x/0.59)*(x/0.59)+(y/0.59)*(y/0.59)+((z-0.35)/0.065)\
              *((z-0.35)/0.065)-1.0),(x/0.59)*(x/0.59)+(y/0.59)*(y/0.59)+((z-0.45)/0.065)*((z-0.45)/0.065)-1.0),(x/0.59)*(x/0.59)+\
              (y/0.59)*(y/0.59)+((z-0.55)/0.065)*((z-0.55)/0.065)-1.0),(x/0.59)*(x/0.59)+(y/0.59)*(y/0.59)+((z-0.65)/0.065)*((z-0.65)/0.065)-1.0)\
              ,(x/0.59)*(x/0.59)+(y/0.59)*(y/0.59)+((z-0.75)/0.065)*((z-0.75)/0.065)-1.0),(x/0.59)*(x/0.59)+(y/0.59)*(y/0.59)+((z-0.85)/0.065)\
              *((z-0.85)/0.065)-1.0),(x/0.59)*(x/0.59)+(y/0.59)*(y/0.59)+((z-0.95)/0.065)*((z-0.95)/0.065)-1.0)))

print "evalAcordion is " + str(evalAcordion)

print "estimAcordion is " + str(estimAcordion)

print "expression is \n"+ acordion.toString()
print "\n"

print union2.toString()

print primitives.ind()
print union2.gradX()
print union2.gradY()
print union2.gradZ()

print "\n\n\n\n\n\n"
print "implicit function is "+ellipsoid1.toString()+"\n\n"

print "gradX is "+ellipsoid1.gradX()+"\n\n"
print "gradY is "+ellipsoid1.gradY()+"\n\n"
print "gradZ is "+ellipsoid1.gradZ()+"\n\n"
