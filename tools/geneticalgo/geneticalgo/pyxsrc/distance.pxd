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


cimport primitives2D

cdef class Tangency2D(object):

    cdef public double firstCoord, secondCoord

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


cimport primitives2D

cdef bint crossed_vectors(primitives2D.Vector2D vect1, primitives2D.Vector2D vect2)
cdef double distance_point_point(primitives2D.Point2D p1, primitives2D.Point2D p2)
cdef double distance_point_vector(primitives2D.Point2D p, primitives2D.Vector2D vect)
cdef double distance_vector_vector(primitives2D.Vector2D vect1, primitives2D.Vector2D vect2)
cdef double distance_point_right_square(primitives2D.Point2D p, primitives2D.Point2D bottom_square, primitives2D.Point2D top_square)


