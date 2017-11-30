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
import crochet


A = [0.0, 0.010, ["smooth", 0.0007, 0.0], ["smooth", 0.0007, 0.0]]
B = [0.0, 0.005, ["smooth", 0.0007, 1.0], ["smooth", 0.0007, 1.0]]
C = [-0.0025, 0.00125, ["smooth", 0.0007, 1.0], ["smooth", 0.0007, 1.0]]
D = [-0.0025, -0.00125, ["smooth", 0.0007, 1.0], ["corner", 0.0015, 1.0, 0.0007, 0.0007]]
E = [0.0, -0.00250, ["smooth", 0.0007, 1.0], ["smooth", 0.0007, 1.0]]
F = [0.0025, -0.0025, ["smooth", 0.0007, 1.0], ["smooth", 0.0007, 1.0]]
G = [0.0035, -0.00125, ["smooth", 0.0007, 1.0], ["smooth", 0.0007, 1.0]]

listOfDrawnPoints=[A, B, C, D, E, F, G]

crochet = crochet.generateCrochetManually(listOfDrawnPoints)

shape = crochet.getShape()

def evalField(x, y, z):

    point=primitives.Point(x,y,z)

    return shape.eval(point)











#crochet = crochet.generateCrochetManually(listOfDrawnPoints)

#shape = crochet.getShape()



#A1 = [-5.2173913, 65.13043478, 0, 0]

#A2 = [-5.2173913,45.00868407, 1, 1]

#A3 = [-24.06731906,-8.3580024,1,1]

#A4 = [-28.99242501,-19.50648175,1, 0]

#A5 = [-19.99742301,-21.21495033,0, 0]

#A6 = [-28.99242501,-22.41931588,0, 1]


#A7 = [-16.07630834,-34.39267478,1, 1]

#A8 = [15.6629416, -43.4429904,1, 1]

#A9 = [21.16511222,-33.0330063,1, 1]


#B1 =[5.25955391, 65.13043478, 0,0]

#B2 = [5.25812001, 38.91081713,1, 1]

#B3 = [-12.83819983,-16.95571288,1, 1]

#B4 = [-12.51071676,-26.38135205,1, 1]

#B5 = [4.60835077,-33.66507377,1, 1]

#def scale(liste):
#    newList=[]
#    for i in range(len(liste)):
#        newList.append([liste[i][0]*0.001, liste[i][1]*0.001, liste[i][2], liste[i][3]])

#    return newList


#listofWeightedPoints1=scale([A1, A2, A3, A4, A5, A6, A7, A8, A9])
#listofWeightedPoints2 =scale([A1, B1, B2, B3, B4, B5, A9])
#print listofWeightedPoints1[0]

#ListTangentPoints1 = piecewisePolynom2D.createListTangentPoints(listofWeightedPoints1)
#ListTangentPoints2 = piecewisePolynom2D.createListTangentPoints(listofWeightedPoints2)

#ListPolynom1 = piecewisePolynom2D.createListPolynom(ListTangentPoints1, -1.0)
#ListPolynom2 = piecewisePolynom2D.createListPolynom(ListTangentPoints2,1.0)


#C1smoothPiecewisePolynomialChain1 = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynom1)
#C1smoothPiecewisePolynomialChain2 = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynom2)


#shape2D = primitives2D.Intersection(C1smoothPiecewisePolynomialChain1, C1smoothPiecewisePolynomialChain2)
#shape=primitives.ExtrusionOfShape2D(shape2D, 0.010,0.0,0.0, primitives.Point(0.0,0.0,0.0))


#def evalField(x, y, z):

#    point=primitives.Point(x,y,z)

#    return shape.eval(point)




##def evalField(x, y, z):
    
##    point=primitives.Point(x,y,z)

##    return shape.eval(point)
