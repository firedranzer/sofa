

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




class Side(object):

    def __init__(self,side, thickness, coef):

        if not side in ["left", "right"]:
            raise ValueError,  "choose a side"

        if not coef >= 0.0:
            raise ValueError, "coef has to be non negative"
        if not thickness > 0.0:
            raise ValueError, "thickness has to be positive"

        self.side = side
        self.coef = coef
        self.thickness = thickness
        self.type="side"

class SmoothSide(Side):

    def __init__(self,side, thickness, coef):
        Side.__init__(self,side, thickness, coef)
        self.type = "SmoothSide"



class CornerSide(Side):

    def __init__(self,side, thickness, coef, depht, width):

        Side.__init__(self,side, thickness, coef)
        self.type = "CornerSide"
        self.depht = depht
        self.width = width


class ControlPoint(object):

    def __init__(self, index, absisse, x, y, leftSide, rightSide):

        if not (isinstance(index, int) and absisse>=0 and absisse<=1.0):
            raise ValueError, "give good indices"
        self.index = index
        self.absisse = absisse
        self.x = x
        self.y = y

        if not (isinstance(leftSide, Side) and isinstance(rightSide, Side)):
            raise TypeError, "need Sides!"

        self.leftSide = leftSide
        self.rightSide = rightSide

    def key_index(self):
        return self.index

def key_absisse(controlPoint):
    return controlPoint.absisse



class Crochet(object):

    def __init__(self,listOfControlPoints):
        listOfControlPoints.sort(key = key_absisse)

        if not (listOfControlPoints[0].absisse == 0.0 and abs(listOfControlPoints[-1].absisse-1.0) <=0.005):
            print listOfControlPoints[-1].absisse
            raise ValueError, "bad endingPoint' absisses"

        for i in range(len(listOfControlPoints)):
            listOfControlPoints[i].index = i
        self.listOfControlPoints = listOfControlPoints

    def generate_LeftLine(self):

        newList=[]

        if isinstance(self.listOfControlPoints[0].leftSide, CornerSide):
            raise TypeError, "dont begin with a Corner"

        newList.append([self.listOfControlPoints[0].x+self.listOfControlPoints[0].leftSide.thickness, self.listOfControlPoints[0].y, 0.0,0.0])

        for i in range(len(self.listOfControlPoints)-2):

            vTangent = (self.listOfControlPoints[i+2].x - self.listOfControlPoints[i].x, self.listOfControlPoints[i+2].y - self.listOfControlPoints[i].y)
            norm = math.sqrt(vTangent[0]**2 + vTangent[1]**2)
            vOrtho = (-vTangent[1]/norm,vTangent[0]/norm)

            X = self.listOfControlPoints[i+1].x +self.listOfControlPoints[i+1].leftSide.thickness*vOrtho[0]
            Y = self.listOfControlPoints[i+1].y +self.listOfControlPoints[i+1].leftSide.thickness*vOrtho[1]

            if isinstance(self.listOfControlPoints[i+1].leftSide, SmoothSide):
                newList.append([X, Y, self.listOfControlPoints[i+1].leftSide.coef, self.listOfControlPoints[i+1].leftSide.coef])

            elif isinstance(self.listOfControlPoints[i+1].leftSide, CornerSide):
                x1 = X - (vTangent[0]/norm) * self.listOfControlPoints[i+1].leftSide.width/2.0
                y1 = Y - (vTangent[1]/norm) * self.listOfControlPoints[i+1].leftSide.width/2.0

                x2 = X + (vTangent[0]/norm) * self.listOfControlPoints[i+1].leftSide.width/2.0
                y2 = Y + (vTangent[1]/norm) * self.listOfControlPoints[i+1].leftSide.width/2.0

                x3 = X - vOrtho[0] * self.listOfControlPoints[i+1].leftSide.depht
                y3 = Y - vOrtho[1] * self.listOfControlPoints[i+1].leftSide.depht

                newList.append([x1, y1, self.listOfControlPoints[i+1].leftSide.coef*0.5, 0.0])
                newList.append([x3, y3, 0.0, 0.0])
                newList.append([x2, y2, 0.0, self.listOfControlPoints[i+1].leftSide.coef*0.5])

            else:
                raise TypeError, "give a right type of Side"

        if isinstance(self.listOfControlPoints[-1].leftSide, CornerSide):
            raise TypeError, "dont end with a Corner"

        newList.append([self.listOfControlPoints[-1].x, self.listOfControlPoints[-1].y+self.listOfControlPoints[0].leftSide.thickness, 0.0,0.0])

        return newList

    def generate_RightLine(self):

        newList=[]

        if isinstance(self.listOfControlPoints[0].rightSide, CornerSide):
            raise TypeError, "dont begin with a Corner"

        newList.append([self.listOfControlPoints[0].x - self.listOfControlPoints[0].rightSide.thickness, self.listOfControlPoints[0].y, 0.0,0.0])

        for i in range(len(self.listOfControlPoints)-2):

            vTangent = (self.listOfControlPoints[i+2].x - self.listOfControlPoints[i].x, self.listOfControlPoints[i+2].y - self.listOfControlPoints[i].y)
            norm = math.sqrt(vTangent[0]**2 + vTangent[1]**2)
            vOrtho = (vTangent[1]/norm, -vTangent[0]/norm)

            X = self.listOfControlPoints[i+1].x + self.listOfControlPoints[i+1].rightSide.thickness*vOrtho[0]
            Y = self.listOfControlPoints[i+1].y + self.listOfControlPoints[i+1].rightSide.thickness*vOrtho[1]

            if isinstance(self.listOfControlPoints[i+1].rightSide, SmoothSide):
                newList.append([X, Y, self.listOfControlPoints[i+1].rightSide.coef, self.listOfControlPoints[i+1].rightSide.coef])

            elif isinstance(self.listOfControlPoints[i+1].rightSide, CornerSide):
                x1 = X - (vTangent[0]/norm) * self.listOfControlPoints[i+1].rightSide.width/2
                y1 = Y - (vTangent[1]/norm) * self.listOfControlPoints[i+1].rightSide.width/2

                x2 = X + (vTangent[0]/norm) * self.listOfControlPoints[i+1].rightSide.width/2
                y2 = Y + (vTangent[1]/norm) * self.listOfControlPoints[i+1].rightSide.width/2

                x3 = X - vOrtho[0] * self.listOfControlPoints[i+1].rightSide.depht
                y3 = Y - vOrtho[1] * self.listOfControlPoints[i+1].rightSide.depht

                newList.append([x1, y1, self.listOfControlPoints[i+1].rightSide.coef*0.5, 0.0])
                newList.append([x3, y3, 0.0, 0.0])
                newList.append([x2, y2, 0.0, self.listOfControlPoints[i+1].rightSide.coef*0.5])

            else:
                raise TypeError, "give a right type of Side"

        if isinstance(self.listOfControlPoints[-1].rightSide, CornerSide):
            raise TypeError, "dont end with a Corner"

        newList.append([self.listOfControlPoints[-1].x, self.listOfControlPoints[-1].y - self.listOfControlPoints[0].rightSide.thickness, 0.0,0.0])

        return newList


    def getShape(self):

        leftLine = self.generate_LeftLine()

        print "LeftLine  "+str(leftLine)

        rightLine = self.generate_RightLine()

        print "rightLine  "+str(rightLine)

        rightLine.insert(0, leftLine[0])
        rightLine.append(leftLine[-1])




        ListTangentPointsLeft = piecewisePolynom2D.createListTangentPoints(leftLine)
        ListTangentPointsRight = piecewisePolynom2D.createListTangentPoints(rightLine)

        ListPolynomLeft = piecewisePolynom2D.createListPolynom(ListTangentPointsLeft, 1.0)
        ListPolynomRight = piecewisePolynom2D.createListPolynom(ListTangentPointsRight,-1.0)


        C1smoothPiecewisePolynomialChainLeft = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynomLeft)
        C1smoothPiecewisePolynomialChainRight = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynomRight)


        shape2D = primitives2D.Intersection(C1smoothPiecewisePolynomialChainLeft, C1smoothPiecewisePolynomialChainRight)
        shape=primitives.ExtrusionOfShape2D(shape2D, 0.0010,0.0,0.0, primitives.Point(0.0,0.0,0.0))

        return shape

def absisse(point, listOfDrawnPoints):

    """
    give the list of points as [x, y, [typeLeft, thicknessLeft, coefLeft (width and depht if corner),[typeRight, thicknessRight, coefRight (width and depht if corner)]
    give the points in the order they will appear

    """

    if not point in listOfDrawnPoints:

        raise ValueError, "I can't give the absisse if the point is not in the line"

    total_length = 0.0

    for i in range(len(listOfDrawnPoints)-1):
        total_length += math.sqrt((listOfDrawnPoints[i+1][0] - listOfDrawnPoints[i][0])**2 + (listOfDrawnPoints[i+1][1] - listOfDrawnPoints[i][1])**2)

    if listOfDrawnPoints.index(point)==0:
        return 0.0
    else:
        i = listOfDrawnPoints.index(point)
        return absisse(listOfDrawnPoints[i-1], listOfDrawnPoints) + math.sqrt((listOfDrawnPoints[i][0] - listOfDrawnPoints[i-1][0])**2 + (listOfDrawnPoints[i][1] - listOfDrawnPoints[i-1][1])**2)/total_length

def generateCrochetManually(listOfDrawnPoints):

    """
    give the list of points as [x, y, [typeLeft, thicknessLeft, coefLeft (width and depht if corner),[typeRight, thicknessRight, coefRight (width and depht if corner)]
    give the points in the order they will appear

    """

    listofControlPoints = []

    for point in listOfDrawnPoints:
        index = listOfDrawnPoints.index(point)
        ab = absisse(point, listOfDrawnPoints)
        x = point[0]
        y = point[1]

        left = point[2]
        right = point[3]

        if left[0] == "smooth":
            if right[0] == "smooth":

                listofControlPoints.append(ControlPoint(index, ab, x, y, SmoothSide("left", left[1], left[2]), SmoothSide("right", right[1], right[2])))

            elif right[0] == "corner":
                listofControlPoints.append(ControlPoint(index, ab, x, y, SmoothSide("left", left[1], left[2]), CornerSide("right", right[1], right[2], right[3], right[4])))

            else:
                raise TypeError, "check rightSide"

        elif left[0] == "corner":

            if right[0] == "smooth":

                listofControlPoints.append(ControlPoint(index, ab, x, y, CornerSide("left", left[1], left[2], left[3], left[4]), SmoothSide("right", right[1], right[2])))

            elif right[0] == "corner":
                listofControlPoints.append(ControlPoint(index, ab, x, y, CornerSide("left", left[1], left[2], left[3], left[4]), CornerSide("right", right[1], right[2], right[3], right[4])))


            else:
                raise TypeError, "check rightSide"

        else:

            raise TypeError, "check leftSide"



    return Crochet(listofControlPoints)

A = [0.0, 0.010, ["smooth", 0.0005, 0.0], ["smooth", 0.0005, 0.0]]
B = [0.0, 0.0075, ["smooth", 0.0005, 1.0], ["smooth", 0.0005, 1.0]]
C = [-0.0025, 0.005, ["smooth", 0.0005, 1.0], ["smooth", 0.0005, 1.0]]
D = [-0.0025, 0.0025, ["smooth", 0.0005, 1.0], ["smooth", 0.0005, 1.0]]
E = [0.0, 0.0, ["smooth", 0.0005, 1.0], ["smooth", 0.0005, 1.0]]
F = [0.005, 0.0025, ["smooth", 0.0005, 1.0], ["smooth", 0.0005, 1.0]]
G = [0.0075, 0.005, ["smooth", 0.0005, 1.0], ["smooth", 0.0005, 1.0]]

listOfDrawnPoints=[A, B, C, D, E, F, G]

crochet = generateCrochetManually(listOfDrawnPoints)

shape = crochet.getShape()

def evalField(x, y, z):
    
    point=primitives.Point(x,y,z)

    return shape.eval(point)





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


#shape2D = primitives2D.Intersection(C1smoothPiecewisePolynomialChain1, C1smoothPiecewisePolynomialChain2)
#shape=primitives.ExtrusionOfShape2D(shape2D, 0.010,0.0,0.0, primitives.Point(0.0,0.0,0.0))
