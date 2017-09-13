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


cdef class Point(object):

    cdef double x, y, z

    cdef display(self,)

cdef class Shape(object):

    cpdef double eval(self, Point)

    cpdef str toString(self)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)


cdef class Union(Shape):

    cdef Shape first, second

    cpdef double eval(self,Point)

    cpdef str toString(self)

    cpdef str indPos(self)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)


cdef class Intersection(Shape):

    cdef Shape first, second

    cpdef double eval(self,Point)

    cpdef str toString(self)

    cpdef str indPos(self)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)

cdef class Difference(Shape):

    cdef Shape first, second

    cpdef double eval(self,Point)

    cpdef str toString(self)

    cpdef str indPosDiff(self)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)

cdef class Primitives(Shape):

    cdef str sign
    cdef double axisX, axisY, axisZ, cosTheta, cosPhi, sinTheta, sinPhi
    cdef Point center

    cdef translationRotation(self,Point)
    cpdef translationRotationToString(self)

cdef class Ellipsoid(Primitives):

    cdef str type

    cpdef double eval(self,Point)

    cpdef str toString(self)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)

cdef class Frisbee(Primitives):

    cdef str type

    cpdef double eval(self,Point)

    cpdef str toString(self)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)

cdef class Ball(Primitives):

    cdef str type

    cpdef double eval(self,Point)

    cpdef str toString(self)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)

cdef class Cylinder(Primitives):

    cdef str type

    cpdef double eval(self,Point)

    cpdef str toString(self)

    cpdef str gradX(self)

    cpdef str gradY(self)

    cpdef str gradZ(self)
