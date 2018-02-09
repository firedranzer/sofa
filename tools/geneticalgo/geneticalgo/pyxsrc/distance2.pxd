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
cimport primitives2D
from libc.math cimport sin, cos, acos, exp, sqrt, fabs, M_PI
cimport numpy
cimport cython


cdef double equation_droite(primitives2D.Vector2D vect, primitives2D.Point2D p)
cdef bint point_in_vector(primitives2D.Point2D p, primitives2D.Vector2D vect)
cdef bint crossed_vector_line(primitives2D.Vector2D vect1, primitives2D.Vector2D vect2)
cdef bint crossed_vectors(primitives2D.Vector2D vect1, primitives2D.Vector2D vect2)

cdef double distance_point_point(primitives2D.Point2D p1, primitives2D.Point2D p2)
cdef double distance_point_vector(primitives2D.Point2D p, primitives2D.Vector2D vect)
cdef double distance_vector_vector(primitives2D.Vector2D vect1, primitives2D.Vector2D vect2)
cdef double distance_point_right_square(primitives2D.Point2D p, primitives2D.Point2D bottom_square, primitives2D.Point2D top_square)
cdef double plateau(double Xmin, double Xmax, double Ymin, double Ymax, double thickness, primitives2D.Point2D p)

cdef class box:

    cdef public primitives2D.Point2D box_bottom, box_top
    cdef public bint bottom_left_corner_in
    cdef public list list_vects_sign, list_vects_distance

cdef class polygon_boosted_evaluation(primitives2D.Shape2D):

    cdef public double minX, minY, maxX, maxY, dx, dy, thickness
    cdef public list list_boxes
    cdef double eval(self, primitives2D. Point2D p)
