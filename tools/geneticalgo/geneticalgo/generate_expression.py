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

def writeHeadLine():


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

    return temp


def litteralExpression(shape):

    temp=writeHeadLine()

    temp+=primitives.sign()
    temp+=primitives.ind()

    temp+="\n\n\n\n\n"

    temp+="#LISTE DES PARAMETRES DES PRIMITIVES\n\n"

    listPrimitives=shape.getListOfPrimitives().listPrimitives

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



        temp+=prm


        temp+="\n\n\n\n\n"



        (expression,(gradX,gradY,gradZ))=shape.toString()



        temp+="#LISTE DES CALCULS PRELIMINAIRES DE RACINES\n\n\n"

        listSqrt=shape.getListOfLitteralExpressions().listSqrt

        for sqrt in listSqrt:
            temp+=sqrt

        temp+="#LISTE DES EXPRESSIONS INTERMEDIAIRES\n\n"

        listA,listB=shape.getListOfLitteralExpressions().listA, shape.getListOfLitteralExpressions().listB

        length=len(listA)

        if len(listA)!=len(listB):
            raise ValueError, "check something"

        for j in range(length):
            temp+=listA[j]+"\n\n"
            temp+=listB[j]+"\n\n\n"

        temp+="#Litteral expression  of the IMPLICIT FIELD is \n"+"expression="+expression+"\n\n\n"

        listgradientDxPrimitives=shape.getListOfPrimitives().listgradientDxPrimitives


        temp+="#LISTE DES GRADIENTS DES PRIMITIVES SELON X\n\n\n"

        for gradx in listgradientDxPrimitives:
            temp+=gradx

        temp+="#LISTE DES GRADIENTS INTERMEDIAIRES SELON X\n\n\n"
        listgradientXTemp=shape.getListOfLitteralExpressions().listgradientXTemp

        for gradx in listgradientXTemp:
            temp+=gradx+"\n\n"


        temp+="#Litteral expression  of the  GRADX is \n"+"gradX="+gradX+"\n\n\n"

        listgradientDyPrimitives=shape.getListOfPrimitives().listgradientDyPrimitives

        temp+="#LISTE DES GRADIENTS DES PRIMITIVES SELON Y\n\n\n"

        for grady in listgradientDyPrimitives:
            temp+=grady

        temp+="#LISTE DES GRADIENTS INTERMEDIAIRES SELON Y\n\n\n"
        listgradientYTemp=shape.getListOfLitteralExpressions().listgradientYTemp

        for grady in listgradientYTemp:
            temp+=grady+"\n\n"

        temp+="#Litteral expression  of the  GRADY is gradY\n"+"gradY="+gradY+"\n\n\n"


        listgradientDzPrimitives=shape.getListOfPrimitives().listgradientDzPrimitives

        temp+="#LISTE DES GRADIENTS DES PRIMITIVES SELON Z\n\n\n"

        for gradz in listgradientDzPrimitives:
            temp+=gradz

        temp+="#LISTE DES GRADIENTS INTERMEDIAIRES SELON Z\n\n\n"

        listgradientZTemp=shape.getListOfLitteralExpressions().listgradientZTemp

        for gradz in listgradientZTemp:
            temp+=gradz+"\n\n"

        temp+="#Litteral expression  of the  GRADZ is\n"+"gradZ="+gradZ+"\n\n\n"

        return temp


def writeLitteralExpressionInFile(shape, filename):

    shapeLitteralExpression=litteralExpression(shape)

    with open(filename, "w") as litteral_expression:
        litteral_expression.write(shapeLitteralExpression)
        litteral_expression.close()

#
#TEST
#

if __name__ == '__main__':

    import math

    ellipsoid1=primitives.Ellipsoid("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
    ellipsoid2=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(0.0,0.0,0.0))
    ellipsoid3=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(5.5,0.0,0.0))

    cylinder1=primitives.Cylinder("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
    cylinder2=primitives.Cylinder("+",1.0,2.0,2.0,0.0,0.0,primitives.Point(1.0,0.0,0.0))
    frisbee1=primitives.Frisbee("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))


    ind=primitives.Difference(primitives.Intersection(primitives.Union(primitives.Union(primitives.Union(ellipsoid1,ellipsoid2), ellipsoid3), cylinder1), cylinder2), frisbee1)

    writeLitteralExpressionInFile(ind, "essai.pyx")









