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

    def __init__(self, index, absisse, x, y, leftSide, rightSide, ellipse = False, axisA = 0.0001 , axisB = 0.0001/2.0 , epsilon = 0.0):

        if not (isinstance(index, int) and absisse >= 0.0 and absisse <= 1.00001):
            print "ABSISSE="+str(absisse)
            raise ValueError, "give good indices"
        self.index = index
        self.absisse = absisse
        self.x = x
        self.y = y

        if not (isinstance(leftSide, Side) and isinstance(rightSide, Side)):
            raise TypeError, "need Sides!"

        self.leftSide = leftSide
        self.rightSide = rightSide
        self.ellipse = ellipse

        if ellipse:

            self.axisA = axisA
            self.axisB = axisB
            self.epsilon = epsilon

    def key_index(self):
        return self.index

def key_absisse(controlPoint):
    return controlPoint.absisse



class Crochet(object):

    def __init__(self,listOfControlPoints):
        listOfControlPoints.sort(key = key_absisse)

        if not (listOfControlPoints[0].absisse == 0.0 and abs(listOfControlPoints[-1].absisse-1.0) <=0.0005):

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

    def generate_Ellipses(self):

        list_ellipses = []

        if len(self.listOfControlPoints)>2:

            for i in range(1, len(self.listOfControlPoints)-2):

                if self.listOfControlPoints[i+1].ellipse == True:

                    vTangent = (self.listOfControlPoints[i+2].x - self.listOfControlPoints[i].x, self.listOfControlPoints[i+2].y - self.listOfControlPoints[i].y)
                    norm = math.sqrt(vTangent[0]**2 + vTangent[1]**2)

                    cosTheta = vTangent[0]/norm
                    sinTheta = vTangent[1]/norm

                    cos_theta = cosTheta*math.cos(self.listOfControlPoints[i+1].epsilon) - sinTheta*math.sin(self.listOfControlPoints[i+1].epsilon)
                    sin_theta = cosTheta*math.sin(self.listOfControlPoints[i+1].epsilon) + sinTheta*math.cos(self.listOfControlPoints[i+1].epsilon)

                    a = min(self.listOfControlPoints[i+1].axisA, min(self.listOfControlPoints[i+1].leftSide.thickness, self.listOfControlPoints[i+1].rightSide.thickness))
                    b = min(self.listOfControlPoints[i+1].axisB, min(self.listOfControlPoints[i+1].leftSide.thickness, self.listOfControlPoints[i+1].rightSide.thickness)/2.0)
                    center = primitives2D.Point2D(self.listOfControlPoints[i+1].x, self.listOfControlPoints[i+1].y)
                    list_ellipses.append((a,b,cosTheta, sinTheta, center))

        return list_ellipses

    def getShape(self):

        leftLine = self.generate_LeftLine()

        rightLine = self.generate_RightLine()
        rightLine.reverse()

        Line = leftLine + rightLine
        Line.append(leftLine[0])

        print str(Line[0])+ "\n"+ str(Line[-1])

        for i in range(len(Line)-1):
            if Line[i] == Line[i+1]:
                print str(Line[i])
                print str(i)
                print str(len(Line))




        ListTangentPoints = piecewisePolynom2D.createListTangentPoints(Line)
#        ListTangentPointsRight = piecewisePolynom2D.createListTangentPoints(rightLine)

        ListPolynom = piecewisePolynom2D.createListPolynom(ListTangentPoints, 1.0)
#        ListPolynomRight = piecewisePolynom2D.createListPolynom(ListTangentPointsRight,-1.0)


        C1smoothPiecewisePolynomialChain = piecewisePolynom2D.CLOSEDC1smoothPiecewisePolynomialChain(ListPolynom)
#        C1smoothPiecewisePolynomialChainRight = piecewisePolynom2D.C1smoothPiecewisePolynomialChain(ListPolynomRight)


        shape2D = C1smoothPiecewisePolynomialChain

#        for parameters in self.generate_Ellipses():
#            print str(*parameters)
#            shape2D = primitives2D.Difference(shape2D, primitives2D.Ellipse(*parameters))
#        primitives2D.Intersection(C1smoothPiecewisePolynomialChainLeft, C1smoothPiecewisePolynomialChainRight)
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

        ellipse = point[4]

        if not ellipse[0]:

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

        else:

            if left[0] == "smooth":
                if right[0] == "smooth":

                    listofControlPoints.append(ControlPoint(index, ab, x, y, SmoothSide("left", left[1], left[2]), SmoothSide("right", right[1], right[2]), ellipse[0], ellipse[1], ellipse[2], ellipse[3]))

                elif right[0] == "corner":
                    listofControlPoints.append(ControlPoint(index, ab, x, y, SmoothSide("left", left[1], left[2]), CornerSide("right", right[1], right[2], right[3], right[4]), ellipse[0], ellipse[1], ellipse[2], ellipse[3]))

                else:
                    raise TypeError, "check rightSide"

            elif left[0] == "corner":

                if right[0] == "smooth":

                    listofControlPoints.append(ControlPoint(index, ab, x, y, CornerSide("left", left[1], left[2], left[3], left[4]), SmoothSide("right", right[1], right[2]), ellipse[0], ellipse[1], ellipse[2], ellipse[3]))

                elif right[0] == "corner":
                    listofControlPoints.append(ControlPoint(index, ab, x, y, CornerSide("left", left[1], left[2], left[3], left[4]), CornerSide("right", right[1], right[2], right[3], right[4]), ellipse[0], ellipse[1], ellipse[2], ellipse[3]))


                else:
                    raise TypeError, "check rightSide"

            else:

                raise TypeError, "check leftSide"


    return Crochet(listofControlPoints)

