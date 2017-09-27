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

import primitives

import math
from math import sqrt

def writeHeadLine(name):

    with open(name, "w") as litteral_expression:

        temp="""#!/usr/bin/env python
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

import numpy
import math
from math import cos, sin, sqrt
from libc.math cimport sin, cos, acos, exp, sqrt, fabs, M_PI
cimport numpy
cimport cython
cimport primitives

"""
        litteral_expression.write(temp)
        litteral_expression.close()


def litteralExpression(ind, filename):

    writeHeadLine(filename)

    with open(filename, "a") as litteral_expression:

        litteral_expression.write(primitives.sign())
        litteral_expression.write(primitives.ind())

        litteral_expression.write("\n\n\n\n\n")

        litteral_expression.write("#LISTE DES PARAMETRES DES PRIMITIVES\n\n")

        listPrimitives=ind.listOfPrimitives.listPrimitives

        for identifier in listPrimitives:

            type=identifier[0]

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
                    +"cosTheta"+str(index)+"=cos("+str(theta)+")\n"\
                    +"cosPhi"+str(index)+"=cos("+str(phi)+")\n"\
                    +"sinTheta"+str(index)+"=sin("+str(theta)+")\n"\
                    +"sinPhi"+str(index)+"=sin("+str(phi)+")\n"\
                    +"(center"+str(index)+"x,center"+str(index)+"y,center"+str(index)+"z)="+str(center)+"\n\n"\
                    +"(x"+str(index)+",y"+str(index)+",z"+str(index)+")=("+x+","+y+","+z+")\n\n\n"\
                    +"radial"+str(index)+"="+str(radial)+"\n\n"\
                    +"height"+str(index)+"="+str(height)+"\n\n"\
                    +"\n\n\n\n"

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
                     +"cosTheta"+str(index)+"=cos("+str(theta)+")\n"\
                     +"cosPhi"+str(index)+"=cos("+str(phi)+")\n"\
                     +"sinTheta"+str(index)+"=sin("+str(theta)+")\n"\
                     +"sinPhi"+str(index)+"=sin("+str(phi)+")\n"\
                     +"(center"+str(index)+"x,center"+str(index)+"y,center"+str(index)+"z)="+str(center)+"\n\n"\
                     +"(x"+str(index)+",y"+str(index)+",z"+str(index)+")=("+x+","+y+","+z+")"\
                     +"\n\n\n\n"



            litteral_expression.write(prm)


        litteral_expression.write("\n\n\n\n\n")



        (expression,(gradX,gradY,gradZ))=ind.toString()



        litteral_expression.write("#LISTE DES CALCULS PRELIMINAIRES DE RACINES\n\n\n")

        listSqrt=ind.listOfLitteralExpressions.listSqrt

        for sqrt in listSqrt:
            litteral_expression.write(sqrt)

        litteral_expression.write("#LISTE DES EXPRESSIONS INTERMEDIAIRES\n\n")

        listA,listB=ind.listOfLitteralExpressions.listA, ind.listOfLitteralExpressions.listB

        length=len(listA)

        if len(listA)!=len(listB):
            raise ValueError, "check something"

        for j in range(length):
            litteral_expression.write(listA[j]+"\n\n")
            litteral_expression.write(listB[j]+"\n\n\n")

        litteral_expression.write("#Litteral expression  of the IMPLICIT FIELD is \n"+"expression="+expression+"\n\n\n")

        listgradientDxPrimitives=ind.listOfPrimitives.listgradientDxPrimitives

#        listgradientDxPrimitives.sort()

        litteral_expression.write("#LISTE DES GRADIENTS DES PRIMITIVES SELON X\n\n\n")

        for gradx in listgradientDxPrimitives:
            litteral_expression.write(gradx)

        litteral_expression.write("#LISTE DES GRADIENTS INTERMEDIAIRES SELON X\n\n\n")
        listgradientXTemp=ind.listOfLitteralExpressions.listgradientXTemp

#        listgradientXTemp.sort()
        for gradx in listgradientXTemp:
                litteral_expression.write(gradx+"\n\n")


        litteral_expression.write("#Litteral expression  of the  GRADX is \n"+"gradX="+gradX+"\n\n\n")

        listgradientDyPrimitives=ind.listOfPrimitives.listgradientDyPrimitives
#        listgradientDyPrimitives.sort()
        litteral_expression.write("#LISTE DES GRADIENTS DES PRIMITIVES SELON Y\n\n\n")

        for grady in listgradientDyPrimitives:
            litteral_expression.write(grady)

        litteral_expression.write("#LISTE DES GRADIENTS INTERMEDIAIRES SELON Y\n\n\n")
        listgradientYTemp=ind.listOfLitteralExpressions.listgradientYTemp

#        listgradientYTemp.sort()
        for grady in listgradientYTemp:
            litteral_expression.write(grady+"\n\n")

        litteral_expression.write("#Litteral expression  of the  GRADY is gradY\n"+"gradY="+gradY+"\n\n\n")


        listgradientDzPrimitives=ind.listOfPrimitives.listgradientDzPrimitives
#        listgradientDzPrimitives.sort()

        litteral_expression.write("#LISTE DES GRADIENTS DES PRIMITIVES SELON Z\n\n\n")

        for gradz in listgradientDzPrimitives:
            litteral_expression.write(gradz)

        litteral_expression.write("#LISTE DES GRADIENTS INTERMEDIAIRES SELON Z\n\n\n")

        listgradientZTemp=ind.listOfLitteralExpressions.listgradientZTemp
#        listgradientZTemp.sort()


        for gradz in listgradientZTemp:
            litteral_expression.write(gradz+"\n\n")

        litteral_expression.write("#Litteral expression  of the  GRADZ is\n"+"gradZ="+gradZ+"\n\n\n")



#
#TEST
#

ellipsoid1=primitives.Ellipsoid("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
ellipsoid2=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(0.0,0.0,0.0))
ellipsoid3=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(5.5,0.0,0.0))

cylinder1=primitives.Cylinder("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
cylinder2=primitives.Cylinder("+",1.0,2.0,2.0,0.0,0.0,primitives.Point(1.0,0.0,0.0))
frisbee1=primitives.Frisbee("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))


ind=primitives.Difference(primitives.Intersection(primitives.Union(primitives.Union(primitives.Union(ellipsoid1,ellipsoid2), ellipsoid3), cylinder1), cylinder2), frisbee1)

litteralExpression(ind, "EssaiMaxime.pyx")









