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

x=1.0
y=1.0


A1 = [-5.2173913, 65.13043478, 0, 0]

A2 = [-5.2173913,45.00868407, 1, 1]

A3 = [-24.06731906,-8.3580024,1,1]

A4 = [-28.99242501,-19.50648175,1, 0]

A5 = [-19.99742301,-21.21495033,0, 0]

A6 = [-28.99242501,-22.41931588,0, 1]


A7 = [-16.07630834,-34.39267478,1, 1]

A8 = [15.6629416, -43.4429904,1, 1]

A9 = [21.16511222,-33.0330063,1, 1]


B1 =[0.25955391, 65.13043478, 0,0]

B2 = [0.25812001, 38.91081713,1, 1]

B3 = [-12.83819983,-16.95571288,1, 1]

B4 = [-12.51071676,-26.38135205,1, 1]

B5 = [4.60835077,-33.66507377,1, 1]


listofWeightedPoints1=[A1, A2, A3, A4, A5, A6, A7, A8, A9]

listofWeightedPoints2 =[A1, B1, B2, B3, B4, B5, A9]


ListTangentPoints1 = piecewisePolynom2D.createListTangentPoints(listofWeightedPoints1)
ListTangentPoints2 = piecewisePolynom2D.createListTangentPoints(listofWeightedPoints2)




t1 = piecewisePolynom2D.Tangency2D(1.0,1.0)

X1 = piecewisePolynom2D.WeightedTangencedPoint2D(primitives2D.Point2D(1.0,1.0), t1, x, y)
X2 = piecewisePolynom2D.WeightedTangencedPoint2D(primitives2D.Point2D(2.0,2.0), t1, x, y)


P = piecewisePolynom2D.Polynom(X1, X2, 1.0)



ListPolynom1 = piecewisePolynom2D.createListPolynom(ListTangentPoints1, -1.0)
ListPolynom2 = piecewisePolynom2D.createListPolynom(ListTangentPoints2,1.0)


C1smoothPiecewisePolynomialChain1 = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynom1)
C1smoothPiecewisePolynomialChain2 = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynom2)


shape2D = primitives2D.Intersection(C1smoothPiecewisePolynomialChain1, C1smoothPiecewisePolynomialChain2)
#shape=primitives.ExtrusionOfShape2D(shape2D, x, y, 1.0, primitives.Point(0.0,0.0,0.0))

#shape2D = C1smoothPiecewisePolynomialChain2

#print shape.eval(primitives.Point(-0.75,2.5,0.5))

def f(x,y):
    return shape2D.eval(primitives2D.Point2D(x, y))

(n,p)=(500,500)
x , y = np.meshgrid(np.linspace(-50.0,50.0,n),np.linspace(-45.0,70.0,p))


z = np.zeros((n,p))

print z


for i in range(n):
    for j in range(p):
        z[i][j] = f(x[i][j], y[i][j])

print z
#graphe = np.contour(x,y,z,20)

import matplotlib.pyplot as plt
plt.figure()
levels = [0.0]
contour = plt.contour(x, y, z, levels,)
plt.clabel(contour, colors = 'k', fmt = '%2.1f', fontsize=12)

plt.show()
