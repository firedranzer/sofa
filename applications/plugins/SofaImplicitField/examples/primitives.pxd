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

cpdef list listA
cpdef list listB

cdef int i

cpdef getListAandB()
cpdef getListPrimitives()

cdef class Point(object):

    cdef double x, y, z

    cdef display(self,)

cdef class Shape(object):

    cpdef double eval(self, Point)

    cpdef tuple toString(self)

cdef class Union(Shape):

    cdef Shape first, second

    cpdef double eval(self,Point)

    cpdef tuple toString(self)


cdef class Intersection(Shape):

    cdef Shape first, second

    cpdef double eval(self,Point)

    cpdef tuple toString(self)

cdef class Difference(Shape):

    cdef Shape first, second

    cpdef double eval(self,Point)

    cpdef tuple toString(self)

cpdef int generateNewPrimitives()

cdef class Primitives(Shape):

    cdef str sign, type
    cdef double axisX, axisY, axisZ, theta, phi, cosTheta, cosPhi, sinTheta, sinPhi
    cdef Point center
    cdef int index
    cdef list identifier

    cdef translationRotation(self,Point)
    cpdef translationRotationToString(self)
    cpdef gradXtranslationRotationToString(self)
    cpdef gradYtranslationRotationToString(self)
    cpdef gradZtranslationRotationToString(self)

cdef class Ellipsoid(Primitives):

    cpdef double eval(self,Point)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)

    cpdef tuple grad(self)

    cpdef tuple toString(self)

cdef class Frisbee(Primitives):

    cpdef double eval(self,Point)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)

    cpdef tuple grad(self)

    cpdef tuple toString(self)

cdef class Cylinder(Primitives):

    cdef str radial, height

    cpdef double eval(self,Point)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)

    cpdef tuple grad(self)

    cpdef tuple toString(self)
