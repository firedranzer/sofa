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


import primitives
import accordion
import random
import operator

cimport numpy
cimport cython
cimport primitives
cimport accordion
cimport geneticAlgorithm
from libc.stdlib cimport qsort


cdef double heigthTube=10.0
cdef double radiusTube=3.0
cdef double thickness=0.5
cdef int number_of_cavities=3
generate_random="ON"
TYPE=["ellipsoid","frisbee"]



## Indiviual

cdef class Individual(accordion.TubeWithCavities):


#    global heigthTube, radiusTube, thickness, number_of_cavities

    def __init__(self, str type):

        accordion.TubeWithCavities.__init__(self,heigthTube, radiusTube, thickness)

        for i in range(1,number_of_cavities):
            heigth=0.5+i*(heigthTube-0.5)/float(number_of_cavities)

            if generate_random=="ON":
                axisX=random.uniform(4.0,7.0)
                axisY=random.uniform(1.0,7.0)
                axisZ=1.58
            else:
                axisX=5.0
                axisY=5.0
                axisZ=1.58

            cavity=[heigth,type,axisX,axisY,axisZ]
            self.addCavity(cavity)

        self.level=random.uniform(0.0,10.0)

def key_func(Individual ind):
    return ind.level

cdef class Population:

    def __init__(self):
        self.pop = []

    cpdef add_pop(self, Individual ind):
        self.pop.append(ind)

    cpdef remove_pop(self, Individual ind):
        self.pop.remove(ind)

    @cython.boundscheck(False)
    @cython.wraparound(False)
    @cython.initializedcheck(False)
    @cython.cdivision(True)
    @cython.infer_types(True)
    @cython.binding(False)

    cpdef update(self):
        self.pop.sort(key=key_func)


cdef Population POPULATION=Population()

cpdef getPOPULATION():
    return POPULATION


cpdef generate_initPop(int n):

    cdef Individual individual

    cdef int i

    for i in range(n):
        type=random.choice(TYPE)
        individual=Individual(type)
        POPULATION.add_pop(individual)


cpdef popDisplay(Population population):

    population.update()

    cdef Individual ind

    for ind in population.pop:

        print str(ind.level)


###
### MUTATIONS
###


cpdef mutation_axisX(Individual ind):

    length=len(ind.listCavities)

    if length==0:
        raise ValueError, "their is no cavity to mutate"

    index=random.randint(0,length-1)

    cdef double axisX=ind.listCavities[index][2]

    cdef double epsilon=random.uniform(-0.5,0.5)
    axisX=max(4.0, axisX+epsilon)
    ind.listCavities[index][2]=axisX

cpdef mutation_axisY(Individual ind):

    length=len(ind.listCavities)

    if length==0:
        raise ValueError, "their is no cavity to mutate"

    index=random.randint(0,length-1)

    cdef double axisY=ind.listCavities[index][3]

    cdef double epsilon=random.uniform(-0.5,0.5)
    axisY=max(0.5, axisY+epsilon)
    ind.listCavities[index][3]=axisY


cpdef mutation_axisZ(Individual ind):

    length=len(ind.listCavities)

    if length<=1:
        raise ValueError, "their is no cavity to mutate, or don't touch the first cavity"

    index=random.randint(1,length-1)

    cdef double axisZ=ind.listCavities[index][4]

    cdef double epsilon=random.uniform(-0.2,0.2)
    axisZ=max(0.5,min(1.5, axisZ+epsilon))
    ind.listCavities[index][4]=axisZ


cpdef mutation_type(Individual ind):

    length=len(ind.listCavities)

    if length==0:
        raise ValueError, "their is no cavity to mutate"

    index=random.randint(0,length-1)

    cdef str type=ind.listCavities[index][1]

    if type=="ellipsoid":
        ind.listCavities[index][1]="frisbee"
    else:
        ind.listCavities[index][1]="ellipsoid"
    print ind.listCavities[index][1]


