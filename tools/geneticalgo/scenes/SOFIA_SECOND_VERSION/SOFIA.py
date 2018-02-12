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
import copy

x = 1.0

class Side(object):

    def __init__(self, side, thickness):

        if not side in ["left", "right"]:
            raise ValueError,  "choose a side"

        if not thickness >= 0.0:
            raise ValueError, "thickness has to be positive"

        self.side = side
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

class SOFIA2D(object):

    def __init__(self,listOfControlPoints):
        listOfControlPoints.sort(key = key_absisse)

        if not (listOfControlPoints[0].absisse == 0.0 and abs(listOfControlPoints[-1].absisse-1.0) <=0.0005):

            raise ValueError, "bad endingPoint' absisses"

        for i in range(len(listOfControlPoints)):
            listOfControlPoints[i].index = i
        self.listOfControlPoints = listOfControlPoints

    def generate_LeftLine(self, round = False):

        newList=[]


        if round:
            newList.append([self.listOfControlPoints[0].x + (self.listOfControlPoints[0].leftSide.thickness),\
                            self.listOfControlPoints[0].y, x, x])
        else:
            newList.append([self.listOfControlPoints[0].x + (self.listOfControlPoints[0].leftSide.thickness),\
                            self.listOfControlPoints[0].y, 0.0, x])

        for i in range(1,len(self.listOfControlPoints)-1):

            X = self.listOfControlPoints[i].x + (self.listOfControlPoints[i].leftSide.thickness)
            Y = self.listOfControlPoints[i].y
            newList.append([X, Y, x, x])

        if round:
            newList.append([self.listOfControlPoints[-1].x + (self.listOfControlPoints[-1].leftSide.thickness), self.listOfControlPoints[-1].y, x,x])

        else:
            newList.append([self.listOfControlPoints[-1].x + (self.listOfControlPoints[-1].leftSide.thickness), self.listOfControlPoints[-1].y, x,0.0])

        return newList

    def generate_RightLine(self, round = False):

        newList=[]

        if round:

            newList.append([self.listOfControlPoints[0].x - self.listOfControlPoints[0].rightSide.thickness, self.listOfControlPoints[0].y, x, x])

        else:
            newList.append([self.listOfControlPoints[0].x - self.listOfControlPoints[0].rightSide.thickness, self.listOfControlPoints[0].y, x, 0.0])

        for i in range(1, len(self.listOfControlPoints)-1):

            X = self.listOfControlPoints[i].x - self.listOfControlPoints[i].rightSide.thickness
            Y = self.listOfControlPoints[i].y

            newList.append([X, Y, x, x])

        if round:
            newList.append([self.listOfControlPoints[-1].x - self.listOfControlPoints[-1].rightSide.thickness, self.listOfControlPoints[-1].y, x, x])

        else:
            newList.append([self.listOfControlPoints[-1].x - self.listOfControlPoints[-1].rightSide.thickness, self.listOfControlPoints[-1].y, 0.0, x])

        return newList

    def getShape2D(self, round = False):

        leftLine = self.generate_LeftLine(round)
        rightLine = self.generate_RightLine(round)

        rightLine.reverse()

        Line = leftLine + rightLine

        newLine = [Line[0]]


        for i in range(len(Line)):
            if not node_equal(newLine[-1], Line[i]):
                newLine.append(Line[i])
        if not node_equal(newLine[-1], leftLine[0]):
            newLine.append(leftLine[0])
        else:
            newLine[-1]=copy.deepcopy(leftLine[0])


        newLine =piecewisePolynom2D.SharpenListTangentPoints(newLine, 0.1)

        Line = newLine

        ListTangentPoints = piecewisePolynom2D.createListTangentPoints(Line)

        ListPolynom = piecewisePolynom2D.createListPolynom(ListTangentPoints, 1.0)

        C1smoothPiecewisePolynomialChain = piecewisePolynom2D.CLOSEDC1smoothPiecewisePolynomialChain(ListPolynom)

        shape2D = C1smoothPiecewisePolynomialChain

        return shape2D

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



class SOFIA(object):

    def __init__(self, sofia2D, list_holes, depth):

        if  not isinstance(sofia2D, SOFIA2D):
            raise TypeError, "bad shape type"

        for hole in list_holes:
            if not isinstance(hole, SOFIA2D):

                raise TypeError, "bad hole type"

        if depth <=0.0:
            raise ValueError, "bad depth"

        self.sofia2D = sofia2D
        self.list_holes = list_holes
        self.depth = depth

    def getShape(self):

        shape2D = self.sofia2D.getShape2D(False)

        for hole in self.list_holes:

            shape2D_hole = hole.getShape2D(True)
            shape2D = primitives2D.Difference(shape2D, shape2D_hole)


        shape=primitives.ExtrusionOfShape2D(shape2D, self.depth,0.0,0.0, primitives.Point(0.0,0.0,0.0))

        return shape


def generateSOFIA2DManually(listOfDrawnPoints):

    """
    give the list of points as [x, y, thicknessLeft, thicknessRight]
    give the points in order of decreasing y

    """

    listofControlPoints = []

    for point in listOfDrawnPoints:
        index = listOfDrawnPoints.index(point)
        ab = absisse(point, listOfDrawnPoints)
        x = point[0]
        y = point[1]

        thicknessLeft = point[2]
        thicknessRight = point[3]

        listofControlPoints.append(ControlPoint(index, ab, x, y, Side("left", thicknessLeft), Side("right", thicknessRight)))



    return SOFIA2D(listofControlPoints)


def generateSOFIAManually(listOfDrawnPoints_main, listsOfDrawnPoints_holes, depth):

    """
    give the list of points as [x, y, thicknessLeft, thicknessRight]
    give the points in order of decreasing y

    """

    sofia2D = generateSOFIA2DManually(listOfDrawnPoints_main)

    list_holes = []

    for listsOfDrawnPoints_hole in listsOfDrawnPoints_holes:

        hole = generateSOFIA2DManually(listsOfDrawnPoints_hole)

        list_holes.append(hole)

    return SOFIA(sofia2D, list_holes, depth)


if False:

    ################
    ###
    ###EXAMPLE
    ###
    ################


    listOfDrawnPoints = [[0.00, 0.1, [0.0, 0.03],[0.0, 0.03]],\
                        [0.0, 0.075, [0.00, 0.05],[0.0, 0.05]],\
                        [0.0, 0.05, [0.01, 0.05], [0.01, 0.05]],\
                        [0.00, 0.025, [0.01, 0.05], [0.01, 0.05]],\
                        [0.00, 0.00, [0.00, 0.02], [0.00, 0.02]],\
                        [0.00, -0.025, [0.00, 0.02], [0.00, 0.02]],\
                        [0.00, -0.05, [0.02, 0.05], [0.02, 0.05]],\
                        [0.00, -0.075, [0.00, 0.05], [0.00, 0.05]],\
                        [0.00, -0.1, [0.00, 0.05], [0.00, 0.05]]]

    sofia = generateSOFIAManually(listOfDrawnPoints, 0.05)

    shape = sofia.getShape()

    import numpy as np
    import random
    import matplotlib.pyplot as plt
    (n,p)=(300,300)
    x , y = np.meshgrid(np.linspace(-0.15, 0.15,n),np.linspace(-0.15,0.12,p))
    z = np.zeros((n,p))
    for i in range(n):
        for j in range(p):
            z[i][j] = shape.eval(primitives.Point(x[i][j], y[i][j], 0.025))
    plt.figure()
    levels = [-0.1, 0.0]
    contour = plt.contour(x, y, z, levels,)
    plt.clabel(contour, colors = 'k', fmt = '%2.1f', fontsize=12)
    plt.show()
