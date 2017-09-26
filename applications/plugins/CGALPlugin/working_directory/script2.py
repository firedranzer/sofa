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


import geneticAlgorithm

#geneticAlgorithm.generate_initPop(1000)


POPULATION=geneticAlgorithm.getPOPULATION()

#geneticAlgorithm.popDisplay(POPULATION)
#print str(individual.Level)


individual1=geneticAlgorithm.getNewInd("frisbee")

#print individual1.display()

#geneticAlgorithm.mutation_axisX(individual1)
#print individual1.display()

#geneticAlgorithm.mutation_axisY(individual1)
#print individual1.display()

#geneticAlgorithm.mutation_axisZ(individual1)
#print individual1.display()

#geneticAlgorithm.mutation_type(individual1)
#print individual1.display()

individual2=geneticAlgorithm.getNewInd("ellipsoid")

#ind1, ind2= geneticAlgorithm.crossing_ind(individual1,individual2)

print "CROSSING"
print "individual1"
print individual1.display()
print "individual2"
print individual2.display()

print "ind1"
print ind1.display()
print "ind2"
print ind2.display()


print "individual1"
print individual1.display()
print "individual2"
print individual2.display()
