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
import distance
#cimport piecewisePolynom2D

from libc.math cimport sin, cos, sqrt

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

    return new_triangle

cdef class Tangency2D(object):

    def __init__(self, firstCoord, secondCoord):
        self.firstCoord = firstCoord
        self.secondCoord = secondCoord

    def prod(self, x):
        self.firstCoord *= x
        self.secondCoord *= x

    def __add__(self, u):

        return Tangency2D(self.firstCoord + u.firstCoord, self.secondCoord + u.secondCoord)


cpdef norm2(u):
    return u.firstCoord**2 + u.secondCoord**2

cpdef norm(u):
    return sqrt(norm2(u))


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
        cdef primitives2D.Vector2D vect = primitives2D.Vector2D(self.X1.p,self.X2.p)
        self.vect = vect

        if not orientation in [-1.0,1.0]:
            raise ValueError, "not orientation in [-1.0,1.0]"
        self.orientation = orientation

        if not side in ["up","down"]:
            raise ValueError, "not side in ['up','down']"

        self.side = side

        cdef double l = sqrt(self.vect.firstCoord()*self.vect.firstCoord() + self.vect.secondCoord()*self.vect.secondCoord())

        if l==0.0:
            raise ValueError, "both points are the same"
        cdef double sinTheta = vect.secondCoord()/l
        cdef double cosTheta = vect.firstCoord()/l
        self.sinTheta = sinTheta
        self.cosTheta = cosTheta
        self.p1_temp = primitives2D.Point2D(0.0,0.0)
        self.p2_temp = primitives2D.translationRotation(sinTheta, cosTheta, X.p, Y.p)

        cdef Tangency2D t1_temp = Tangency2D(cosTheta * X.t.firstCoord + sinTheta * X.t.secondCoord , -sinTheta * X.t.firstCoord + cosTheta * X.t.secondCoord)
        cdef Tangency2D t2_temp = Tangency2D(cosTheta * Y.t.firstCoord + sinTheta * Y.t.secondCoord , -sinTheta * Y.t.firstCoord + cosTheta * Y.t.secondCoord)

        if t2_temp.firstCoord == 0.0:
            print str(X.p.x)
            print str(X.p.y)
            print str(Y.p.x)
            print str(Y.p.y)
            print "self.X2.t.firstCoord " + str(Y.t.firstCoord)
            print "self.X2.t.secondCoord " + str(Y.t.secondCoord)
            print "cosTheta "+str(cosTheta)
            print "sinTheta "+str(sinTheta)

        if t1_temp.firstCoord == 0.0 or t2_temp.firstCoord == 0.0:

            print "t1_temp.firstCoord  "+str(t1_temp.firstCoord)
            print "t2_temp.firstCoord  "+str(t2_temp.firstCoord)
            raise ValueError, "vertical tangent"


        cdef double alpha1 = t1_temp.secondCoord/t1_temp.firstCoord
        cdef double alpha2 = t2_temp.secondCoord/t2_temp.firstCoord

        self.alpha1 = alpha1
        self.alpha2 = alpha2



        self.identifier=[self.type, X, self.X2, self.vect, self.orientation, self.side]


    cdef double eval(self,primitives2D.Point2D point):

        cdef primitives2D.Point2D point_temp = primitives2D.translationRotation(self.sinTheta, self.cosTheta, self.X1.p, point)

        cdef double x = point_temp.x
        cdef double y = point_temp.y

        cdef double function = self.X2.wLeft * self.alpha2 * (x - self.p1_temp.x) * (x - self.p1_temp.x) * (x - self.p2_temp.x) /((self.p2_temp.x - self.p1_temp.x) * (self.p2_temp.x - self.p1_temp.x))\
                 + self.X1.wRight * self.alpha1 * (x - self.p2_temp.x) * (x - self.p2_temp.x) * (x - self.p1_temp.x) /((self.p2_temp.x - self.p1_temp.x) * (self.p2_temp.x - self.p1_temp.x))

        if self.side == "up":
            if self.orientation == 1.0:
                if x >= self.p1_temp.x and x <= self.p2_temp.x  and y >=0.0:
                    return (y - function)
                else:
                    return 1.0
            else:
                if x >= self.p1_temp.x and x <= self.p2_temp.x  and y <=0.0:
                    return -(y - function)
                else:
                    return 1.0

        elif self.side == "down":
            if self.orientation == 1.0:
                if x >= self.p1_temp.x and x <= self.p2_temp.x and y <=0:
                    return (y - function)

                else:
                    return 1.0#y
            else:
                if x >= self.p1_temp.x and x <= self.p2_temp.x and y >=0:
                    return -(y - function)
                else:
                    return 1.0 #-y
        else:

            raise ValueError, "I need a 'up' or 'down'"

def SharpenListTangentPoints(listOfCouplesxyWeight, t):

    n = len(listOfCouplesxyWeight)
    if n <= 3:
        raise ValueError, "flat polygon"

    if not listOfCouplesxyWeight[0][0:2] == listOfCouplesxyWeight[-1][0:2]:
        raise ValueError, "not closed polygon"
    if not 0.0 <= t <0.5:
        raise ValueError, "bad coefficient"

    newList=[]

    p_minus_1 = (listOfCouplesxyWeight[-2][0], listOfCouplesxyWeight[-2][1])
    p0 = (listOfCouplesxyWeight[0][0], listOfCouplesxyWeight[0][1])
    p1 = (listOfCouplesxyWeight[1][0], listOfCouplesxyWeight[1][1])

    P_new1 = (p0[0] - t * (p0[0] - p_minus_1[0]), p0[1] - t * (p0[1] - p_minus_1[1]))
    P_new2 = (p0[0] + t * (p1[0] - p0[0]), p0[1] + t * (p1[1] - p0[1]))

    newList.append([P_new1[0], P_new1[1], listOfCouplesxyWeight[0][2], listOfCouplesxyWeight[0][3]])
    newList.append([P_new2[0], P_new2[1], listOfCouplesxyWeight[0][2], listOfCouplesxyWeight[0][3]])
    n = len(listOfCouplesxyWeight)

    for i in range(n-2):
        p_minus_1 = (listOfCouplesxyWeight[i][0], listOfCouplesxyWeight[i][1])
        p0 = (listOfCouplesxyWeight[i+1][0], listOfCouplesxyWeight[i+1][1])
        p1 = (listOfCouplesxyWeight[i+2][0], listOfCouplesxyWeight[i+2][1])

        p_new1 = (p0[0] - t * (p0[0] - p_minus_1[0]), p0[1] - t * (p0[1] - p_minus_1[1]))
        p_new2 = (p0[0] + t * (p1[0] - p0[0]), p0[1] + t * (p1[1] - p0[1]))

        newList.append([p_new1[0], p_new1[1], listOfCouplesxyWeight[i+1][2], listOfCouplesxyWeight[i+1][3]])
        newList.append([p_new2[0], p_new2[1], listOfCouplesxyWeight[i+1][2], listOfCouplesxyWeight[i+1][3]])

    newList.append([P_new1[0], P_new1[1], listOfCouplesxyWeight[0][2], listOfCouplesxyWeight[0][3]])
    print "ecorned liste=" + str(newList)
    print "begin=" + str(newList[0])
    print "end=" + str(newList[-1])
    return newList


def createListTangentPoints(listOfCouplesxyWeight):


    n = len(listOfCouplesxyWeight)

    if n<2:
        raise ValueError, "I need at least two points!"

    newList=[]


    p_minus_1=primitives2D.Point2D(listOfCouplesxyWeight[-2][0],listOfCouplesxyWeight[-2][1])
    p0=primitives2D.Point2D(listOfCouplesxyWeight[-1][0],listOfCouplesxyWeight[-1][1])
    p1=primitives2D.Point2D(listOfCouplesxyWeight[0][0],listOfCouplesxyWeight[0][1])
    p2=primitives2D.Point2D(listOfCouplesxyWeight[1][0],listOfCouplesxyWeight[1][1])

    if not primitives2D.equal(p0, p1):

        raise ValueError, "not closed poygon!!"


    if primitives2D.equal(p_minus_1, p1) or primitives2D.equal(p1, p2) or primitives2D.equal(p_minus_1, p2):

        print "points" + str(p_minus_1.x)+ "  " + str(p1.x)+ "  " + str(p2.x)+ " \n\n "
        print "points" + str(p_minus_1.y)+ "  " + str(p1.y)+ "  " + str(p2.y)+ " \n\n "
        raise ValueError, "two successive points are the same"


    tan1 = Tangency2D(p1.x - p_minus_1.x , p1.y - p_minus_1.y)
    tan1.prod(1.0/norm(tan1))
    tan2 = Tangency2D(p2.x - p1.x , p2.y - p1.y)
    tan2.prod(1.0/norm(tan2))

    T1 = tan1 + tan2
    WLeft1 = listOfCouplesxyWeight[0][2]
    WRight1 = listOfCouplesxyWeight[0][3]

    newList.append(WeightedTangencedPoint2D(p1,T1,WLeft1, WRight1))

    if n>2:
        for i in range(0,n-2):

            p1=primitives2D.Point2D(listOfCouplesxyWeight[i][0],listOfCouplesxyWeight[i][1])
            p2=primitives2D.Point2D(listOfCouplesxyWeight[i+1][0],listOfCouplesxyWeight[i+1][1])
            p3=primitives2D.Point2D(listOfCouplesxyWeight[i+2][0],listOfCouplesxyWeight[i+2][1])

            if primitives2D.equal(p1, p2) or primitives2D.equal(p2, p3) or primitives2D.equal(p1, p3):

                print "points" + str(p1.x)+ "  " + str(p2.x)+ "  " + str(p3.x)+ " \n\n "
                print "points" + str(p1.y)+ "  " + str(p2.y)+ "  " + str(p3.y)+ " \n\n "
                raise ValueError, "two successive points are the same"


            tan1 = Tangency2D(p2.x - p1.x , p2.y - p1.y)
            tan1.prod(1.0/norm(tan1))
            tan2 = Tangency2D(p3.x - p2.x , p3.y - p2.y)
            tan2.prod(1.0/norm(tan2))

#            if abs(Det(tan1, tan2))<0.01*sqrt(norm2(tan1)*norm2(tan2)) and scal(tan1, tan2) < 0.0:
#                raise ValueError, "the succesive points make a too sharp angle"

#            while scal(tan1,tan1 + tan2) <= 0.1*sqrt(norm2(tan1)*norm2(tan1 + tan2)):

#                tan2 = tan2.prod(0.5)

            t2 = tan1 + tan2

            wLeft2 = listOfCouplesxyWeight[i+1][2]
            wRight2= listOfCouplesxyWeight[i+1][3]

            newList.append(WeightedTangencedPoint2D(p2,t2,wLeft2, wRight2))

    p1=primitives2D.Point2D(listOfCouplesxyWeight[-2][0],listOfCouplesxyWeight[-2][1])
    p2=primitives2D.Point2D(listOfCouplesxyWeight[-1][0],listOfCouplesxyWeight[-1][1])

#    if primitives2D.equal(p1, p2):
#        raise ValueError, "two successive points are the same"

#    t2 = Tangency2D(p2.x - p1.x, p2.y - p1.y)

#    wLeft2 = listOfCouplesxyWeight[-1][2]
#    wRight2 = listOfCouplesxyWeight[-1][3]

    newList.append(WeightedTangencedPoint2D(p2,T1,WLeft1, WRight1))

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

#    if [listPolynom[-1].X2.p.x, listPolynom[-1].X2.p.y] in listVertices:
#            raise ValueError, "already in the list"

    listVertices.append([listPolynom[-1].X2.p.x, listPolynom[-1].X2.p.y])

#    if listVertices[0]!=listVertices[-1]:
#        print "listVertices[0]!=listVertices[-1]"+str(listVertices[0])+str( listVertices[-1])
#        raise ValueError, "the chain is not closed"
    polygon = []

    for vertex in listVertices:

        polygon.append(primitives2D.Point2D(vertex[0], vertex[1]))



    if not (polygon[0].x == polygon[-1].x and polygon[0].y == polygon[-1].y):

        raise ValueError, "non closed polygon"



    ShapeInt = distance.polygon_boosted_evaluation(polygon)

    for polynom in listPolynom:
        ShapeInt = primitives2D.Union(ShapeInt, Polynom(polynom.X1, polynom.X2, polynom.orientation, "up"))

    ShapeExt = primitives2D.Difference(primitives2D.All(), ShapeInt)

    for polynom in listPolynom:
        ShapeExt = primitives2D.Union(ShapeExt, Polynom(polynom.X1, polynom.X2, -polynom.orientation, "up"))


    return primitives2D.Difference(ShapeInt, ShapeExt)



