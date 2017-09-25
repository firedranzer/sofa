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
import copy


cimport numpy
cimport cython
cimport primitives
cimport accordion
cimport geneticAlgorithm
from libc.stdlib cimport qsort


cdef double heigthTube=10.0
cdef double radiusTube=3.0
cdef double thickness=0.5
cdef int number_of_cavities=2
generate_random="OFF"
TYPE=["ellipsoid","frisbee"]

mutationType="ON"
mutationAxisX="OFF"
mutationAxisY="OFF"
mutationAxisZ="OFF"

cdef primitives.Shape shape_to_export

cpdef primitives.Shape getShape_to_export():
    return shape_to_export


## Indiviual

cdef class Individual(accordion.TubeWithCavities):


#    global heigthTube, radiusTube, thickness, number_of_cavities

    def __init__(self,heigth, radius, thickness):

        accordion.TubeWithCavities.__init__(self,heigth, radius, thickness)

        self.level

    cpdef str display(self):
        cdef str temp="heigthTube="+str(self.heigth)+"\n"\
                     +"radiusTube="+str(self.radius)+"\n"\
                     +"thickness="+str(self.thickness)+"\n\n"\

        for i in range(len(self.listCavities)):

            [heigth,type,axisX,axisY,axisZ]=self.listCavities[i]

            temp=temp+"cavity number "+str(i)+"\n"\
                     +"type="+type+"\n"\
                     +"heigth="+str(heigth)+"\n"\
                     +"axisX="+str(axisX)+"\n"\
                     +"axisY="+str(axisY)+"\n"\
                     +"axisZ="+str(axisZ)+"\n\n"
        return temp

cpdef Individual getNewInd(str type):

    cdef Individual individual=Individual(heigthTube, radiusTube, thickness)

    for i in range(1,number_of_cavities+1):
        heigth=0.5+i*(heigthTube-0.5)/float(number_of_cavities)

        if generate_random=="ON":
            axisX=random.uniform(4.0,7.0)
            axisY=random.uniform(1.0,7.0)
            axisZ=(heigthTube-0.5)/(2*number_of_cavities)
        else:
            axisX=5.0
            axisY=5.0
            axisZ=(heigthTube-0.5)/(2*number_of_cavities)

        cavity=[heigth,type,axisX,axisY,axisZ]
        individual.addCavity(cavity)

    individual.level=random.uniform(0.0,10.0)

    return individual

def key_func(Individual ind):
    return ind.level


cpdef Individual copyInd (Individual individual):

    cdef Individual temp=Individual(individual.heigth,individual.radius,individual.thickness)
    cdef list listCavities=copy.deepcopy(individual.listCavities)
    temp.listCavities=listCavities
    return temp

cpdef tuple accordionFreeDimension(Individual ind):

    cdef primitives.Shape shapePlus, shapeMinus

    shapePlus=primitives.Cylinder("+",ind.radius,ind.radius,ind.heigth/2.0,0.0,0.0,primitives.Point(0.0,0.0,ind.heigth/2.0))

    shapeMinus=primitives.Cylinder("+",ind.radius-ind.thickness,ind.radius-ind.thickness,ind.heigth/2.0-ind.thickness,0.0,0.0,primitives.Point(0.0,0.0,ind.heigth/2.0))

    for cavity in ind.listCavities:

        [heigth,type,axisX,axisY,axisZ]=cavity


        if type=="ellipsoid":

            shapePlus=primitives.Union(shapePlus,primitives.Ellipsoid("+",axisX,axisY,axisZ,0.0,0.0,primitives.Point(0.0,0.0,heigth)))
            shapeMinus=primitives.Union(shapeMinus,primitives.Ellipsoid("+",axisX-ind.thickness,axisY-ind.thickness,axisZ-ind.thickness,0.0,0.0,primitives.Point(0.0,0.0,heigth)))

        elif type=="cylinder":

            shapePlus=primitives.Union(shapePlus,primitives.Cylinder("+",axisX,axisY,axisZ,0.0,0.0,primitives.Point(0.0,0.0,heigth)))
            shapeMinus=primitives.Union(shapeMinus,primitives.Cylinder("+",axisX-ind.thickness,axisY-ind.thickness,axisZ-ind.thickness,0.0,0.0,primitives.Point(0.0,0.0,heigth)))

        elif type=="frisbee":
            shapePlus=primitives.Union(shapePlus,primitives.Frisbee("+",axisX,axisY,axisZ,0.0,0.0,primitives.Point(0.0,0.0,heigth)))
            shapeMinus=primitives.Union(shapeMinus,primitives.Frisbee("+",axisX-ind.thickness,axisY-ind.thickness,axisZ-ind.thickness,0.0,0.0,primitives.Point(0.0,0.0,heigth)))

    shape=primitives.Difference(shapePlus,shapeMinus)

    return (shape,shapeMinus)


cdef class Population:

    def __init__(self):
        self.pop = []

    cpdef void add_pop(self, Individual ind):
        self.pop.append(ind)

    cpdef void remove_pop(self, Individual ind):
        self.pop.remove(ind)

    @cython.boundscheck(False)
    @cython.wraparound(False)
    @cython.initializedcheck(False)
    @cython.cdivision(True)
    @cython.infer_types(True)
    @cython.binding(False)

    cpdef void update(self):
        self.pop.sort(key=key_func)


cdef Population POPULATION=Population()

cpdef Population getPOPULATION():
    return POPULATION


cpdef void generate_initPop(int n):

    cdef Individual individual

    cdef int i

    for i in range(n):
        type=random.choice(TYPE)
        individual=Individual(type)
        POPULATION.add_pop(individual)


cpdef void popDisplay(Population population):

    population.update()

    cdef Individual ind

    for ind in population.pop:

        print str(ind.level)


###
### MUTATIONS
###


cpdef void mutation_axisX(Individual ind):

    length=len(ind.listCavities)

    if length==0:
        raise ValueError, "their is no cavity to mutate"

    index=random.randint(0,length-1)

    cdef double axisX=ind.listCavities[index][2]

    cdef double epsilon=random.uniform(-0.5,0.5)
    axisX=max(4.0, axisX+epsilon)
    ind.listCavities[index][2]=axisX

cpdef void mutation_axisY(Individual ind):

    length=len(ind.listCavities)

    if length==0:
        raise ValueError, "their is no cavity to mutate"

    index=random.randint(0,length-1)

    cdef double axisY=ind.listCavities[index][3]

    cdef double epsilon=random.uniform(-0.5,0.5)
    axisY=max(0.5, axisY+epsilon)
    ind.listCavities[index][3]=axisY


cpdef void mutation_axisZ(Individual ind):

    length=len(ind.listCavities)
    print length
    if length<=1:
        raise ValueError, "their is no cavity to mutate, or don't touch the first cavity"

    index=random.randint(1,length-1)

    cdef double axisZ=ind.listCavities[index][4]

    cdef double epsilon=random.uniform(-0.2,0.2)
    axisZ=max(0.5,min(1.5, axisZ+epsilon))
    ind.listCavities[index][4]=axisZ


cpdef void mutation_type(Individual ind):

    length=len(ind.listCavities)

    if length==0:
        raise ValueError, "their is no cavity to mutate"

    index=random.randint(0,length-1)

    cdef str type=ind.listCavities[index][1]

    if type=="ellipsoid":
        ind.listCavities[index][1]="frisbee"
    else:
        ind.listCavities[index][1]="ellipsoid"


cpdef void mutation(Individual ind):

    if mutationAxisX=="ON":
        mutation_axisX(ind)

    if mutationAxisY=="ON":
        mutation_axisY(ind)

    if mutationAxisZ=="ON":
        mutation_axisZ(ind)

    if mutationType=="ON":
        mutation_type(ind)

cpdef void mutation_Pop(int number_of_mutated_ind, int number_of_mutation_per_ind):

    length_temp=len(POPULATION)

    cdef int i,j,k

    cdef Individual ind

    for i in range(number_of_mutated_ind):
        j=random.randint(0,length_temp-1)
        ind=copyInd(POPULATION[j])

        for k in range(number_of_mutation_per_ind):
            mutation(ind)

        POPULATION.add_pop(ind)


###
###CROSSING
###

cpdef void crossing_ind(Individual individual1, Individual individual2):

    cdef Individual ind1=copyInd(individual1)
    cdef Individual ind2=copyInd(individual2)

    cdef int index=random.randint(0,number_of_cavities-1)

    temp=ind1.listCavities[index]
    print temp
    ind1.listCavities[index]=ind2.listCavities[index]
    ind2.listCavities[index]=temp

    POPULATION.add_pop(ind1)
    POPULATION.add_pop(ind2)

cpdef void crossing_Pop(int number_of_crossing):

    cdef int length_temp=len(POPULATION)

    cdef int i,j,k

    for i in range(number_of_crossing):
        j=random.randint(0,length_temp-1)
        k=random.randint(0,length_temp-1)
        crossing_ind(POPULATION[j],POPULATION[k])


cpdef void evaluation(Individual ind):

    cdef primitives.Shape shape, shapeInt

    (shape, shapeInt)=accordionFreeDimension(ind)

    shape_to_export=shape



