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
import copy
cimport numpy
cimport cython
cimport primitives
cimport accordion

from libc.stdlib cimport qsort

cdef double heigthTube, radiusTube, thickness
cdef int number_of_cavities

cdef str generate_random, mutationType, mutationAxisX, mutationAxisY, mutationAxisZ

cdef list TYPE

cdef class Individual(accordion.TubeWithCavities):
    cdef double level

    cpdef str display(self)

cpdef Individual getNewInd(str)

#def key_func(Individual)

cpdef Individual copyInd (Individual)

cpdef tuple accordionFreeDimension(Individual)


cdef class Population:

    cdef list pop

    cpdef void add_pop(self, Individual)
    cpdef void remove_pop(self, Individual)
    cpdef void update(self)
    cpdef void deleteFrom(self, int)


cdef Population POPULATION

cpdef Population getPOPULATION()


cpdef void generate_initPop(int n)

cpdef void popDisplay(Population)

###
### MUTATIONS
###


cpdef void mutation_axisX(Individual)

cpdef void mutation_axisY(Individual)

cpdef void mutation_axisZ(Individual)

cpdef void mutation_type(Individual)

cpdef void mutation(Individual)

cpdef void mutation_Pop(int, int)

###
###CROSSING
###

cpdef void crossing_ind(Individual, Individual)

cpdef void crossing_Pop(int)

cpdef void evaluation(Individual)

cpdef void getStart(int)
