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
import accordion
import operator
cimport numpy
cimport cython
cimport primitives
cimport accordion

cdef class Individual(accordion.TubeWithCavities):
    cdef double level

    cpdef str display(self)

cdef class Population:

    cdef list pop

    cpdef add_pop(self, Individual)
    cpdef remove_pop(self, Individual)
    cpdef update(self)
