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

import numpy
import math
import primitives2D
from libc.math cimport sin, cos, acos, exp, sqrt, fabs, M_PI
cimport numpy
cimport cython
cimport primitives
cimport primitives2D

cdef i=0

cpdef str generateNewIndex():
    global i
    cdef str index=str(i)
    i+=1
    return index

cpdef void clearOut():
    global i
    i=0

cdef class ListOfLitteralExpressions(object):


    def __init__(self):

        self.listA=[]
        self.listB=[]

        self.listgradientXTemp=[]
        self.listgradientYTemp=[]
        self.listgradientZTemp=[]
        self.listSqrt=[]


    cpdef ListOfLitteralExpressions plus(self, ListOfLitteralExpressions listOfLitteralExpressions):

        newList=ListOfLitteralExpressions()
        newList.listA=self.listA + listOfLitteralExpressions.listA
        newList.listB=self.listB + listOfLitteralExpressions.listB

        newList.listgradientXTemp=self.listgradientXTemp + listOfLitteralExpressions.listgradientXTemp
        newList.listgradientYTemp=self.listgradientYTemp + listOfLitteralExpressions.listgradientYTemp
        newList.listgradientZTemp=self.listgradientZTemp + listOfLitteralExpressions.listgradientZTemp
        newList.listSqrt=self.listSqrt + listOfLitteralExpressions.listSqrt

        return newList


cdef class ListOfPrimitives(object):

    def __init__(self):

        self.listPrimitives=[]
        self.listgradientDxPrimitives=[]
        self.listgradientDyPrimitives=[]
        self.listgradientDzPrimitives=[]

    cpdef ListOfPrimitives plus(self, ListOfPrimitives listOfPrimitives):

        newList=ListOfPrimitives()
        newList.listPrimitives=self.listPrimitives + listOfPrimitives.listPrimitives
        newList.listgradientDxPrimitives=self.listgradientDxPrimitives + listOfPrimitives.listgradientDxPrimitives
        newList.listgradientDyPrimitives=self.listgradientDyPrimitives + listOfPrimitives.listgradientDyPrimitives
        newList.listgradientDzPrimitives=self.listgradientDzPrimitives + listOfPrimitives.listgradientDzPrimitives

        return newList

cdef class ListForWriting(object):

    def __init__(self):

        self.listWritingA=[]
        self.listWritingB=[]

    cpdef ListForWriting plus(self, ListForWriting listForWriting):

            newList=ListForWriting()
            newList.listWritingA=self.listWritingA + listForWriting.listWritingA
            newList.listWritingB=self.listWritingB + listForWriting.listWritingB

            return newList


cpdef str ind():

    temp="cdef double ind(double a, double b):\n"\
         +"     if a>b:\n"\
         +"         return 1.0\n\n"\
         +"     elif a<b:\n"\
         +"         return 0.0\n\n"\
         +"     else:\n"\
         +"         return 0.5\n\n"

    return temp


cpdef str sign():

    temp="cdef double sign(double x):\n"\
         +"     if x>0.0:\n"\
         +"         return 1.0\n\n"\
         +"     else:\n"\
         +"         return -1.0\n\n"

    return temp


cpdef str minus(str expression, double x):
    if x==0.0:
        return expression
    else:
        return "("+expression+"-"+str(x)+")"

cpdef str divide(str expression, double x):
    if x==1.0:
        return expression
    else:
        return "("+expression+"/"+str(x)+")"

cpdef str multiply(str expression1,str expression2):

    if expression1=="0.0" or expression2=="0.0" or expression1=="-0.0" or expression2=="-0.0" or expression1=="" or expression2=="":

            return ""

    elif expression1=="1.0":

        return expression2

    elif expression2=="1.0":

        return expression1

    else:

        return "("+expression1+"*"+expression2+")"

cpdef str add(str expression1,str expression2):

    if expression1=="" or expression1=="0.0":

        if expression2!="0.0":
            return expression2
        else:
            return ""

    else:

        if expression2=="0.0" or expression2=="":
            return expression1
        else:
            return "("+expression1+"+"+expression2+")"

cdef class Point(object):

    def __init__(self, x, y, z):
        self.x=x
        self.y=y
        self.z=z

    cpdef display(self):
        print "("+str(self.x),str(self.y),str(self.z)+")"
        return

    cdef primitives2D.Point2D projectTo2D(self):

        cdef primitives2D.Point2D point2D=primitives2D.Point2D(self.x,self.y)

        return point2D

cdef class Shape(object):

    def __init__(self):

        self.index=generateNewIndex()

    cpdef double eval(self, Point point):
        return self.eval(point)

    cpdef tuple toString(self):
        return self.toString()

    cpdef str toWriting(self):
        return self.toWriting()

    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.getListOfPrimitives()

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):
        return self.getListOfLitteralExpressions()


    cpdef ListForWriting getListForWriting(self):
        return self.getListForWriting()

    def geometric_transformation(self, type, **kwargs):
        self.geometric_transformation(type, kwargs)

cdef class Union(Shape):

    def __init__(self, first, second):
        Shape.__init__(self)
        self.first=first
        self.second=second

    cpdef double eval(self,Point point):
        return min(self.first.eval(point),self.second.eval(point))


    cpdef tuple toString(self):

        expression="min(A"+self.index+",B"+self.index+")"

        gradX="(1.0-ind(A"+self.index+",B"+self.index+"))*gradA"+self.index+"X"+"+"+"ind(A"+self.index+",B"+self.index+")*gradB"+self.index+"X"
        gradY="(1.0-ind(A"+self.index+",B"+self.index+"))*gradA"+self.index+"Y"+"+"+"ind(A"+self.index+",B"+self.index+")*gradB"+self.index+"Y"
        gradZ="(1.0-ind(A"+self.index+",B"+self.index+"))*gradA"+self.index+"Z"+"+"+"ind(A"+self.index+",B"+self.index+")*gradB"+self.index+"Z"

        return (expression,(gradX,gradY,gradZ))


    cpdef str toWriting(self):

        writing="primitives.Union(WritingA"+self.index+",WritingB"+self.index+")"

        return writing

    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.first.getListOfPrimitives().plus(self.second.getListOfPrimitives())

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=self.first.getListOfLitteralExpressions().plus(self.second.getListOfLitteralExpressions())

        (expression1,(grad1X,grad1Y,grad1Z))=self.first.toString()
        (expression2,(grad2X,grad2Y,grad2Z))=self.second.toString()

        listOfLitteralExpressions.listA.append("A"+self.index+"="+expression1)
        listOfLitteralExpressions.listB.append("B"+self.index+"="+expression2)

        listOfLitteralExpressions.listgradientXTemp.append("gradA"+self.index+"X="+grad1X)
        listOfLitteralExpressions.listgradientYTemp.append("gradA"+self.index+"Y="+grad1Y)
        listOfLitteralExpressions.listgradientZTemp.append("gradA"+self.index+"Z="+grad1Z)

        listOfLitteralExpressions.listgradientXTemp.append("gradB"+self.index+"X="+grad2X)
        listOfLitteralExpressions.listgradientYTemp.append("gradB"+self.index+"Y="+grad2Y)
        listOfLitteralExpressions.listgradientZTemp.append("gradB"+self.index+"Z="+grad2Z)

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=self.first.getListForWriting().plus(self.second.getListForWriting())

        writing1=self.first.toWriting()
        writing2=self.second.toWriting()

        listForWriting.listWritingA.append("WritingA"+self.index+"="+writing1)
        listForWriting.listWritingB.append("WritingB"+self.index+"="+writing2)

        return listForWriting

    def geometric_transformation(self, type, **kwargs):
        self.first.geometric_transformation(type, kwargs)
        self.second.geometric_transformation(type, kwargs)


cdef class Intersection(Shape):

    def __init__(self, first, second):
        Shape.__init__(self)
        self.first=first
        self.second=second

    cpdef double eval(self,Point point):
        return max(self.first.eval(point),self.second.eval(point))

    cpdef tuple toString(self):

        expression="max(A"+self.index+",B"+self.index+")"

        gradX="ind(A"+self.index+",B"+self.index+")*gradA"+self.index+"X"+"+"+"(1.0-ind(A"+self.index+",B"+self.index+"))*gradB"+self.index+"X"
        gradY="ind(A"+self.index+",B"+self.index+")*gradA"+self.index+"Y"+"+"+"(1.0-ind(A"+self.index+",B"+self.index+"))*gradB"+self.index+"Y"
        gradZ="ind(A"+self.index+",B"+self.index+")*gradA"+self.index+"Z"+"+"+"(1.0-ind(A"+self.index+",B"+self.index+"))*gradB"+self.index+"Z"

        return (expression,(gradX,gradY,gradZ))


    cpdef str toWriting(self):

        writing="primitives.Intersection(WritingA"+self.index+",WritingB"+self.index+")"

        return writing

    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.first.getListOfPrimitives().plus(self.second.getListOfPrimitives())

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=self.first.getListOfLitteralExpressions().plus(self.second.getListOfLitteralExpressions())

        (expression1,(grad1X,grad1Y,grad1Z))=self.first.toString()
        (expression2,(grad2X,grad2Y,grad2Z))=self.second.toString()

        listOfLitteralExpressions.listA.append("A"+self.index+"="+expression1)
        listOfLitteralExpressions.listB.append("B"+self.index+"="+expression2)

        listOfLitteralExpressions.listgradientXTemp.append("gradA"+self.index+"X="+grad1X)
        listOfLitteralExpressions.listgradientYTemp.append("gradA"+self.index+"Y="+grad1Y)
        listOfLitteralExpressions.listgradientZTemp.append("gradA"+self.index+"Z="+grad1Z)

        listOfLitteralExpressions.listgradientXTemp.append("gradB"+self.index+"X="+grad2X)
        listOfLitteralExpressions.listgradientYTemp.append("gradB"+self.index+"Y="+grad2Y)
        listOfLitteralExpressions.listgradientZTemp.append("gradB"+self.index+"Z="+grad2Z)

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=self.first.getListForWriting().plus(self.second.getListForWriting())

        writing1=self.first.toWriting()
        writing2=self.second.toWriting()

        listForWriting.listWritingA.append("WritingA"+self.index+"="+writing1)
        listForWriting.listWritingB.append("WritingB"+self.index+"="+writing2)

        return listForWriting


    def geometric_transformation(self, type, **kwargs):
        self.first.geometric_transformation(type, kwargs)
        self.second.geometric_transformation(type, kwargs)

cdef class Difference(Shape):

    def __init__(self, first, second):
        Shape.__init__(self)
        self.first=first
        self.second=second

    cpdef double eval(self,Point point):
        return max(self.first.eval(point),-self.second.eval(point))


    cpdef tuple toString(self):

        expression="max(A"+self.index+",-B"+self.index+")"

        gradX="ind(A"+self.index+",-B"+self.index+")*gradA"+self.index+"X"+"+"+"(1.0-ind(A"+self.index+",-B"+self.index+"))*(-gradB"+self.index+"X"+")"
        gradY="ind(A"+self.index+",-B"+self.index+")*gradA"+self.index+"Y"+"+"+"(1.0-ind(A"+self.index+",-B"+self.index+"))*(-gradB"+self.index+"Y"+")"
        gradZ="ind(A"+self.index+",-B"+self.index+")*gradA"+self.index+"Z"+"+"+"(1.0-ind(A"+self.index+",-B"+self.index+"))*(-gradB"+self.index+"Z"+")"

        return (expression,(gradX,gradY,gradZ))


    cpdef str toWriting(self):

        writing="primitives.Difference(WritingA"+self.index+",WritingB"+self.index+")"

        return writing


    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.first.getListOfPrimitives().plus(self.second.getListOfPrimitives())

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=self.first.getListOfLitteralExpressions().plus(self.second.getListOfLitteralExpressions())

        (expression1,(grad1X,grad1Y,grad1Z))=self.first.toString()
        (expression2,(grad2X,grad2Y,grad2Z))=self.second.toString()

        listOfLitteralExpressions.listA.append("A"+self.index+"="+expression1)
        listOfLitteralExpressions.listB.append("B"+self.index+"="+expression2)

        listOfLitteralExpressions.listgradientXTemp.append("gradA"+self.index+"X="+grad1X)
        listOfLitteralExpressions.listgradientYTemp.append("gradA"+self.index+"Y="+grad1Y)
        listOfLitteralExpressions.listgradientZTemp.append("gradA"+self.index+"Z="+grad1Z)

        listOfLitteralExpressions.listgradientXTemp.append("gradB"+self.index+"X="+grad2X)
        listOfLitteralExpressions.listgradientYTemp.append("gradB"+self.index+"Y="+grad2Y)
        listOfLitteralExpressions.listgradientZTemp.append("gradB"+self.index+"Z="+grad2Z)

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=self.first.getListForWriting().plus(self.second.getListForWriting())

        writing1=self.first.toWriting()
        writing2=self.second.toWriting()

        listForWriting.listWritingA.append("WritingA"+self.index+"="+writing1)
        listForWriting.listWritingB.append("WritingB"+self.index+"="+writing2)

        return listForWriting

    def geometric_transformation(self, type, **kwargs):
        self.first.geometric_transformation(type, kwargs)
        self.second.geometric_transformation(type, kwargs)


cdef class Primitives(Shape):

    def __init__(self, sign, axisX, axisY, axisZ, double theta, double phi, Point center):
        Shape.__init__(self)
        self.sign=sign
        self.axisX=axisX
        self.axisY=axisY
        self.axisZ=axisZ
        self.theta=theta
        self.phi=phi

        self.cosTheta=cos(theta)
        self.sinTheta=sin(theta)
        self.cosPhi=cos(phi)
        self.sinPhi=sin(phi)

        self.center=center
        self.type
        self.identifier=[]

#        cpdef translationRotationToString(self):

        #        if self.cosTheta==0.0:

        #            x1="(-"+minus("y",self.center.y)+")"
        #            y=minus("x",self.center.x)


        #        elif self.cosTheta==1.0:

        #            x1=minus("x",self.center.x)
        #            y=minus("y",self.center.y)


        #        else:

        x1="cosTheta"+self.index+"*(x-center"+self.index+"x)+sinTheta"+self.index+"*(y-center"+self.index+"y)"
        y="-sinTheta"+self.index+"*(x-center"+self.index+"x)+cosTheta"+self.index+"*(y-center"+self.index+"y)"

        #        if self.cosPhi==0.0:

        #           x="(-"+minus("z",self.center.z)+")"
        #           z=x1

        #        elif self.cosPhi==1.0:

        #            x=x1
        #            z=minus("z",self.center.z)

        #        else:

        x="cosPhi"+self.index+"*"+x1+"+sinPhi"+self.index+"*(z-center"+self.index+"z)"
        z="-sinPhi"+self.index+"*"+x1+"+cosPhi"+self.index+"*(z-center"+self.index+"z)"

#        listCoordPrimitives.append("x"+self.index+"="+x+"\n"+"y"+self.index+"="+y+"\n"+"z"+self.index+"="+z+"\n\n\n")
        #        print self.index+"      "+"("+x+","+y+","+z+")"
#            return ("x"+self.index,"y"+self.index,"z"+self.index)

        self.coord=(x,y,z)

#        cpdef gradXtranslationRotationToString(self):

        #        (dxX,dxY,dxZ)=(multiply(str(self.cosPhi),str(self.cosTheta)), str(self.sinTheta),multiply(str(self.sinPhi),str(self.cosTheta)))



    cpdef ListOfPrimitives getListOfPrimitives(self):

        cdef ListOfPrimitives listOfPrimitives=ListOfPrimitives()

        listOfPrimitives.listPrimitives.append(self.identifier)

        (dxX,dxY,dxZ)=("cosPhi"+self.index+"*"+"cosTheta"+self.index, "sinTheta"+self.index,"sinPhi"+self.index+"*"+"cosTheta"+self.index)

        listOfPrimitives.listgradientDxPrimitives.append("dxX"+self.index+"="+dxX+"\n"+"dxY"+self.index+"="+dxY+"\n"+"dxZ"+self.index+"="+dxZ+"\n\n\n")

        (dyX,dyY,dyZ)=("(-cosPhi"+self.index+")*sinTheta"+self.index, "cosTheta"+self.index,"-sinPhi"+self.index+"*"+"sinTheta"+self.index)

        listOfPrimitives.listgradientDyPrimitives.append("dyX"+self.index+"="+dyX+"\n"+"dyY"+self.index+"="+dyY+"\n"+"dyZ"+self.index+"="+dyZ+"\n\n\n")

        (dzX,dzY,dzZ)=("sinPhi"+self.index, str(0.0),"cosPhi"+self.index)

        listOfPrimitives.listgradientDzPrimitives.append("dzX"+self.index+"="+dzX+"\n"+"dzY"+self.index+"="+dzY+"\n"+"dzZ"+self.index+"="+dzZ+"\n\n\n")

        return listOfPrimitives



    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=ListOfLitteralExpressions()

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=ListForWriting()

        return listForWriting

    cdef translationRotation(self,Point point):

        cdef double x
        cdef double y
        cdef double z

        x=self.cosTheta*(point.x-self.center.x)+self.sinTheta*(point.y-self.center.y)
        y=-self.sinTheta*(point.x-self.center.x)+self.cosTheta*(point.y-self.center.y)

        x=self.cosPhi*point.x+self.sinPhi*(point.z-self.center.z)
        z=-self.sinPhi*point.x+self.cosPhi*(point.z-self.center.z)
        point=Point(x,y,z)
        return point

    def geometric_transformation(self, type, **kwargs):

        if type=="translation":
            (x,y,z)=kwargs.get("vect",None)
            self.center.x+=x
            self.center.y+=y
            self.center.z+=z


        elif type=="rotationAroundAxisZ":
            centerRotation=kwargs.get("center", None)
            theta=kwargs.get("theta", None)

            x=centerRotation.x+(cos(theta)*(self.center.x-centerRotation.x)-sin(theta)*(self.center.y-centerRotation.y))
            y=centerRotation.y+(sin(theta)*(self.center.x-centerRotation.x)+cos(theta)*(self.center.y-centerRotation.y))


            self.center.x=x
            self.center.y=y

            self.theta+=theta


        elif type=="rotationAroundXYPlane":

            centerRotation=kwargs.get("center", None)
            phi=kwargs.get("phi", None)

            l=sqrt((self.center.x-centerRotation.x)**2+(self.center.y-centerRotation.y)**2)

            cosTheta_temp=(self.center.x-centerRotation.x)/l
            sinTheta_temp=(self.center.y-centerRotation.y)/l

            X=cos(phi)*l-sin(phi)*(self.center.z-centerRotation.z)
            z=sin(phi)*l+cos(phi)*(self.center.z-centerRotation.z)

            self.center.x=centerRotation.x + X*cosTheta_temp
            self.center.y=centerRotation.y + X*sinTheta_temp
            self.center.z+=z

            self.phi+=phi


cdef class Ellipsoid(Primitives):


    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):

        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="ellipsoid"
        identifier=[self.type,self.sign, self.axisX,self.axisY,self.axisZ,self.theta,self.phi,(self.center.x,self.center.y,self.center.z),self.coord,self.index]
        self.identifier=identifier    

    cpdef double eval(self,Point point):

        point=self.translationRotation(point)

        return (point.x/self.axisX)**2+(point.y/self.axisY)**2+(point.z/self.axisZ)**2-1.0

    cpdef tuple toString(self):

        (x,y,z)=("x"+self.index,"y"+self.index,"z"+self.index)#self.translationRotationToString()
        (dxX,dxY,dxZ)=("dxX"+self.index,"dxY"+self.index,"dxZ"+self.index)#self.gradXtranslationRotationToString()

#        add(add(multiply(dxX,divide("2.0*"+x,self.axisX**2)),multiply(dxY,divide("2.0*"+y,self.axisY**2))),\
#        multiply(dxZ,divide("2.0*"+z,self.axisZ**2)))

        gradX="("+dxX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dxY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"+"+"+\
                "("+dxZ+"*2.0*"+z+"/axis"+self.index+"Z*axis"+self.index+"Z)"


        (dyX,dyY,dyZ)=("dyX"+self.index,"dyY"+self.index,"dyZ"+self.index)#self.gradYtranslationRotationToString()

#         add(add(multiply(dyX,divide("2.0*"+x,self.axisX**2)),multiply(dyY,divide("2.0*"+y,self.axisY**2))),\
#         multiply(dyZ,divide("2.0*"+z,self.axisZ**2)))

        gradY="("+dyX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dyY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"+"+"+\
                "("+dyZ+"*2.0*"+z+"/axis"+self.index+"Z*axis"+self.index+"Z)"

        (dzX,dzY,dzZ)=("dzX"+self.index,"dzY"+self.index,"dzZ"+self.index)#self.gradZtranslationRotationToString()

#        add(add(multiply(dzX,divide("2.0*"+x,self.axisX**2)),multiply(dzY,divide("2.0*"+y,self.axisY**2))),\
#        multiply(dzZ,divide("2.0*"+z,self.axisZ**2)))

        gradZ="("+dzX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dzY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"+"+"+\
            "("+dzZ+"*2.0*"+z+"/axis"+self.index+"Z*axis"+self.index+"Z)"

        grad=(gradX,gradY,gradZ)

        expression="("+x+"/axis"+self.index+"X)*("+x+"/axis"+self.index+"X)"+"+"+"("+y+"/axis"+self.index+"Y)*("+y+"/axis"+self.index+"Y)"+"+"\
                  +"("+z+"/axis"+self.index+"Z)*("+z+"/axis"+self.index+"Z)-1.0"


#        divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+divide(y,self.axisY)+"*"+divide(y,self.axisY)+"+"+divide(z,self.axisZ)+"*"+\
#        divide(z,self.axisZ)+"-1.0"

        return (expression, grad)

    cpdef str toWriting(self):

        return "Ellipsoid"+self.index

cdef class Frisbee(Primitives):

    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):
        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="frisbee"
        identifier=[self.type,self.sign, self.axisX,self.axisY,self.axisZ,self.theta,self.phi,(self.center.x,self.center.y,self.center.z),self.coord,self.index]
        self.identifier=identifier

    cpdef double eval(self,Point point):

        point=self.translationRotation(point)

        return abs(point.z/self.axisZ)+math.sqrt((point.x/self.axisX)**2+(point.y/self.axisY)**2)-1


    cpdef tuple toString(self):

        (x,y,z)=("x"+self.index,"y"+self.index,"z"+self.index)#self.translationRotationToString()

        (dxX,dxY,dxZ)=("dxX"+self.index,"dxY"+self.index,"dxZ"+self.index)#self.gradXtranslationRotationToString()

        gradX=dxX+"*("+x+"/sqrt"+self.index+")"\
              +"+"+dxY+"*("+y+"/sqrt"+self.index+")"\
              +"+"+dxZ+"*"+"sign("+z+")/axis"+self.index+"Z"

        (dyX,dyY,dyZ)=("dyX"+self.index,"dyY"+self.index,"dyZ"+self.index)#self.gradYtranslationRotationToString()

        gradY=dyX+"*("+x+"/list"+self.index+")"\
              +"+"+dyY+"*("+y+"/sqrt"+self.index+")"\
              +"+"+dyZ+"*"+"sign("+z+")/axis"+self.index+"Z"

        (dzX,dzY,dzZ)=("dzX"+self.index,"dzY"+self.index,"dzZ"+self.index)#self.gradZtranslationRotationToString()

        gradZ=dzX+"*("+x+"/sqrt"+self.index+")"\
              +"+"+dzY+"*("+y+"/sqrt"+self.index+")"\
              +"+"+dzZ+"*"+"sign("+z+")/axis"+self.index+"Z"

        grad=(gradX,gradY,gradZ)

        expression="abs("+z+"/axis"+self.index+"Z)"+"+"\
                    +"sqrt"+self.index+"-1.0"


        return (expression, grad)

    cpdef str toWriting(self):

        return "Frisbee"+self.index

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=ListOfLitteralExpressions()

        (x,y,z)=("x"+self.index,"y"+self.index,"z"+self.index)
        sqrtTemp="sqrt(("+x+"/axis"+self.index+"X)"+"*"+"("+x+"/axis"+self.index+"X)"+"+"+"("+y+"/axis"+self.index+"Y)"+"*"+"("+y+"/axis"+self.index+"Y)"+")"
        print "SqrtTemp="+sqrtTemp
        listOfLitteralExpressions.listSqrt.append("sqrt"+self.index+"="+sqrtTemp)

        return listOfLitteralExpressions


cdef class Cylinder(Primitives):


    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):
        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="cylinder"
#        (x,y,z)=("x"+self.index,"y"+self.index,"z"+self.index)#self.translationRotationToString()

        self.radial="(x"+self.index+"/axis"+self.index+"X)"+"*"+"(x"+self.index+"/axis"+self.index+"X)"+"+"+"(y"+self.index+"/axis"+self.index+"Y)"+"*"+"(y"+self.index+"/axis"+self.index+"Y)"+"-1.0"
        self.height="abs(z"+self.index+"-axis"+self.index+"Z)"
        identifier=[self.type,self.sign, self.axisX,self.axisY,self.axisZ,self.theta,self.phi,(self.center.x,self.center.y,self.center.z),self.coord,self.radial,self.height,self.index]
        self.identifier=identifier

    cpdef double eval(self,Point point):
        point=self.translationRotation(point)

        return max(abs(point.z)-self.axisZ,(point.x/self.axisX)**2+(point.y/self.axisY)**2-1)


    cpdef tuple toString(self):

        (x,y,z)=("x"+self.index,"y"+self.index,"z"+self.index)#self.translationRotationToString()
        (dxX,dxY,dxZ)=("dxX"+self.index,"dxY"+self.index,"dxZ"+self.index)#self.gradXtranslationRotationToString()

        gradX=dxX+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+x+"/(axis"+self.index+"X"+"*"+"axis"+self.index+"X)+"\
              +dxY+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+y+"/(axis"+self.index+"Y"+"*"+"axis"+self.index+"Y)+"\
              +dxZ+"*ind(height"+self.index+",radial"+self.index+")*sign("+z+")"

        (dyX,dyY,dyZ)=("dyX"+self.index,"dyY"+self.index,"dyZ"+self.index)#self.gradYtranslationRotationToString()

        gradY=dyX+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+x+"/(axis"+self.index+"X"+"*"+"axis"+self.index+"X)+"\
              +dyY+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+y+"/(axis"+self.index+"Y"+"*"+"axis"+self.index+"Y)+"\
              +dyZ+"*ind(height"+self.index+",radial"+self.index+")*sign("+z+")"

        (dzX,dzY,dzZ)=("dzX"+self.index,"dzY"+self.index,"dzZ"+self.index)#self.gradZtranslationRotationToString()

        gradZ=dzX+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+x+"/(axis"+self.index+"X"+"*"+"axis"+self.index+"X)+"\
              +dzY+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+y+"/(axis"+self.index+"Y"+"*"+"axis"+self.index+"Y)+"\
              +dzZ+"*ind(height"+self.index+",radial"+self.index+")*sign("+z+")"

        grad=(gradX,gradY,gradZ)


        expression="max(height"+self.index+",radial"+self.index+")"

        return (expression, grad)


    cpdef str toWriting(self):
        return "Cylinder"+self.index


cdef class ExtrusionOfShape2D(Shape):

        def __init__(self, primitives2D.Shape2D shape2D, double heigth, double theta, double phi, Point center):
            Shape.__init__(self)
            self.shape2D=shape2D
            self.heigth=heigth
            self.theta=theta
            self.phi=phi

            self.cosTheta=cos(theta)
            self.sinTheta=sin(theta)
            self.cosPhi=cos(phi)
            self.sinPhi=sin(phi)

            self.center=center
            self.type="extrusionOfShape2D"

            identifier=[self.type, self.theta,self.phi,(self.center.x,self.center.y,self.center.z),self.index]
            self.identifier=identifier


            x1="cosTheta"+self.index+"*(x-center"+self.index+"x)-sinTheta"+self.index+"*(y-center"+self.index+"y)"
            y="sinTheta"+self.index+"*(x-center"+self.index+"x)+cosTheta"+self.index+"*(y-center"+self.index+"y)"

            x="cosPhi"+self.index+"*"+x1+"-sinPhi"+self.index+"*(z-center"+self.index+"z)"
            z="sinPhi"+self.index+"*"+x1+"+cosPhi"+self.index+"*(z-center"+self.index+"z)"

            self.coord=(x,y,z)


        cpdef ListOfPrimitives getListOfPrimitives(self):

            cdef ListOfPrimitives listOfPrimitives=ListOfPrimitives()

            listOfPrimitives.listPrimitives.append(self.identifier)

            (dxX,dxY,dxZ)=("cosPhi"+self.index+"*"+"cosTheta"+self.index, "sinTheta"+self.index,"sinPhi"+self.index+"*"+"cosTheta"+self.index)

            listOfPrimitives.listgradientDxPrimitives.append("dxX"+self.index+"="+dxX+"\n"+"dxY"+self.index+"="+dxY+"\n"+"dxZ"+self.index+"="+dxZ+"\n\n\n")

            (dyX,dyY,dyZ)=("(-cosPhi"+self.index+")*sinTheta"+self.index, "cosTheta"+self.index,"-sinPhi"+self.index+"*"+"sinTheta"+self.index)

            listOfPrimitives.listgradientDyPrimitives.append("dyX"+self.index+"="+dyX+"\n"+"dyY"+self.index+"="+dyY+"\n"+"dyZ"+self.index+"="+dyZ+"\n\n\n")

            (dzX,dzY,dzZ)=("sinPhi"+self.index, str(0.0),"cosPhi"+self.index)

            listOfPrimitives.listgradientDzPrimitives.append("dzX"+self.index+"="+dzX+"\n"+"dzY"+self.index+"="+dzY+"\n"+"dzZ"+self.index+"="+dzZ+"\n\n\n")

            return listOfPrimitives



        cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

            cdef ListOfLitteralExpressions listOfLitteralExpressions=ListOfLitteralExpressions()

            return listOfLitteralExpressions

        cpdef ListForWriting getListForWriting(self):

            cdef ListForWriting listForWriting=ListForWriting()

            return listForWriting

        cdef translationRotation(self,Point point):

            cdef double x
            cdef double y
            cdef double z

            x=self.cosTheta*(point.x-self.center.x)-self.sinTheta*(point.y-self.center.y)
            y=self.sinTheta*(point.x-self.center.x)+self.cosTheta*(point.y-self.center.y)

            x=self.cosPhi*point.x-self.sinPhi*(point.z-self.center.z)
            z=self.sinPhi*point.x+self.cosPhi*(point.z-self.center.z)
            point=Point(x,y,z)
            return point


        cpdef double eval(self,Point point):

            point=self.translationRotation(point)

            return max(abs(point.z-self.heigth/2.0)-self.heigth/2.0,self.shape2D.eval(point.projectTo2D()))


        def geometric_transformation(self, type, **kwargs):

            if type=="translation":
                (x,y,z)=kwargs.get("vect",None)
                self.center.x+=x
                self.center.y+=y
                self.center.z+=z


            elif type=="rotationAroundAxisZ":
                centerRotation=kwargs.get("center", None)
                theta=kwargs.get("theta", None)

                x=centerRotation.x+(cos(theta)*(self.center.x-centerRotation.x)-sin(theta)*(self.center.y-centerRotation.y))
                y=centerRotation.y+(sin(theta)*(self.center.x-centerRotation.x)+cos(theta)*(self.center.y-centerRotation.y))


                self.center.x=x
                self.center.y=y

                self.theta+=theta


            elif type=="rotationAroundXYPlane":

                centerRotation=kwargs.get("center", None)
                phi=kwargs.get("phi", None)

                l=sqrt((self.center.x-centerRotation.x)**2+(self.center.y-centerRotation.y)**2)

                cosTheta_temp=(self.center.x-centerRotation.x)/l
                sinTheta_temp=(self.center.y-centerRotation.y)/l

                X=cos(phi)*l-sin(phi)*(self.center.z-centerRotation.z)
                z=sin(phi)*l+cos(phi)*(self.center.z-centerRotation.z)

                self.center.x=centerRotation.x + X*cosTheta_temp
                self.center.y=centerRotation.y + X*sinTheta_temp
                self.center.z+=z

                self.phi+=phi
