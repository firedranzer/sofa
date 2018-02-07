#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - thomas.morzadec@inria.fr
##
####################################################################################################

# distutils: language=c++
# cython: profile=True

import numpy
import math
from libc.math cimport sin, cos, acos, exp, sqrt, fabs, M_PI
cimport numpy
cimport cython


cdef int i

cpdef str generateNewSubIndex()

cpdef void clearOut()

cdef class ListOfLitteralExpressions(object):

    cdef public list listA
    cdef public list listB

    cdef public list listgradientXTemp
    cdef public listgradientYTemp
    cdef public listSqrt


    cpdef ListOfLitteralExpressions plus(self, ListOfLitteralExpressions)

cdef class ListOfPrimitives(object):

    cdef public listPrimitives

    cpdef ListOfPrimitives plus(self, ListOfPrimitives)

cdef class ListForWriting(object):

    cdef public listWritingA
    cdef public listWritingB

    cpdef ListForWriting plus(self, ListForWriting)


cdef class Point2D(object):

    cdef public double x, y

    cdef display(self)

cdef class Vector2D(object):

       cdef public Point2D first, second

       cpdef double firstCoord(self)

       cpdef double secondCoord(self)

cpdef double norm(Vector2D u)

cpdef double Det(Vector2D u, Vector2D v)

cpdef double scal(Vector2D u, Vector2D v)

cdef Point2D translationRotation(double, double, Point2D, Point2D)

cdef class Shape2D(object):

    cdef public str index, subIndex, type

    cdef double eval(self, Point2D)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfPrimitives getListOfPrimitives(self)

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)


    cpdef ListForWriting getListForWriting(self)


cdef class Union(Shape2D):

    cdef public Shape2D first, second

    cdef double eval(self,Point2D point)

    cpdef tuple toString(self)

    cpdef str toWriting(self)


    cpdef ListOfPrimitives getListOfPrimitives(self)
    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)
    cpdef ListForWriting getListForWriting(self)



cdef class Intersection(Shape2D):

    cdef public Shape2D first, second

    cdef double eval(self,Point2D point)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfPrimitives getListOfPrimitives(self)
    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)
    cpdef ListForWriting getListForWriting(self)



cdef class Difference(Shape2D):

    cdef public Shape2D first, second

    cdef double eval(self,Point2D point)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfPrimitives getListOfPrimitives(self)
    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)
    cpdef ListForWriting getListForWriting(self)



cdef class Primitives2D(Shape2D):

    cdef list identifier

    cdef double eval(self,Point2D point)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfPrimitives getListOfPrimitives(self)
    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)
    cpdef ListForWriting getListForWriting(self)





cdef class Ellipse(Primitives2D):

    cdef public double axisX, axisY, theta, cosTheta, sinTheta
    cdef public Point2D center
    cdef public tuple coord


    cdef double eval(self,Point2D point)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfPrimitives getListOfPrimitives(self)
    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)
    cpdef ListForWriting getListForWriting(self)



cdef class HalfPlaneGivenByAVector2D(Shape2D):

    cdef public Vector2D vect
    cdef public Point2D center
    cdef public double sinTheta
    cdef public double cosTheta
    cdef public list identifier

    cdef double eval(self,Point2D point)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfPrimitives getListOfPrimitives(self)
    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)
    cpdef ListForWriting getListForWriting(self)


cpdef list createPolygoalChain(list)
