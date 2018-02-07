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

# distutils: language=c++
# cython: profile=True

import numpy
import math

from libc.math cimport sin, cos, sqrt


cdef int i=0

cpdef str generateNewSubIndex():
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

    cpdef ListOfPrimitives plus(self, ListOfPrimitives listOfPrimitives):

        newList=ListOfPrimitives()
        newList.listPrimitives=self.listPrimitives + listOfPrimitives.listPrimitives
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

cdef class Point2D(object):

    def __init__(self, x, y):
        self.x=x
        self.y=y

    cdef display(self):
        print "("+str(self.x),str(self.y)+")"
        return

def equal (p1, p2):
    return p1.x == p2.x and p1.y == p2.y

cdef class Vector2D(object):

    def __init__(self, first, second):
        self.first=first
        self.second=second

    cpdef double firstCoord(self):

        return self.second.x-self.first.x

    cpdef double secondCoord(self):

        return self.second.y-self.first.y

cpdef double norm(Vector2D u):
    return sqrt(u.firstCoord()**2 + u.secondCoord()**2)

cpdef double norm2(Vector2D u):
    return (u.firstCoord()**2 + u.secondCoord()**2)

cpdef double Det(Vector2D u, Vector2D v):
    return u.firstCoord()*v.secondCoord()-u.secondCoord()*v.firstCoord()

cpdef double scal(Vector2D u, Vector2D v):
    return u.firstCoord()*v.firstCoord() + u.secondCoord()*v.secondCoord()

cdef Point2D translationRotation(double sinTheta, double cosTheta, Point2D center, Point2D point):

    cdef double x
    cdef double y

    x=cosTheta*(point.x-center.x)+sinTheta*(point.y-center.y)
    y=-sinTheta*(point.x-center.x)+cosTheta*(point.y-center.y)

    point=Point2D(x,y)

    return point


cdef class Shape2D(object):

    def __init__(self):


        self.subIndex=generateNewSubIndex()
        self.index=self.subIndex
        self.type

    cdef double eval(self, Point2D point):

        cdef double eval = self.eval(point)
        return eval

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

cdef class All(Shape2D):

    def __init__(self):
        Shape2D.__init__(self)
        self.type = "All"

    cdef double eval(self,Point2D point):
        return -1.0

cdef class NoWhere(Shape2D):

    def __init__(self):
        Shape2D.__init__(self)
        self.type = "NoWhere"

    cdef double eval(self,Point2D point):
        return 1.0

cdef class Union(Shape2D):

    def __init__(self, Shape2D first, Shape2D second):
        Shape2D.__init__(self)
        self.first=first
        self.second=second
        self.type = "Union"

    cdef double eval(self,Point2D point):

        cdef double eval = min(self.first.eval(point),self.second.eval(point))
        return eval


    cpdef tuple toString(self):

        expression="min(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")"

        gradX="(1.0-ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+"))*gradA"+self.index+"_"+self.subIndex+"X"+"+"+"ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")*gradB"+self.index+"_"+self.subIndex+"X"
        gradY="(1.0-ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+"))*gradA"+self.index+"_"+self.subIndex+"Y"+"+"+"ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")*gradB"+self.index+"_"+self.subIndex+"Y"

        return (expression,(gradX,gradY))


    cpdef str toWriting(self):

        writing="primitives2D.Union(WritingA"+self.index+"_"+self.subIndex+",WritingB"+self.index+"_"+self.subIndex+")"

        return writing


    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.first.getListOfPrimitives().plus(self.second.getListOfPrimitives())

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=self.first.getListOfLitteralExpressions().plus(self.second.getListOfLitteralExpressions())

        (expression1,(grad1X,grad1Y))=self.first.toString()
        (expression2,(grad2X,grad2Y))=self.second.toString()

        listOfLitteralExpressions.listA.append("A"+self.index+"_"+self.subIndex+"="+expression1)
        listOfLitteralExpressions.listB.append("B"+self.index+"_"+self.subIndex+"="+expression2)

        listOfLitteralExpressions.listgradientXTemp.append("gradA"+self.index+"_"+self.subIndex+"X="+grad1X)
        listOfLitteralExpressions.listgradientYTemp.append("gradA"+self.index+"_"+self.subIndex+"Y="+grad1Y)

        listOfLitteralExpressions.listgradientXTemp.append("gradB"+self.index+"_"+self.subIndex+"X="+grad2X)
        listOfLitteralExpressions.listgradientYTemp.append("gradB"+self.index+"_"+self.subIndex+"Y="+grad2Y)

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=self.first.getListForWriting().plus(self.second.getListForWriting())

        writing1=self.first.toWriting()
        writing2=self.second.toWriting()

        listForWriting.listWritingA.append("WritingA"+self.index+"_"+self.subIndex+"="+writing1)
        listForWriting.listWritingB.append("WritingB"+self.index+"_"+self.subIndex+"="+writing2)
        return listForWriting



cdef class Intersection(Shape2D):

    def __init__(self, Shape2D first, Shape2D second):
        Shape2D.__init__(self)
        self.first=first
        self.second=second
        self.type = "Intersection"

    cdef double eval(self,Point2D point):

        cdef double eval = max(self.first.eval(point),self.second.eval(point))
        return eval

    cpdef tuple toString(self):

        expression="max(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")"

        gradX="ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")*gradA"+self.index+"_"+self.subIndex+"X"+"+"+"(1.0-ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+"))*gradB"+self.index+"_"+self.subIndex+"X"
        gradY="ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+")*gradA"+self.index+"_"+self.subIndex+"Y"+"+"+"(1.0-ind(A"+self.index+"_"+self.subIndex+",B"+self.index+"_"+self.subIndex+"))*gradB"+self.index+"_"+self.subIndex+"Y"

        return (expression,(gradX,gradY))


    cpdef str toWriting(self):

        writing="primitives2D.Intersection(WritingA"+self.index+"_"+self.subIndex+",WritingB"+self.index+"_"+self.subIndex+")"

        return writing

    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.first.getListOfPrimitives().plus(self.second.getListOfPrimitives())

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=self.first.getListOfLitteralExpressions().plus(self.second.getListOfLitteralExpressions())

        (expression1,(grad1X,grad1Y))=self.first.toString()
        (expression2,(grad2X,grad2Y))=self.second.toString()

        listOfLitteralExpressions.listA.append("A"+self.index+"_"+self.subIndex+"="+expression1)
        listOfLitteralExpressions.listB.append("B"+self.index+"_"+self.subIndex+"="+expression2)

        listOfLitteralExpressions.listgradientXTemp.append("gradA"+self.index+"_"+self.subIndex+"X="+grad1X)
        listOfLitteralExpressions.listgradientYTemp.append("gradA"+self.index+"_"+self.subIndex+"Y="+grad1Y)

        listOfLitteralExpressions.listgradientXTemp.append("gradB"+self.index+"_"+self.subIndex+"X="+grad2X)
        listOfLitteralExpressions.listgradientYTemp.append("gradB"+self.index+"_"+self.subIndex+"Y="+grad2Y)

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=self.first.getListForWriting().plus(self.second.getListForWriting())

        writing1=self.first.toWriting()
        writing2=self.second.toWriting()

        listForWriting.listWritingA.append("WritingA"+self.index+"_"+self.subIndex+"="+writing1)
        listForWriting.listWritingB.append("WritingB"+self.index+"_"+self.subIndex+"="+writing2)
        return listForWriting


cdef class Difference(Shape2D):

    def __init__(self, first, second):
        Shape2D.__init__(self)
        self.first=first
        self.second=second
        self.type = "Difference"

    cdef double eval(self,Point2D point):

        cdef double eval = max(self.first.eval(point),-self.second.eval(point))
        return eval


    cpdef tuple toString(self):

        expression="max(A"+self.index+"_"+self.subIndex+",-B"+self.index+"_"+self.subIndex+")"

        gradX="ind(A"+self.index+"_"+self.subIndex+",-B"+self.index+"_"+self.subIndex+")*gradA"+self.index+"_"+self.subIndex+"X"+"+"+"(1.0-ind(A"+self.index+"_"+self.subIndex+",-B"+self.index+"_"+self.subIndex+"))*(-gradB"+self.index+"_"+self.subIndex+"X"+")"
        gradY="ind(A"+self.index+"_"+self.subIndex+",-B"+self.index+"_"+self.subIndex+")*gradA"+self.index+"_"+self.subIndex+"Y"+"+"+"(1.0-ind(A"+self.index+"_"+self.subIndex+",-B"+self.index+"_"+self.subIndex+"))*(-gradB"+self.index+"_"+self.subIndex+"Y"+")"

        return (expression,(gradX,gradY))


    cpdef str toWriting(self):

        writing="primitives.Difference(WritingA"+self.index+"_"+self.subIndex+",WritingB"+self.index+"_"+self.subIndex+")"

        return writing


    cpdef ListOfPrimitives getListOfPrimitives(self):
        return self.first.getListOfPrimitives().plus(self.second.getListOfPrimitives())

    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=self.first.getListOfLitteralExpressions().plus(self.second.getListOfLitteralExpressions())

        (expression1,(grad1X,grad1Y,grad1Z))=self.first.toString()
        (expression2,(grad2X,grad2Y,grad2Z))=self.second.toString()

        listOfLitteralExpressions.listA.append("A"+self.index+"_"+self.subIndex+"="+expression1)
        listOfLitteralExpressions.listB.append("B"+self.index+"_"+self.subIndex+"="+expression2)

        listOfLitteralExpressions.listgradientXTemp.append("gradA"+self.index+"_"+self.subIndex+"X="+grad1X)
        listOfLitteralExpressions.listgradientYTemp.append("gradA"+self.index+"_"+self.subIndex+"Y="+grad1Y)
        listOfLitteralExpressions.listgradientZTemp.append("gradA"+self.index+"_"+self.subIndex+"Z="+grad1Z)

        listOfLitteralExpressions.listgradientXTemp.append("gradB"+self.index+"_"+self.subIndex+"X="+grad2X)
        listOfLitteralExpressions.listgradientYTemp.append("gradB"+self.index+"_"+self.subIndex+"Y="+grad2Y)
        listOfLitteralExpressions.listgradientZTemp.append("gradB"+self.index+"_"+self.subIndex+"Z="+grad2Z)

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=self.first.getListForWriting().plus(self.second.getListForWriting())

        writing1=self.first.toWriting()
        writing2=self.second.toWriting()

        listForWriting.listWritingA.append("WritingA"+self.index+"_"+self.subIndex+"="+writing1)
        listForWriting.listWritingB.append("WritingB"+self.index+"_"+self.subIndex+"="+writing2)

        return listForWriting



cdef class Primitives2D(Shape2D):

    def __init__(self):
        Shape2D.__init__(self)
        self.type
        self.identifier

    cdef double eval (self, Point2D point):
        return self.eval(point)



    cpdef tuple toString(self):

        return self.toString()

    cpdef str toWriting(self):

        return self.toWriting()



    cpdef ListOfPrimitives getListOfPrimitives(self):

        cdef ListOfPrimitives listOfPrimitives=ListOfPrimitives()

        listOfPrimitives.listPrimitives.append(self.identifier)

        return listOfPrimitives



    cpdef ListOfLitteralExpressions getListOfLitteralExpressions(self):

        cdef ListOfLitteralExpressions listOfLitteralExpressions=ListOfLitteralExpressions()

        return listOfLitteralExpressions

    cpdef ListForWriting getListForWriting(self):

        cdef ListForWriting listForWriting=ListForWriting()

        return listForWriting


cdef class Ellipse(Primitives2D):

    def __init__(self, axisX, axisY, theta=0.0, center=Point2D(0.0,0.0)):

        Primitives2D.__init__(self)
        self.type="Ellipse"
        self.axisX= axisX
        self.axisY= axisY

        self.theta=theta

        self.cosTheta=cos(theta)
        self.sinTheta=sin(theta)

        self.center=center

        x="cosTheta"+self.index+"_"+self.subIndex+"*(x-center"+self.index+"_"+self.subIndex+"x)-sinTheta"+self.index+"_"+self.subIndex+"*(y-center"+self.index+"_"+self.subIndex+"y)"
        y="sinTheta"+self.index+"_"+self.subIndex+"*(x-center"+self.index+"_"+self.subIndex+"x)+cosTheta"+self.index+"_"+self.subIndex+"*(y-center"+self.index+"_"+self.subIndex+"y)"

        self.coord=(x,y)

        identifier=[self.type, self.axisX,self.axisY,self.theta,(self.center.x,self.center.y),self.coord,self.index, self.subIndex]
        self.identifier=identifier

    cdef double eval(self,Point2D point):

        cdef Point2D point_temp=translationRotation(self.sinTheta, self.cosTheta, self.center, point)

        cdef double eval = (point_temp.x/self.axisX)**2+(point_temp.y/self.axisY)**2-1.0

        return eval

    cpdef tuple toString(self):

        (x,y)=("x"+self.index,"y"+self.index)
        (dxX,dxY)=("dxX"+self.index,"dxY"+self.index)

        gradX="("+dxX+"*2.0*"+x+"/axis"+self.index+"_"+self.subIndex+"X*axis"+self.index+"X)"+"+"+"("+dxY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"


        (dyX,dyY)=("dyX"+self.index,"dyY"+self.index)

        gradY="("+dyX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dyY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"

        (dzX,dzY)=("dzX"+self.index,"dzY"+self.index)

        gradZ="("+dzX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dzY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"

        grad=(gradX,gradY,gradZ)

        expression="("+x+"/axis"+self.index+"X)*("+x+"/axis"+self.index+"X)"+"+"+"("+y+"/axis"+self.index+"Y)*("+y+"/axis"+self.index+"Y)-1.0"

        return (expression, grad)

    cpdef str toWriting(self):

        return "Ellipse"+self.index+"_"+self.subIndex

cdef class HalfPlaneGivenByAVector2D(Shape2D):

    """
    The half_plane will be on the right of the vector
    """

    def __init__(self, Vector2D vect):

        Shape2D.__init__(self)
        self.type="Half-Plane"

        self.vect = vect

        cdef Point2D center = vect.first
        self.center = center

        cdef double l = sqrt(vect.firstCoord() * vect.firstCoord()\
                     + vect.secondCoord() * vect.secondCoord())
        if l==0.0:
            raise ValueError, "the vector has length 0.0"

        self.sinTheta = vect.secondCoord()/l
        self.cosTheta = vect.secondCoord()/l

        self.identifier = [self.type, self.vect, self.index, self.subIndex]


    cdef double eval(self,Point2D point):

        cdef Point2D point_temp = translationRotation(self.sinTheta, self.cosTheta, self.center, point)

        return point_temp.y

    cpdef tuple toString(self):

        (x,y)=("x"+self.index,"y"+self.index)

        gradX="dxY"+self.index
        gradY="dyY"+self.index
        gradZ="dzY"+self.index
        grad=(gradX,gradY,gradZ)

        expression=y

        return (expression, grad)

    cpdef str toWriting(self):
        return "HalfPlane"+self.index+"_"+self.subIndex

cpdef list createPolygoalChain(list listOfCouplesxy):

    cdef list newList=[]
    cdef int n=len(listOfCouplesxy)
    cdef int i
    cdef Vector2D u

    if n<2:
        raise ValueError, "I need at least two points!"

    listOfCouplesxy.append(listOfCouplesxy[0])

    for i in range(n):
        point1=Point2D(listOfCouplesxy[i][0],listOfCouplesxy[i][1])
        point2=Point2D(listOfCouplesxy[i+1][0],listOfCouplesxy[i+1][1])

        u=Vector2D(point1,point2)

        newList.append(u)

    return newList

