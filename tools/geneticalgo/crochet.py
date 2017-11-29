

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

def class Side(object)

def __init__(self,side, thickness, coef):

    if not side in ["left", "right"]:
        raise ValueError,  "choose a side"

    if not self.coef >= 0.0:
        raise ValueError, "coef has to be non negative"
    if not self.thickness > 0.0:
        raise ValueError, "thickness has to be positive"*

    self.side = side
    self.coef = coef
    self.thickness = thickness
    self.type

def class SmoothSide(Side):

    def __init__(self,side, thickness, coef):
        Side.__init__(self,side, thickness, coef)
        self.type = "SmoothSide"



def class CornerSide(Side):

    def __init__(self,side, thickness, coef, depht, width):

        Side.__init__(self,side, thickness, coef)
        self.depht = depht
        self.width = width


def class ControlPoint(object):

    def __init__(self, index, absisse, x, y, leftSide, rightSide):

        if not (isinstance(index, int) and absisse>=0 and absisse<=1.0):
            raise ValueError, "give good indices"
        self.index = index
        self.absisse
        self.x = x
        self.y = y

        if not (isinstance(leftSide, Side) and isinstance(rightSide, Side)):
            raise TypeError, "need Sides!"

        self.leftSide = leftSide
        self.rightSide = rightSide

    def key_index(self):
        return self.index

    def key_absisse(self):
        return self.absisse

def class Crochet(object):



    def __init__(listOfControlPoints):
        listOfControlPoints.sort(key = key_absisse)

        if not (listOfControlPoints[0].absisse = 0.0 and listOfControlPoints[0].absisse = 1.0):
            raise ValueError, "bad endinPoints' absisses"

        for i in range(len(listOfControlPoints)):
            listOfControlPoints[i].index = i
        self.listOfControlPoints = listOfControlPoints

    def generate_LeftLine(self):

        newList=[]

        if isistance(listOfControlPoints[0], CornerSide):
            raise TypeError, "dont begin with a Corner"

        newList.append([self.listOfControlPoints[0].x+self.listOfControlPoints[0].leftSide.thickness, self.listOfControlPoints[0].y, 0.0,0.0])

        for i in range(len(listOfControlPoints)):









A1 = [-5.2173913, 65.13043478, 0, 0]

A2 = [-5.2173913,45.00868407, 1, 1]

A3 = [-24.06731906,-8.3580024,1,1]

A4 = [-28.99242501,-19.50648175,1, 0]

A5 = [-19.99742301,-21.21495033,0, 0]

A6 = [-28.99242501,-22.41931588,0, 1]


A7 = [-16.07630834,-34.39267478,1, 1]

A8 = [15.6629416, -43.4429904,1, 1]

A9 = [21.16511222,-33.0330063,1, 1]


B1 =[5.25955391, 65.13043478, 0,0]

B2 = [5.25812001, 38.91081713,1, 1]

B3 = [-12.83819983,-16.95571288,1, 1]

B4 = [-12.51071676,-26.38135205,1, 1]

B5 = [4.60835077,-33.66507377,1, 1]

def scale(liste):
    newList=[]
    for i in range(len(liste)):
        newList.append([liste[i][0]*0.001, liste[i][1]*0.001, liste[i][2], liste[i][3]])

    return newList


listofWeightedPoints1=scale([A1, A2, A3, A4, A5, A6, A7, A8, A9])
listofWeightedPoints2 =scale([A1, B1, B2, B3, B4, B5, A9])
print listofWeightedPoints1[0]

ListTangentPoints1 = piecewisePolynom2D.createListTangentPoints(listofWeightedPoints1)
ListTangentPoints2 = piecewisePolynom2D.createListTangentPoints(listofWeightedPoints2)

ListPolynom1 = piecewisePolynom2D.createListPolynom(ListTangentPoints1, -1.0)
ListPolynom2 = piecewisePolynom2D.createListPolynom(ListTangentPoints2,1.0)


C1smoothPiecewisePolynomialChain1 = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynom1)
C1smoothPiecewisePolynomialChain2 = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynom2)


shape2D = primitives2D.Intersection(C1smoothPiecewisePolynomialChain1, C1smoothPiecewisePolynomialChain2)
shape=primitives.ExtrusionOfShape2D(shape2D, 0.010,0.0,0.0, primitives.Point(0.0,0.0,0.0))



def evalField(x, y, z):
    
    point=primitives.Point(x,y,z)

    return shape.eval(point)
