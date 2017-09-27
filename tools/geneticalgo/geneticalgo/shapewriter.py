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

import os
import math
from math import sqrt
import primitives

def writeHeadLine():

    temp="""#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - thomas.morzadec@inria.fr
##     - damien.marchal@inria.fr
####################################################################################################
# distutils: language=c++
# cython: profile=True

import primitives
cimport numpy
cimport cython
cimport primitives


"""


    return temp


def toPythonString(shape):
    return "print('HELLO WORLD')"


def write(ind):

    temp=writeHeadLine()

    temp+="#LISTE DES PARAMETRES DES PRIMITIVES\n\n"

    listPrimitives=ind.listOfPrimitives.listPrimitives

    for identifier in listPrimitives:
        type=identifier[0]
        index=identifier[-1]

        if type=="cylinder":

            [type,sign, axisX,axisY,axisZ,theta,phi,center,(x,y,z),radial,height,index]=identifier

            prm="#############\n"+"##PRIMITIVE   "+str(index)+"\n"+"############\n\n\n"\
                +"type"+str(index)+"='"+type+"'\n"\
                +"sign"+str(index)+"='"+sign+"'\n"\
                +"axis"+str(index)+"X="+str(axisX)+"\n"\
                +"axis"+str(index)+"Y="+str(axisY)+"\n"\
                +"axis"+str(index)+"Z="+str(axisZ)+"\n"\
                +"theta"+str(index)+"="+str(theta)+"\n"\
                +"phi"+str(index)+"="+str(phi)+"\n"\
                +"(center"+str(index)+"x,center"+str(index)+"y,center"+str(index)+"z)="+str(center)+"\n\n"

            prm=prm+"Cylinder"+index+"=primitives.Cylinder("+"sign,axis"+str(index)+"X,axis"+str(index)+"Y,axis"\
                   +str(index)+"Z,theta"+str(index)+",phi"+str(index)\
                   +",(center"+str(index)+"x,center"+str(index)+"y,center"+str(index)+"z))\n\n\n"

        else:

            [type,sign, axisX,axisY,axisZ,theta,phi,center,(x,y,z),index]=identifier

            prm="#############\n"+"##PRIMITIVE   "+str(index)+"\n"+"############\n\n\n"\
            +"type"+str(index)+"='"+type+"'\n"\
            +"sign"+str(index)+"='"+sign+"'\n"\
            +"axis"+str(index)+"X="+str(axisX)+"\n"\
            +"axis"+str(index)+"Y="+str(axisY)+"\n"\
            +"axis"+str(index)+"Z="+str(axisZ)+"\n"\
            +"theta"+str(index)+"="+str(theta)+"\n"\
            +"phi"+str(index)+"="+str(phi)+"\n"\
            +"(center"+str(index)+"x,center"+str(index)+"y,center"+str(index)+"z)="+str(center)+"\n\n"


            if type=="ellipsoid":

                prm=prm+"Ellipsoid"+index+"=primitives.Ellipsoid("+"sign,axis"+str(index)+"X,axis"+str(index)+"Y,axis"\
                        +str(index)+"Z,theta"+str(index)+",phi"+str(index)\
                        +",(center"+str(index)+"x,center"+str(index)+"y,center"+str(index)+"z))\n\n\n"

            elif type=="frisbee":

                prm=prm+"Frisbee"+index+"=primitives.Frisbee("+"sign,axis"+str(index)+"X,axis"+str(index)+"Y,axis"\
                            +str(index)+"Z,theta"+str(index)+",phi"+str(index)\
                            +",(center"+str(index)+"x,center"+str(index)+"y,center"+str(index)+"z))\n\n\n"



        temp+=prm


    temp+="\n\n\n\n\n"

    write=ind.toWriting()


    temp+="#LISTE DES EXPRESSIONS INTERMEDIAIRES\n\n"

    listWritingA,listWritingB=ind.listForWriting.listWritingA, ind.listForWriting.listWritingB

    length=len(listWritingA)

    if len(listWritingA)!=len(listWritingB):
        raise ValueError, "check something"

    for j in range(length):

        temp+=listWritingA[j]+"\n\n"
        temp+=listWritingB[j]+"\n\n\n"

    temp+="#Expression  of the IMPLICIT FIELD is \n"+"expression="+write+"\n\n\n"

    return temp


def writeInFile(ind, filename):

    shapeWritten=write(ind)

    with open(filename, "w") as litteral_expression:
        litteral_expression.write(shapeWritten)
        litteral_expression.close()



################################################################################################################################################
#
#               TESTS
#
############################################################################################################################################


ellipsoid1=primitives.Ellipsoid("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
ellipsoid2=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(0.0,0.0,0.0))
ellipsoid3=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(5.5,0.0,0.0))

cylinder1=primitives.Cylinder("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
cylinder2=primitives.Cylinder("+",1.0,2.0,2.0,0.0,0.0,primitives.Point(1.0,0.0,0.0))
frisbee1=primitives.Frisbee("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))


ind=primitives.Difference(primitives.Intersection(primitives.Union(primitives.Union(primitives.Union(ellipsoid1,ellipsoid2), ellipsoid3), cylinder1), cylinder2), frisbee1)

writeInFile(ind,"EssaiMaxime.pyx")
