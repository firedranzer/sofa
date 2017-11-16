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


A1 = [0.0,5.0,random.uniform(0.0,0.5),random.uniform(0.0,0.5)]

A2 = [-1.0,2.5,random.uniform(0.0,0.5),random.uniform(0.0,0.5)]

A3 = [-1.0,1.0,random.uniform(0.0,0.5),random.uniform(0.0,0.5)]

A4 = [0.0,0.0,random.uniform(0.0,0.5),random.uniform(0.0,0.5)]

A5 = [1.0,1.0,random.uniform(0.0,0.5),random.uniform(0.0,0.5)]

B1 = [0.5,5.0,random.uniform(0.0,0.5),random.uniform(0.0,0.5)]

B2 = [-0.5,2.5,random.uniform(0.0,0.5),random.uniform(0.0,0.5)]

B3 = [-0.50,1.0,random.uniform(0.0,0.5),random.uniform(0.0,0.5)]

B4 = [0.0,0.5,random.uniform(0.0,0.5),random.uniform(0.0,0.5)]



listofWeightedPoints1=[A1, A2, A3, A4, A5, A1]

listofWeightedPoints2 =[A1, B1, B2,B3, B4, A5]


ListTangentPoints1 = piecewisePolynom2D.createListTangentPoints(listofWeightedPoints1)
ListTangentPoints2 = piecewisePolynom2D.createListTangentPoints(listofWeightedPoints2)




t1 = piecewisePolynom2D.Tangency2D(1.0,1.0)

X1 = piecewisePolynom2D.WeightedTangencedPoint2D(primitives2D.Point2D(1.0,1.0), t1, 1.0, 1.0)
X2 = piecewisePolynom2D.WeightedTangencedPoint2D(primitives2D.Point2D(2.0,2.0), t1, 1.0, 1.0)


P = piecewisePolynom2D.Polynom(X1, X2, 1.0)



ListPolynom1 = piecewisePolynom2D.createListPolynom(ListTangentPoints1, -1.0)
ListPolynom2 = piecewisePolynom2D.createListPolynom(ListTangentPoints2,1.0)


C1smoothPiecewisePolynomialChain1 = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynom1)
C1smoothPiecewisePolynomialChain2 = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynom2)


shape2D = primitives2D.Intersection(C1smoothPiecewisePolynomialChain1, C1smoothPiecewisePolynomialChain2)

shape=primitives.ExtrusionOfShape2D(shape2D, 1.0, 0.0, 0.0, primitives.Point(0.0,0.0,0.0))


print shape.eval(primitives.Point(-0.75,2.5,0.5))

def f(x,y):
    return shape2D.eval(primitives2D.Point2D(x, y))
(n,p)=(200,200)
x , y = np.meshgrid(np.linspace(-10,10,n),np.linspace(-5,5,p))


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
