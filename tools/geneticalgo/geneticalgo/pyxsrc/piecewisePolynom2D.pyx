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

# distutils: language=c++
# cython: profile=True

import numpy
import math
import primitives2D
import triangulation
import copy
#cimport piecewisePolynom2D

def clockwise(triangle):

#    barycenter = ((triangle[0][0] + triangle[1][0] + triangle[2][0])/3.0,\
#                  (triangle[1][1] + triangle[1][1] + triangle[2][1])/3.0)

    if triangulation.equation_droite(triangle[0], triangle[1], triangle[2]) <= 0.0:
        return True
    else:
        return False


def clockwise_triangle(triangle):


    new_triangle = copy.deepcopy(triangle)

#    barycenter = [(triangle[0][0] + triangle[1][0] + triangle[2][0])/3.0,\
#                  (triangle[1][1] + triangle[1][1] + triangle[2][1])/3.0]

    if triangulation.equation_droite(triangle[0], triangle[1], triangle[2]) > 0.0:
        new_triangle[1] = triangle[2]
        new_triangle[2] = triangle[1]
        print "triangle   "+ str(triangle)
        print "new_triangle   "+ str(new_triangle)

    return new_triangle

cdef class Tangency2D(object):

    def __init__(self, firstCoord, secondCoord):
        self.firstCoord = firstCoord
        self.secondCoord = secondCoord

    def prod(self, x):
        return Tangency2D(self.firstCoord * x, self.firstCoord * x)

    def __add__(self, u):

        return Tangency2D(self.firstCoord + u.firstCoord, self.secondCoord + u.secondCoord)


def norm2(u):
    return u.firstCoord**2 + u.secondCoord**2


cpdef Det(u , v):

    cdef double res = (u.firstCoord) * (v.secondCoord) - (u.secondCoord) * (v.firstCoord)

    return res

def scal(u, v):

    return u.firstCoord * v.firstCoord + u.secondCoord * v.secondCoord

cdef class WeightedTangencedPoint2D(object):

    def __init__(self, p, t, wLeft, wRight):

        self.p = p
        self.t = t
        self.wLeft = wLeft
        self.wRight = wRight

def equal(X,Y):
    return primitives2D.equal(X.p, Y.p)

cdef class Polynom(primitives2D.Shape2D):

    def __init__(self, X, Y, orientation, side = "down"):

        if equal(X,Y):

            raise ValueError, "X and Y can not be the same"

        primitives2D.Shape2D.__init__(self)

        self.type="Polynom"
        self.X1 = X
        self.X2 = Y
        self.vect = primitives2D.Vector2D(self.X1.p,self.X2.p)


        if not orientation in [-1.0,1.0]:
            raise ValueError, "not orientation in [-1.0,1.0]"
        self.orientation = orientation

        if not side in ["up","down"]:
            raise ValueError, "not side in ['up','down']"

        self.side = side
        self.identifier=[self.type, self.X1, self.X2, self.vect, self.orientation, self.side]

    cpdef double eval(self,primitives2D.Point2D point):

        cdef double l=math.sqrt(self.vect.firstCoord()*self.vect.firstCoord()\
                     +self.vect.secondCoord()*self.vect.secondCoord())
        if l==0.0:
            raise ValueError, "both points are the same"

        cdef double sinTheta=self.vect.secondCoord()/l
        cdef double cosTheta=self.vect.firstCoord()/l

        cdef primitives2D.Point2D p1_temp = primitives2D.Point2D(0.0,0.0)
        cdef primitives2D.Point2D p2_temp = primitives2D.translationRotation(sinTheta, cosTheta, self.X1.p, self.X2.p)

        cdef Tangency2D t1_temp = Tangency2D(cosTheta * self.X1.t.firstCoord + sinTheta * self.X1.t.secondCoord , -sinTheta * self.X1.t.firstCoord + cosTheta * self.X1.t.secondCoord)
        cdef Tangency2D t2_temp = Tangency2D(cosTheta * self.X2.t.firstCoord + sinTheta * self.X2.t.secondCoord , -sinTheta * self.X2.t.firstCoord + cosTheta * self.X2.t.secondCoord)

        if t2_temp.firstCoord == 0.0:
            print str(self.X1.p.x)
            print str(self.X1.p.y)
            print str(self.X2.p.x)
            print str(self.X2.p.y)
            print "self.X2.t.firstCoord " + str(self.X2.t.firstCoord)
            print "self.X2.t.secondCoord " + str(self.X2.t.secondCoord)
            print "cosTheta "+str(cosTheta)
            print "sinTheta "+str(sinTheta)

        if t1_temp.firstCoord == 0.0 or t2_temp.firstCoord == 0.0:

            print "t1_temp.firstCoord  "+str(t1_temp.firstCoord)
            print "t2_temp.firstCoord  "+str(t2_temp.firstCoord)
            raise ValueError, "vertical tangent"

        point=primitives2D.translationRotation(sinTheta, cosTheta, self.X1.p, point)

        alpha1 = t1_temp.secondCoord/t1_temp.firstCoord
        alpha2 = t2_temp.secondCoord/t2_temp.firstCoord

        cdef double x = point.x
        cdef double y = point.y

        function = self.X2.wLeft * alpha2 * (x - p1_temp.x) * (x - p1_temp.x) * (x - p2_temp.x) /((p2_temp.x - p1_temp.x) * (p2_temp.x - p1_temp.x))\
                 + self.X1.wRight * alpha1 * (x - p2_temp.x) * (x - p2_temp.x) * (x - p1_temp.x) /((p2_temp.x - p1_temp.x) * (p2_temp.x - p1_temp.x))

        if self.side == "up":
            if self.orientation == 1.0:
                if x >= p1_temp.x and x <= p2_temp.x  and y >=0.0:
                    return (y - function)
                else:
                    return 1.0
            else:
                if x >= p1_temp.x and x <= p2_temp.x  and y <=0.0:
                    return -(y - function)
                else:
                    return 1.0

        elif self.side == "down":
            if self.orientation == 1.0:
                if x >= p1_temp.x and x <= p2_temp.x:
                    return (y - function)

                else:
                    return y
            else:
                if x >= p1_temp.x and x <= p2_temp.x:
                    return -(y - function)
                else:
                    return -y
        else:

            raise ValueError, "I need a 'up' or 'down'"


def createListTangentPoints(listOfCouplesxyWeight):


    n = len(listOfCouplesxyWeight)

    if n<2:
        raise ValueError, "I need at least two points!"

    newList=[]


    p1=primitives2D.Point2D(listOfCouplesxyWeight[0][0],listOfCouplesxyWeight[0][1])
    p2=primitives2D.Point2D(listOfCouplesxyWeight[1][0],listOfCouplesxyWeight[1][1])

    if primitives2D.equal(p1, p2):
        raise ValueError, "two successive given points are the same"

    t1 = Tangency2D(p2.x - p1.x, p2.y - p1.y)

    wLeft1 = listOfCouplesxyWeight[0][2]
    wRight1 = listOfCouplesxyWeight[0][3]
    newList.append(WeightedTangencedPoint2D(p1,t1,wLeft1, wRight1))

    if n>2:
        for i in range(0,n-2):

            p1=primitives2D.Point2D(listOfCouplesxyWeight[i][0],listOfCouplesxyWeight[i][1])
            p2=primitives2D.Point2D(listOfCouplesxyWeight[i+1][0],listOfCouplesxyWeight[i+1][1])
            p3=primitives2D.Point2D(listOfCouplesxyWeight[i+2][0],listOfCouplesxyWeight[i+2][1])

            if primitives2D.equal(p1, p2) or primitives2D.equal(p2, p3) or primitives2D.equal(p1, p3):
                raise ValueError, "two successive points are the same"


            tan1 = Tangency2D(p2.x - p1.x , p2.y - p1.y)
            tan2 = Tangency2D(p3.x - p2.x , p3.y - p2.y)

#            if abs(Det(tan1, tan2))<0.01*math.sqrt(norm2(tan1)*norm2(tan2)) and scal(tan1, tan2) < 0.0:
#                raise ValueError, "the succesive points make a too sharp angle"

            while scal(tan1,tan1 + tan2) <= 0.1*math.sqrt(norm2(tan1)*norm2(tan1 + tan2)):

                tan2 = tan2.prod(0.5)

            t2 = tan1 + tan2

            wLeft2 = listOfCouplesxyWeight[i+1][2]
            wRight2= listOfCouplesxyWeight[i+1][3]

            newList.append(WeightedTangencedPoint2D(p2,t2,wLeft2, wRight2))

    p1=primitives2D.Point2D(listOfCouplesxyWeight[-2][0],listOfCouplesxyWeight[-2][1])
    p2=primitives2D.Point2D(listOfCouplesxyWeight[-1][0],listOfCouplesxyWeight[-1][1])

    if primitives2D.equal(p1, p2):
        raise ValueError, "two successive points are the same"

    t2 = Tangency2D(p2.x - p1.x, p2.y - p1.y)

    wLeft2 = listOfCouplesxyWeight[-1][2]
    wRight2 = listOfCouplesxyWeight[-1][3]

    newList.append(WeightedTangencedPoint2D(p2,t2,wLeft2, wRight2))

    return newList

def createListPolynom(listOfTangentPoints, orientation):

    newList=[]
    if len(listOfTangentPoints)<2:
        raise ValueError, "I need more than 1 points"

    for i in range(len(listOfTangentPoints)-1):

        if equal(listOfTangentPoints[i],listOfTangentPoints[i+1]):
            raise ValueError, "two successive points are equal"

        newList.append(Polynom(listOfTangentPoints[i], listOfTangentPoints[i+1], orientation, 'down'))

    return newList

def CLOSEDC1smoothPiecewisePolynomialChain(listPolynom):

    """
    The body of the shape will be on the right of the line, according to its
    orientation. BECAREFUL: first and last point have to be equal!! Points have to be clockwise!!!!!
    """

    length=len(listPolynom)

    if length<1:

        raise ValueError, "I need more than one polynom!!"

    listVectors = []

    listVertices = []
    listShape = []

    for polynom in listPolynom:
        listVectors.append([(polynom.vect.first.x, polynom.vect.first.y), (polynom.vect.second.x, polynom.vect.second.y)])
        listVertices.append([polynom.X1.p.x, polynom.X1.p.y])

    listVertices.append([listPolynom[-1].X2.p.x, listPolynom[-1].X2.p.y])

#    if listVertices[0]!=listVertices[-1]:
#        print "listVertices[0]!=listVertices[-1]"+str(listVertices[0])+str( listVertices[-1])
#        raise ValueError, "the chain is not closed"

    liste_triangles = triangulation.trianguler_polygone(listVertices)
    list_shapes_triangle = []

    print "listVectors:   " + str(listVectors)

    for triangle in liste_triangles:
        triangle = clockwise_triangle(triangle)
        if not clockwise(triangle):
            print str(triangle)
            raise ValueError, "ill-oriented triangle"

        vects = [primitives2D.Vector2D(primitives2D.Point2D(triangle[0][0],triangle[0][1]), primitives2D.Point2D(triangle[1][0],triangle[1][1])),\
                 primitives2D.Vector2D(primitives2D.Point2D(triangle[1][0],triangle[1][1]), primitives2D.Point2D(triangle[2][0],triangle[2][1])),\
                 primitives2D.Vector2D(primitives2D.Point2D(triangle[2][0],triangle[2][1]), primitives2D.Point2D(triangle[0][0],triangle[0][1]))\
                 ]
        localShapes = []
        for i in range(3):
            localShapes.append(primitives2D.HalfPlaneGivenByAVector2D(vects[i]))

        shape_triangle = localShapes[0]

        for i in [1,2]:
            shape_triangle = primitives2D.Intersection(shape_triangle, localShapes[i])

        list_shapes_triangle.append(shape_triangle)

    ShapeInt = list_shapes_triangle[0]

    if len(list_shapes_triangle)>1:
        for i in range(1, len(list_shapes_triangle)):
            ShapeInt = primitives2D.Union(ShapeInt, list_shapes_triangle[i])

    for polynom in listPolynom:
        ShapeInt = primitives2D.Union(ShapeInt, Polynom(polynom.X1, polynom.X2, -1.0*polynom.orientation, "up"))

    ShapeExt = primitives2D.Difference(primitives2D.All(), ShapeInt)

    for polynom in listPolynom:
        ShapeExt = primitives2D.Union(ShapeExt, Polynom(polynom.X1, polynom.X2, polynom.orientation, "up"))


    return primitives2D.Difference(ShapeInt, ShapeExt)



