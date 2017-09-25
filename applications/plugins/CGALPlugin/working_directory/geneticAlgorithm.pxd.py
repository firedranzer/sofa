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
cimport numpy
cimport cython
cimport primitives
cimpoort accordion

cdef class Individual(accordion.TubeWuthCavities):

    cdef double level
