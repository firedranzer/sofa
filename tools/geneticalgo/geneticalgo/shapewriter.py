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
import primitives
import primitives2D

def getDefaultHeader():
    temp="""#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - thomas.morzadec@inria.fr
##     - damien.marchal@univ-lille1.fr
####################################################################################################
import primitives
"""
    return temp

def toPythonString(shape):
    temp=getDefaultHeader()
    temp+="#LISTE DES PARAMETRES DES PRIMITIVES\n\n"

    listPrimitives=shape.getListOfPrimitives().listPrimitives


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

            prm=prm+"Cylinder"+index+"=primitives.Cylinder(sign"+str(index)+",axis"+str(index)+"X,axis"+str(index)+"Y,axis"\
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

                prm=prm+"Ellipsoid"+index+"=primitives.Ellipsoid("+"sign"+str(index)+",axis"+str(index)+"X,axis"+str(index)+"Y,axis"\
                        +str(index)+"Z,theta"+str(index)+",phi"+str(index)\
                        +",(center"+str(index)+"x,center"+str(index)+"y,center"+str(index)+"z))\n\n\n"

            elif type=="frisbee":

                prm=prm+"Frisbee"+index+"=primitives.Frisbee("+"sign"+str(index)+",axis"+str(index)+"X,axis"+str(index)+"Y,axis"\
                            +str(index)+"Z,theta"+str(index)+",phi"+str(index)\
                            +",(center"+str(index)+"x,center"+str(index)+"y,center"+str(index)+"z))\n\n\n"



        temp+=prm

    temp+="\n\n\n\n\n"

    temp+="#LISTE DES EXPRESSIONS INTERMEDIAIRES\n\n"

    listWritingA,listWritingB=shape.getListForWriting().listWritingA, shape.getListForWriting().listWritingB

    length=len(listWritingA)

    if len(listWritingA)!=len(listWritingB):
        raise ValueError, "check something"

    for j in range(length):

        temp+=listWritingA[j]+"\n\n"
        temp+=listWritingB[j]+"\n\n\n"


    temp+="#Expression  of the IMPLICIT FIELD is \n"+"expression="+shape.toWriting()+"\n\n\n"

    return temp


def writeInFile(ind, filename):
    shapeWritten=toPythonString(ind)

    with open(filename, "w") as litteral_expression:
        litteral_expression.write(shapeWritten)
        litteral_expression.close()



################################################################################################################################################
#
#               TESTS
#
############################################################################################################################################

if __name__ == "__main__":

    import math
    from math import sqrt

    ellipsoid1=primitives.Ellipsoid("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
    ellipsoid2=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(0.0,0.0,0.0))
    ellipsoid3=primitives.Ellipsoid("+",1.0,1.0,1.0,0.0,0.0,primitives.Point(5.5,0.0,0.0))

    cylinder1=primitives.Cylinder("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))
    cylinder2=primitives.Cylinder("+",1.0,2.0,2.0,0.0,0.0,primitives.Point(1.0,0.0,0.0))
    frisbee1=primitives.Frisbee("+",1.0,2.0,2.0,math.pi/4.0,math.pi/2.0,primitives.Point(1.0,0.0,0.0))

    ind=primitives.Difference(primitives.Intersection(primitives.Union(primitives.Union(primitives.Union(ellipsoid1,ellipsoid2), ellipsoid3), cylinder1), cylinder2), frisbee1)

#    writeInFile(ind,"essai.pyx")




import primitives2D




square=primitives2D.closedPolygonalChain("0",[(0.0,0.0),(0.0,1.0),(1.0,1.0),(1.0,0.0)])

print square.eval(primitives2D.Point2D(0.5,0.5))

cube=primitives.ExtrusionOfShape2D(square,1.0, 0.0,0.0,primitives.Point(0.0,0.0,0.0))



print ""+str(cube.eval(primitives.Point(0.5,0.5,0.5)))


print ""+str(cube.eval(primitives.Point(1.5,0.5,0.5)))


print ""+str(cube.eval(primitives.Point(1.0,0.5,0.5)))


print ""+str(cube.eval(primitives.Point(0.7,0.7,0.2)))




ellipse=primitives2D.Ellipse("0", 1.0,1.0)

print ellipse.eval(primitives2D.Point2D(0.5,0.0))

ellipsoid=primitives.ExtrusionOfShape2D(ellipse,1.0, 0.0,0.0,primitives.Point(0.0,0.0,0.0))

print ellipsoid.eval(primitives.Point(0.5,0.0,0.5))

print ""+str(ellipsoid.eval(primitives.Point(0.5,0.5,0.5)))


print ""+str(ellipsoid.eval(primitives.Point(1.5,0.5,0.5)))


print ""+str(ellipsoid.eval(primitives.Point(1.0,0.5,0.5)))


print ""+str(ellipsoid.eval(primitives.Point(0.7,0.7,0.2)))
