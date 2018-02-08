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

x = 1.0

class Side(object):

    def __init__(self, side, width_hole, thickness):

        if not side in ["left", "right"]:
            raise ValueError,  "choose a side"

        if not width_hole >= 0.0:
            raise ValueError, "width_hole has to be non negative"

        if not thickness > 0.0:
            raise ValueError, "thickness has to be positive"

        self.side = side
        self.width_hole = width_hole
        self.thickness = thickness
        self.type="side"

class ControlPoint(object):

    def __init__(self, index, absisse, x, y, leftSide, rightSide):

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



    def key_index(self):
        return self.index

def key_absisse(controlPoint):
    return controlPoint.absisse


def node_equal(P1, P2):
    if P1[0:2] == P2[0:2]:
        return True
    else:
        return False

class SOFIA(object):

    def __init__(self,listOfControlPoints, thickness):
        listOfControlPoints.sort(key = key_absisse)

        if not (listOfControlPoints[0].absisse == 0.0 and abs(listOfControlPoints[-1].absisse-1.0) <=0.0005):

            raise ValueError, "bad endingPoint' absisses"

        for i in range(len(listOfControlPoints)):
            listOfControlPoints[i].index = i
        self.listOfControlPoints = listOfControlPoints
        self.thickness = thickness

    def generate_LeftLine(self):

        newList=[]

        newList.append([self.listOfControlPoints[0].x + (self.listOfControlPoints[0].leftSide.thickness + self.listOfControlPoints[0].leftSide.width_hole),\
                        self.listOfControlPoints[0].y, 0.0, x])

        for i in range(len(self.listOfControlPoints)-2):

            vTangent1 = (self.listOfControlPoints[i+2].x - self.listOfControlPoints[i+1].x,\
                         self.listOfControlPoints[i+2].y - self.listOfControlPoints[i+1].y)
            norm1 = math.sqrt(vTangent1[0]**2 + vTangent1[1]**2)

            if not norm1 > 0.0:
                raise ValueError, "bad angle"

            vTangent2 = (self.listOfControlPoints[i+1].x - self.listOfControlPoints[i].x, self.listOfControlPoints[i+1].y - self.listOfControlPoints[i].y)
            norm2 = math.sqrt(vTangent2[0]**2 + vTangent2[1]**2)

            if not norm2 > 0.0:
                raise ValueError, "bad angle"

            vOrtho_temp = ((vTangent1[0]/norm1 + vTangent2[0]/norm2),(vTangent1[1]/norm1 + vTangent2[1]/norm2))

            norm = math.sqrt(vOrtho_temp[0]**2 + vOrtho_temp[1]**2)

            vOrtho = (-vOrtho_temp[1]/norm, vOrtho_temp[0]/norm)
            print str(vOrtho)

            X = self.listOfControlPoints[i+1].x + (self.listOfControlPoints[i+1].leftSide.thickness + self.listOfControlPoints[i+1].leftSide.width_hole) * vOrtho[0]
            Y = self.listOfControlPoints[i+1].y + (self.listOfControlPoints[i+1].leftSide.thickness + self.listOfControlPoints[i+1].leftSide.width_hole) * vOrtho[1]

            newList.append([X, Y, x, x])

        newList.append([self.listOfControlPoints[-1].x + (self.listOfControlPoints[-1].leftSide.thickness + self.listOfControlPoints[-1].leftSide.width_hole), self.listOfControlPoints[-1].y, x,0.0])

        return newList

    def generate_RightLine(self):

        newList=[]

        newList.append([self.listOfControlPoints[0].x - (self.listOfControlPoints[0].rightSide.width_hole + \
                                                         self.listOfControlPoints[0].rightSide.thickness), self.listOfControlPoints[0].y, x, 0.0])

        for i in range(len(self.listOfControlPoints)-2):

            vTangent1 = (self.listOfControlPoints[i+2].x - self.listOfControlPoints[i+1].x, self.listOfControlPoints[i+2].y - self.listOfControlPoints[i+1].y)
            norm1 = math.sqrt(vTangent1[0]**2 + vTangent1[1]**2)

            if not norm1 > 0.0:
                raise ValueError, "bad angle"

            vTangent2 = (self.listOfControlPoints[i+1].x - self.listOfControlPoints[i].x, self.listOfControlPoints[i+1].y - self.listOfControlPoints[i].y)
            norm2 = math.sqrt(vTangent2[0]**2 + vTangent2[1]**2)

            if not norm2 > 0.0:
                raise ValueError, "bad angle"

            vOrtho_temp = ((vTangent1[0]/norm1 + vTangent2[0]/norm2),(vTangent1[1]/norm1 + vTangent2[1]/norm2))

            norm = math.sqrt(vOrtho_temp[0]**2 + vOrtho_temp[1]**2)

            vOrtho = (vOrtho_temp[1]/norm, -vOrtho_temp[0]/norm)
            print str(vOrtho)

            X = self.listOfControlPoints[i+1].x + (self.listOfControlPoints[i+1].rightSide.width_hole + self.listOfControlPoints[i+1].rightSide.thickness) * vOrtho[0]
            Y = self.listOfControlPoints[i+1].y + (self.listOfControlPoints[i+1].rightSide.width_hole + self.listOfControlPoints[i+1].rightSide.thickness) * vOrtho[1]

            newList.append([X, Y, x, x])

        newList.append([self.listOfControlPoints[-1].x - (self.listOfControlPoints[-1].rightSide.width_hole +\
                                                          self.listOfControlPoints[-1].rightSide.thickness), self.listOfControlPoints[-1].y, 0.0, x])

        return newList

    def hole_LeftLine(self):

        newList=[]
        n = len(self.listOfControlPoints)

        if n < 3:

            raise ValueError, "I need at least 3 points"

        if self.listOfControlPoints[0].leftSide.width_hole > 0.0 or self.listOfControlPoints[0].rightSide.width_hole > 0.0 or\
           self.listOfControlPoints[-1].leftSide.width_hole> 0.0 or self.listOfControlPoints[-1].rightSide.width_hole > 0.0:
               raise ValueError, "you shouldn't open at the top or bottom"


        k = 0

        while k < n-1:

            i = k

            while i < n and self.listOfControlPoints[i].leftSide.width_hole == 0.0 and self.listOfControlPoints[i].rightSide.width_hole == 0.0:
                    i+=1
                    print(i)
            if i == 0 or i == n-1:
                raise ValueError, "doesn't start hole at the top nor the bottom"
            if i == n:
                return newList

            if self.listOfControlPoints[i+1].leftSide.width_hole == 0.0 and self.listOfControlPoints[i+1].rightSide.width_hole == 0.0:

                vTangent1 = (self.listOfControlPoints[i+1].x - self.listOfControlPoints[i].x, self.listOfControlPoints[i+1].y - self.listOfControlPoints[i].y)
                norm1 = math.sqrt(vTangent1[0]**2 + vTangent1[1]**2)

                if not norm1 > 0.0:
                    raise ValueError, "bad angle"

                vTangent2 = (self.listOfControlPoints[i].x - self.listOfControlPoints[i-1].x, self.listOfControlPoints[i].y - self.listOfControlPoints[i-1].y)
                norm2 = math.sqrt(vTangent2[0]**2 + vTangent2[1]**2)

                if not norm2 > 0.0:
                    raise ValueError, "bad angle"

                vOrtho_temp = ((vTangent1[0]/norm1 + vTangent2[0]/norm2),(vTangent1[1]/norm1 + vTangent2[1]/norm2))

                norm = math.sqrt(vOrtho_temp[0]**2 + vOrtho_temp[1]**2)

                Ortho = (-vOrtho_temp[1]/norm, vOrtho_temp[0]/norm)

                print str(vOrtho)

                X = self.listOfControlPoints[i].x + self.listOfControlPoints[i].leftSide.width_hole * vOrtho[0]
                Y = self.listOfControlPoints[i].y + self.listOfControlPoints[i].leftSide.width_hole * vOrtho[1]

                list_temp = [[self.listOfControlPoints[i-1].x, self.listOfControlPoints[i-1].y, x, x], [X, Y, x, x], [self.listOfControlPoints[i+1].x, self.listOfControlPoints[i+1].y, x, x]]

                newList.append(list_temp)

                i+=1

                k = i


            else:

                list_temp = []

                while i < n-1 and (self.listOfControlPoints[i].leftSide.width_hole > 0.0 or self.listOfControlPoints[i].rightSide.width_hole > 0.0):

                    vTangent1 = (self.listOfControlPoints[i+1].x - self.listOfControlPoints[i].x, self.listOfControlPoints[i+1].y - self.listOfControlPoints[i].y)
                    norm1 = math.sqrt(vTangent1[0]**2 + vTangent1[1]**2)

                    if not norm1 > 0.0:
                        raise ValueError, "bad angle"

                    vTangent2 = (self.listOfControlPoints[i].x - self.listOfControlPoints[i-1].x, self.listOfControlPoints[i].y - self.listOfControlPoints[i-1].y)
                    norm2 = math.sqrt(vTangent2[0]**2 + vTangent2[1]**2)

                    if not norm2 > 0.0:
                        raise ValueError, "bad angle"

                    vOrtho_temp = ((vTangent1[0]/norm1 + vTangent2[0]/norm2),(vTangent1[1]/norm1 + vTangent2[1]/norm2))

                    norm = math.sqrt(vOrtho_temp[0]**2 + vOrtho_temp[1]**2)

                    vOrtho = (-vOrtho_temp[0]/norm, vOrtho_temp[1]/norm)

                    print str(vOrtho)
                    X = self.listOfControlPoints[i].x + self.listOfControlPoints[i].leftSide.width_hole * vOrtho[0]
                    Y = self.listOfControlPoints[i].y + self.listOfControlPoints[i].leftSide.width_hole * vOrtho[1]

                    list_temp .append([X, Y, x, x])
                    i+=1

                k=i

        return newList


    def hole_RighttLine(self):

        newList=[]
        n = len(self.listOfControlPoints)

        if n < 3:

            raise ValueError, "I need at least 3 points"

        if self.listOfControlPoints[0].leftSide.width_hole > 0.0 or self.listOfControlPoints[0].rightSide.width_hole > 0.0 or\
           self.listOfControlPoints[-1].leftSide.width_hole> 0.0 or self.listOfControlPoints[-1].rightSide.width_hole > 0.0:
               raise ValueError, "you shouldn't open at the top or bottom"


        k = 0

        while k < n-1:

            i = k

            while i < n and self.listOfControlPoints[i].leftSide.width_hole == 0.0 and self.listOfControlPoints[i].rightSide.width_hole == 0.0:
                    i+=1
            if i == 0 or i == n-1:
                raise ValueError, "doesn't start hole at the top nor the bottom"
            if i == n:
                return newList

            if self.listOfControlPoints[i+1].leftSide.width_hole == 0.0 and self.listOfControlPoints[i+1].rightSide.width_hole == 0.0:

                vTangent1 = (self.listOfControlPoints[i+1].x - self.listOfControlPoints[i].x, self.listOfControlPoints[i+1].y - self.listOfControlPoints[i].y)
                norm1 = math.sqrt(vTangent1[0]**2 + vTangent1[1]**2)

                if not norm1 > 0.0:
                    raise ValueError, "bad angle"

                vTangent2 = (self.listOfControlPoints[i].x - self.listOfControlPoints[i-1].x, self.listOfControlPoints[i].y - self.listOfControlPoints[i-1].y)
                norm2 = math.sqrt(vTangent2[0]**2 + vTangent2[1]**2)

                if not norm2 > 0.0:
                    raise ValueError, "bad angle"

                vOrtho_temp = ((vTangent1[0]/norm1 + vTangent2[0]/norm2),(vTangent1[1]/norm1 + vTangent2[1]/norm2))

                norm = math.sqrt(vOrtho_temp[0]**2 + vOrtho_temp[1]**2)

                vOrtho = (vOrtho_temp[1]/norm, vOrtho_temp[0]/norm)

                print str(vOrtho)
                X = self.listOfControlPoints[i].x + self.listOfControlPoints[i].leftSide.width_hole * vOrtho[0]
                Y = self.listOfControlPoints[i].y + self.listOfControlPoints[i].leftSide.width_hole * vOrtho[1]

                list_temp = [[self.listOfControlPoints[i-1].x, self.listOfControlPoints[i-1].y, x, x], [X, Y, x, x], [self.listOfControlPoints[i+1].x, self.listOfControlPoints[i+1].y, x, x]]

                newList.append(list_temp)
                i+=1
                k = i


            else:

                list_temp = []

                while i < n-1 and (self.listOfControlPoints[i].leftSide.width_hole > 0.0 or self.listOfControlPoints[i].rightSide.width_hole > 0.0):
                    print(i)

                    vTangent1 = (self.listOfControlPoints[i+1].x - self.listOfControlPoints[i].x, self.listOfControlPoints[i+1].y - self.listOfControlPoints[i].y)
                    norm1 = math.sqrt(vTangent1[0]**2 + vTangent1[1]**2)

                    if not norm1 > 0.0:
                        raise ValueError, "bad angle"

                    vTangent2 = (self.listOfControlPoints[i].x - self.listOfControlPoints[i-1].x, self.listOfControlPoints[i].y - self.listOfControlPoints[i-1].y)
                    norm2 = math.sqrt(vTangent2[0]**2 + vTangent2[1]**2)

                    if not norm2 > 0.0:
                        raise ValueError, "bad angle"

                    vOrtho_temp = ((vTangent1[0]/norm1 + vTangent2[0]/norm2),(vTangent1[1]/norm1 + vTangent2[1]/norm2))

                    norm = math.sqrt(vOrtho_temp[0]**2 + vOrtho_temp[1]**2)

                    vOrtho = (-vOrtho_temp[1]/norm, vOrtho_temp[0]/norm)
                    print str(vOrtho)

                    X = self.listOfControlPoints[i].x + self.listOfControlPoints[i].leftSide.width_hole * vOrtho[0]
                    Y = self.listOfControlPoints[i].y + self.listOfControlPoints[i].leftSide.width_hole * vOrtho[1]

                    list_temp .append([X, Y, x, x])
                    i+=1

                k=i
        print len(newList)
        return newList



    def getShape(self):

        leftLine = self.generate_LeftLine()

        print leftLine

        rightLine = self.generate_RightLine()

        print rightLine
        rightLine.reverse()

        Line = leftLine + rightLine

        newLine = [Line[0]]


        for i in range(len(Line)):
            if not node_equal(newLine[-1], Line[i]):
                newLine.append(Line[i])

        Line = newLine

        Line.append(leftLine[0])

        print str(Line)




        ListTangentPoints = piecewisePolynom2D.createListTangentPoints(Line)

        ListPolynom = piecewisePolynom2D.createListPolynom(ListTangentPoints, -1.0)

        C1smoothPiecewisePolynomialChain = piecewisePolynom2D.CLOSEDC1smoothPiecewisePolynomialChain(ListPolynom)

        shape2D = C1smoothPiecewisePolynomialChain

        hole_Left = self.hole_LeftLine()
        hole_Right = self.hole_RighttLine()

        if len(hole_Left) != len(hole_Right):
            raise ValueError, "Something wrong with holes"

        n = len(hole_Left)

        print n

        for i in range(n):

            newLeftHole = copy.deepcopy(hole_Left[i])
            newRightHole = copy.deepcopy(hole_Right[i])
            newRightHole.reverse()

            LocalHole = newLeftHole + newRightHole


            newHole = [LocalHole [0]]


            for i in range(len(LocalHole)):
                if not node_equal(newHole[-1], LocalHole[i]):
                    newLine.append(LocalHole[i])

            LocalHole = newHole

            LocalHole.append(newLeftHole[0])


            ListTangentPoints_hole = piecewisePolynom2D.createListTangentPoints(LocalHole)

            ListPolynom_hole = piecewisePolynom2D.createListPolynom(ListTangentPoints_hole, -1.0)

            C1smoothPiecewisePolynomialChain_hole = piecewisePolynom2D.CLOSEDC1smoothPiecewisePolynomialChain(ListPolynom_hole)

            shape2D = primtives2D.Difference(shape2D, C1smoothPiecewisePolynomialChain_hole)


        shape=primitives.ExtrusionOfShape2D(shape2D, self.thickness,0.0,0.0, primitives.Point(0.0,0.0,0.0))

        return shape

def absisse(point, listOfDrawnPoints):

    """
    give the list of points as [x, y, [width_hole_Left, thicknessLeft, [width_hole_Right, thicknessRight]
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

def generateSOFIAManually(listOfDrawnPoints, thickness):

    """
    give the list of points as [x, y, [width_hole_Left, thicknessLeft], [width_hole_Right, thicknessRight]]
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

        listofControlPoints.append(ControlPoint(index, ab, x, y, Side("left", left[0], left[1]), Side("right", right[0], right[1])))



    return SOFIA(listofControlPoints, thickness)


if True:

    ################
    ###
    ###EXAMPLE
    ###
    ################


    listOfDrawnPoints = [[0.0, 0.1, [0.0, 0.05],[0.0, 0.05]],\
                         [0.0, 0.075, [0.001, 0.05],[0.001, 0.05]],\
                         [0.00, 0.05, [0.002, 0.075], [0.002, 0.075]],\
                         [0.00, 0.025, [0.0015, 0.075], [0.0015, 0.075]],\
                         [0.0, 0.0, [0.0, 0.025], [0.0, 0.025]]]

    sofia = generateSOFIAManually(listOfDrawnPoints, 0.05)

    shape = sofia.getShape()

    import numpy as np
    import random
    import matplotlib.pyplot as plt
    (n,p)=(100,100)
    x , y = np.meshgrid(np.linspace(-0.15, 0.15,n),np.linspace(-0.01,0.12,p))
    z = np.zeros((n,p))
    for i in range(n):
        for j in range(p):
            z[i][j] = shape.eval(primitives.Point(x[i][j], y[i][j], 0.025))
    plt.figure()
    levels = [-0.1, 0.0]
    contour = plt.contour(x, y, z, levels,)
    plt.clabel(contour, colors = 'k', fmt = '%2.1f', fontsize=12)
    plt.show()
