#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 CNRS, INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - damien.marchal@univ-lille1.fr
##     - thomas.morzadec@inria.fr
##
####################################################################################################
# distutils: language=c++
import primitives
cimport numpy
cimport cython
cimport primitives

## Shape generation function


cdef class TubeWithCavities(primitives.Shape):

    cdef double heigth, radius, thickness
    cdef list listCavities


    cpdef addCavity(self, list)

cpdef tuple accordionFreeDimension(double, double, double, list)
    

cpdef tuple accordionRecoveringGiven(double, double, double, str, list, list, list, double)

cpdef tuple accordionUniform(double,double, double, str, int, double, double , double)
