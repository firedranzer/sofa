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


import accordion
import primitives

import math
from math import sqrt


def writeHeadLine():

    with open("litteral.pyx", "w") as litteral_expression:

        temp="""#!/usr/bin/env python
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

import numpy
import math
from math import cos, sin, sqrt
from libc.math cimport sin, cos, acos, exp, sqrt, fabs, M_PI
cimport numpy
cimport cython
cimport primitives

cdef double x=1.0
cdef double y=2.0
cdef double z=3.0


"""
        litteral_expression.write(temp)
        litteral_expression.close()


def litteralExpression(shape):

    writeHeadLine()

    with open("litteral.pyx", "a") as litteral_expression:

        litteral_expression.write(primitives.sign())
        litteral_expression.write(primitives.ind())

        litteral_expression.write("\n\n\n\n\n")

        litteral_expression.write("#LISTE DES PARAMETRES DES PRIMITIVES\n\n")

        listPrimitives=primitives.getListPrimitives()
#        listPrimitives.sort()

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
        (expression,(gradX,gradY,gradZ))=shape.toString()
        litteral_expression.write("#LISTE DES CALCULS PRELIMINAIRES DE RACINES\n\n\n")

        listSqrt=primitives.getListSqrt()

        for sqrt in listSqrt:
            litteral_expression.write(sqrt)

        litteral_expression.write("#LISTE DES EXPRESSIONS INTERMEDIAIRES\n\n")



        (expression,(gradX,gradY,gradZ))=shape.toString()

        listA,listB=primitives.getListAandB()
        length=len(listA)

        if len(listA)!=len(listB):
            raise ValueError, "check something"

        for j in range(length):
            litteral_expression.write(listA[j]+"\n\n")
            litteral_expression.write(listB[j]+"\n\n\n")

        litteral_expression.write("#Litteral expression  of the IMPLICIT FIELD is \n"+"expression="+expression+"\n\n\n")







        listgradientDxPrimitives=primitives.getListgradientDxPrimitives()

#        listgradientDxPrimitives.sort()

        litteral_expression.write("#LISTE DES GRADIENTS DES PRIMITIVES SELON X\n\n\n")

        for gradx in listgradientDxPrimitives:
            litteral_expression.write(gradx)

        litteral_expression.write("#LISTE DES GRADIENTS INTERMEDIAIRES SELON X\n\n\n")
        listgradientXTemp=primitives.getListgradientXTemp()
#        listgradientXTemp.sort()
        for gradx in listgradientXTemp:
                litteral_expression.write(gradx+"\n\n")


        litteral_expression.write("#Litteral expression  of the  GRADX is \n"+"gradX="+gradX+"\n\n\n")

        listgradientDyPrimitives=primitives.getListgradientDyPrimitives()
#        listgradientDyPrimitives.sort()
        litteral_expression.write("#LISTE DES GRADIENTS DES PRIMITIVES SELON Y\n\n\n")

        for grady in listgradientDyPrimitives:
            litteral_expression.write(grady)

        litteral_expression.write("#LISTE DES GRADIENTS INTERMEDIAIRES SELON Y\n\n\n")
        listgradientYTemp=primitives.getListgradientYTemp()
#        listgradientYTemp.sort()
        for grady in listgradientYTemp:
            litteral_expression.write(grady+"\n\n")

        litteral_expression.write("#Litteral expression  of the  GRADY is gradY\n"+"gradY="+gradY+"\n\n\n")


        listgradientDzPrimitives=primitives.getListgradientDzPrimitives()
#        listgradientDzPrimitives.sort()

        litteral_expression.write("#LISTE DES GRADIENTS DES PRIMITIVES SELON Z\n\n\n")

        for gradz in listgradientDzPrimitives:
            litteral_expression.write(gradz)

        litteral_expression.write("#LISTE DES GRADIENTS INTERMEDIAIRES SELON Z\n\n\n")

        listgradientZTemp=primitives.getListgradientZTemp()
#        listgradientZTemp.sort()


        for gradz in listgradientZTemp:
            litteral_expression.write(gradz+"\n\n")

        litteral_expression.write("#Litteral expression  of the  GRADZ is\n"+"gradZ="+gradZ+"\n\n\n")
        litteral_expression.write("print 'expression='+str(expression)"+"\n")
        litteral_expression.write("print 'gradX='+str(gradX)"+"\n")
        litteral_expression.write("print 'gradY='+str(gradY)"+"\n")
        litteral_expression.write("print 'gradZ='+str(gradZ)"+"\n")
        litteral_expression.close()



ellipsoid1=primitives.Ellipsoid("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
ellipsoid2=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(0.0,0.0,0.0))
ellipsoid3=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(5.5,0.0,0.0))

cylinder1=primitives.Cylinder("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
frisbee1=primitives.Cylinder("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))


union=primitives.Union(ellipsoid1,primitives.Intersection(ellipsoid2,cylinder1))

#expressionUnion=union.toString()


#with open("litteral.pyx", "a") as litteral_expression:

#    litteral_expression.write("#expressionUnion is "+expressionUnion+"\n\n\n")
#    litteral_expression.write("#gradxUnion is "+gradxUnion+"\n\n\n")
#    litteral_expression.write("#gradyUnion is "+gradyUnion+"\n\n\n")
#    litteral_expression.write("#gradzUnion is "+gradzUnion+"\n\n\n")
#    litteral_expression.close()




niceaccordion=accordion.accordionUniform(10.0,2.0,0.1,"frisbee",10,4.0,4.0,4.5)

litteralExpression(niceaccordion)










