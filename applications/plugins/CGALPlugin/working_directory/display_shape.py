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


def writeHeadLine(i):

    with open("display"+str(i)+".pyx", "w") as display:

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
        display.write(temp)
        display.close()


def display(shape,i):

    writeHeadLine(i)

    with open("display"+str(i)+".pyx", "a") as doc:

        doc.write("#LISTE DES PARAMETRES DES PRIMITIVES\n\n")

        listPrimitives=primitives.getListPrimitives()
#        listPrimitives.sort()

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



            doc.write(prm)


        doc.write("\n\n\n\n\n")

        (expression,(gradX,gradY,gradZ), display)=shape.toString()


        doc.write("#LISTE DES EXPRESSIONS INTERMEDIAIRES\n\n")

        listDisplayA,listDisplayB=primitives.getListDisplayAandB()

        length=len(listDisplayA)

        if len(listDisplayA)!=len(listDisplayB):
            raise ValueError, "check something"

        for j in range(length):
            doc.write(listDisplayA[j]+"\n\n")
            doc.write(listDisplayB[j]+"\n\n\n")

        doc.write("#Expression  of the IMPLICIT FIELD is \n"+"expression="+display+"\n\n\n")
