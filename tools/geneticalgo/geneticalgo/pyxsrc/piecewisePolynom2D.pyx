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
#cimport piecewisePolynom2D

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

    def __init__(self, X, Y, orientation):

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
        self.identifier=[self.type, self.X1, self.X2, self.vect, self.orientation]

    cpdef double eval(self,primitives2D.Point2D point):

        cdef double l=math.sqrt(self.vect.firstCoord()*self.vect.firstCoord()\
                     +self.vect.secondCoord()*self.vect.secondCoord())
        print "l="+str(l)
        if l==0.0:
            raise ValueError, "both points are the same"

        cdef double sinTheta=self.vect.secondCoord()/l
        cdef double cosTheta=self.vect.firstCoord()/l

        cdef primitives2D.Point2D p1_temp = primitives2D.Point2D(0.0,0.0)
        cdef primitives2D.Point2D p2_temp = primitives2D.translationRotation(sinTheta, cosTheta, self.X1.p, self.X2.p)

        cdef Tangency2D t1_temp = Tangency2D(cosTheta * self.X1.t.firstCoord - sinTheta * self.X1.t.secondCoord , sinTheta * self.X1.t.firstCoord - cosTheta * self.X1.t.secondCoord)
        cdef Tangency2D t2_temp = Tangency2D(cosTheta * self.X2.t.firstCoord - sinTheta * self.X2.t.secondCoord , sinTheta * self.X2.t.firstCoord - cosTheta * self.X2.t.secondCoord)


        if t1_temp.firstCoord == 0.0 or t2_temp.firstCoord == 0.0:
            raise ValueError, "vertical tangent"

        point=primitives2D.translationRotation(sinTheta, cosTheta, self.X1.p, point)
        print "t1_temp.firstCoord"+str(t1_temp.firstCoord)
        print "t2_temp.firstCoord"+str(t2_temp.firstCoord)

        alpha1 = t1_temp.secondCoord/t1_temp.firstCoord
        alpha2 = t2_temp.secondCoord/t2_temp.firstCoord

        cdef double x = point.x
        cdef double y = point.y

        print "(p2_temp.x - p1_temp.x)" +str(p2_temp.x - p1_temp.x)


        function = self.X1.wRight * alpha1 * (x - p1_temp.x) * (x - p1_temp.x) * (x - p2_temp.x) /((p2_temp.x - p1_temp.x) * (p2_temp.x - p1_temp.x))\
                 + self.X2.wLeft * alpha2 * (x - p2_temp.x) * (x - p2_temp.x) * (x - p1_temp.x) /((p2_temp.x - p1_temp.x) * (p2_temp.x - p1_temp.x))

        if x > p1_temp.x and x < p2_temp.x:
            return self.orientation * (y - function)
        else:
            return self.orientation * y

def createListTangentPoints(listOfCouplesxyWeight):


    n = len(listOfCouplesxyWeight)

    if n<2:
        raise ValueError, "I need at least two points!"

    newList=[]


    p1=primitives2D.Point2D(listOfCouplesxyWeight[0][0],listOfCouplesxyWeight[0][1])
    p2=primitives2D.Point2D(listOfCouplesxyWeight[1][0],listOfCouplesxyWeight[1][1])

    if primitives2D.equal(p1, p2):
        raise ValueError, "two given points are the same"
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
                raise ValueError, "two points are the same"


            tan1 = Tangency2D(p2.x - p1.x , p2.y - p1.y)
            tan2 = Tangency2D(p3.x - p2.x , p3.y - p2.y)

            if abs(Det(tan1, tan2))<0.1*math.sqrt(norm2(tan1)*norm2(tan2)) and scal(tan1, tan2) < 0.0:
                raise ValueError, "the succesive points make a too sharp angle"

            while scal(tan1,tan1 + tan2) <= 0.1*math.sqrt(norm2(tan1)*norm2(tan2)):
                tan2 = tan2.prod(0.5)

            t2 = tan1 + tan2

            wLeft2 = listOfCouplesxyWeight[i+1][2]
            wRight2 = listOfCouplesxyWeight[i+1][3]

            newList.append(WeightedTangencedPoint2D(p2,t2,wLeft2, wRight2))

    p1=primitives2D.Point2D(listOfCouplesxyWeight[-2][0],listOfCouplesxyWeight[-2][1])
    p2=primitives2D.Point2D(listOfCouplesxyWeight[-1][0],listOfCouplesxyWeight[-1][1])

    if primitives2D.equal(p1, p2):
        raise ValueError, "two points are the same"

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
            raise ValueError, "two points are equal"

        newList.append(Polynom(listOfTangentPoints[i], listOfTangentPoints[i+1], orientation))

    return newList

def C1smoothPiecewisePolynomialChain(listPolynom):

    """
    The body of the shape will be on the right of the line, according to its
    orientation. It may not be bounded!!
    """

    length=len(listPolynom)

    if length<1:

        raise ValueError, "I need more than one polynom!!"

    newShape=listPolynom[0]

    if length>1:
        for i in range(length-1):

            P1 = listPolynom[i]
            P2 = listPolynom[i+1]

            if P1.orientation != P2.orientation:
                raise ValueError, "inconsistent orientation"

            if P1.orientation * primitives2D.Det(P1.vect,P2.vect)<=0.0:

                newShape = primitives2D.Intersection(newShape, P2)

            else:

                newShape = primitives2D.Union(newShape, P2)

    return newShape

