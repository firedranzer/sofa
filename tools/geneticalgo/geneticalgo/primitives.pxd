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


# cython: profile=True

import numpy
import math
from libc.math cimport sin, cos, acos, exp, sqrt, fabs, M_PI
cimport numpy
cimport cython

cdef int i

cpdef str generateNewIndex()

cdef class ListOfLitteralExpressions(object):
    cdef public list listA
    cdef public list listB
    cdef public list listPrimitives
    cdef public list listgradientDxPrimitives
    cdef public list listgradientDyPrimitives
    cdef public list listgradientDzPrimitives
    cdef public list listgradientXTemp
    cdef public list listgradientYTemp
    cdef public list listgradientZTemp
    cdef public list listSqrt
    cdef public list listDisplayA
    cdef public list listDisplayB

    cpdef ListOfLitteralExpressions plus(self, ListOfLitteralExpressions)

cdef class ListOfPrimitives(object):

    cdef public list listPrimitives
    cdef public list listgradientDxPrimitives
    cdef public list listgradientDyPrimitives
    cdef public list listgradientDzPrimitives


    cpdef ListOfPrimitives plus(self, ListOfPrimitives)


cdef class ListForWriting(object):

    cdef public list listWritingA
    cdef public list listWritingB

    cpdef ListForWriting plus(self, ListForWriting)

cdef class Point(object):

    cdef double x, y, z

    cdef display(self,)

cdef class Shape(object):

    cdef str index

    cpdef double eval(self, Point)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfPrimitives getListOfPrimitives(self)

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)

    cpdef ListForWriting getListForWriting(self)

cdef class Union(Shape):

    cdef Shape first, second

    cpdef double eval(self,Point)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfPrimitives getListOfPrimitives(self)

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)

    cpdef ListForWriting getListForWriting(self)


cdef class Intersection(Shape):

    cdef Shape first, second

    cpdef double eval(self,Point)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfPrimitives getListOfPrimitives(self)

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)

    cpdef ListForWriting getListForWriting(self)

cdef class Difference(Shape):

    cdef Shape first, second

    cpdef double eval(self,Point)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfPrimitives getListOfPrimitives(self)

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)

    cpdef ListForWriting getListForWriting(self)


cdef class Primitives(Shape):

    cdef str sign, type
    cdef double axisX, axisY, axisZ, theta, phi, cosTheta, cosPhi, sinTheta, sinPhi
    cdef Point center

    cdef list identifier
    cdef tuple coord

    cpdef ListOfPrimitives getListOfPrimitives(self)

    cdef translationRotation(self,Point)
#    cpdef translationRotationToString(self)
#    cpdef gradXtranslationRotationToString(self)
#    cpdef gradYtranslationRotationToString(self)
#    cpdef gradZtranslationRotationToString(self)

cdef class Ellipsoid(Primitives):

    cpdef double eval(self,Point)

#    cpdef str gradX(self)

#    cpdef str gradY(self)

#    cpdef str gradZ(self)

#    cpdef tuple grad(self)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

cdef class Frisbee(Primitives):

    cpdef double eval(self,Point)

#    cpdef str gradX(self)

#    cpdef str gradY(self)

#    cpdef str gradZ(self)

#    cpdef tuple grad(self)

    cpdef tuple toString(self)

    cpdef str toWriting(self)

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self)


cdef class Cylinder(Primitives):

    cdef str radial, height

    cpdef double eval(self,Point)

#    cpdef str gradX(self)

#    cpdef str gradY(self)

#    cpdef str gradZ(self)

#    cpdef tuple grad(self)

    cpdef tuple toString(self)

    cpdef str toWriting(self)