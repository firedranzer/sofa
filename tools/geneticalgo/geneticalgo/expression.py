cdef primitives2D.Point2D projectTo2D(self):

    cdef primitives2D.Point2D point2D=primitives2D.Point2D(self.x,self.y)

    return point2D









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
import primitives2D
import primitives
import expression2D

class Expression(object):

    def toString():

       return ""

class ExpressionBinary(Expression):

    def __init__(self,type, left, right):

        self.type = type
        self.left = left
        self.right = right

    def toString():

       return ""

class ExpressionUnary(Expression):

    def __init__(self, type, middle):

        self.type = type
        self.middle = middle

class ExpressionConst(Expression):

    def __init__(self, type,  const):

        self.type = type
        self.const = const



def GetXYZ(shape)

    theta=shape.theta
    phi=shape.phi
    center=shape.center


    A = ExpressionBinary("*",\
    ExpressionUnary("cos", ExpressionConst("paramTheta", shape.theta)),\
    ExpressionBinary("-",  ExpressionConst("var", "x"), ExpressionConst("paramCenterX", shape.center.x))

    B = ExpressionBinary("*",\
    ExpressionUnary("sin", ExpressionConst("paramTheta", shape.theta)),\
    ExpressionBinary("-",  ExpressionConst("var", "y"), ExpressionConst("paramCenterY", shape.center.y))


    x1 = ExpressionBinary("+", A, B)


    A = ExpressionBinary("*",\
        ExpressionUnary("-", ExpressionUnary("cos", ExpressionConst("paramTheta", shape.theta))),\
        ExpressionBinary("-",  ExpressionConst("var", "x"), ExpressionConst("paramCenterX", shape.center.x))

    B = ExpressionBinary("*",\
        ExpressionUnary("cos", ExpressionConst("paramTheta", shape.theta)),\
        ExpressionBinary("-",  ExpressionConst("var", "y"), ExpressionConst("paramCenterY", shape.center.y))


    y = ExpressionBinary("+", A, B)


    A = ExpressionBinary("*",\
    ExpressionUnary("cos", ExpressionConst("paramPhi", shape.phi)),\
    x1)

    B = ExpressionBinary("*",\
    ExpressionUnary("sin", ExpressionConst("paramPhi", shape.phi)),\
    ExpressionBinary("-",  ExpressionConst("var", "z"), ExpressionConst("paramCenterZ", shape.center.z))

    x= ExpressionBinary("+", A, B)


    A = ExpressionBinary("*",\
    ExpressionUnary("-", ExpressionUnary("sin", ExpressionConst("paramPhi", shape.phi))),\
    x1)

    B = ExpressionBinary("*",\
    ExpressionUnary("cos", ExpressionConst("paramPhi", shape.phi)),\
    ExpressionBinary("-",  ExpressionConst("var", "z"), ExpressionConst("paramCenterZ", shape.center.z))


    z = ExpressionBinary("+", A, B)

    return x, y, z



def expressionPrimitive(shape):

    if not isinstance(primitives, primitives.Primitives):
        raise TypeError, "I need a shape of type primitives.Primitives!"

    x, y, z = getXYZ(shape)

    axisX=ExpressionConst("paramAxisX", shape.axisX)
    axisY=ExpressionConst("paramAxisY", shape.axisY)
    axisZ=ExpressionConst("paramAxisZ", shape.axisZ)

    if isisnstance(shape, primitives.Ellipsoid):

        X = ExpressionBinary("*", ExpressionBinary("/", x, axisX) , ExpressionBinary("/", x, axisX))

        Y = ExpressionBinary("*", ExpressionBinary("/", y, axisY) , ExpressionBinary("/", y, axisY))

        Z = ExpressionBinary("*", ExpressionBinary("/", z, axisZ) , ExpressionBinary("/", z, axisZ))

        return ExpressionBinary("-",\
                ExpressionBinary("+", X,\
                ExpressionBinary("+", Y, Z)),\
                ExpressionConst("nbr", 1.0)\
                )


    elif isisnstance(shape, primitives.Frisbee):

        X = ExpressionBinary("*", ExpressionBinary("/", x, axisX) , ExpressionBinary("/", x, axisX))

        Y = ExpressionBinary("*", ExpressionBinary("/", y, axisY) , ExpressionBinary("/", y, axisY))

        Z = ExpressionBinary("/", z, axisZ)

        return ExpressionBinary("-",ExpressionBinary("+", ExpressionUnary("abs", Z),\
                                    ExpressionBinary("+", X, Y)),\
                                ExpressionConst("nbr", 1.0))



    elif isisnstance(shape, primitives.Cylinder):

        X = ExpressionBinary("*", ExpressionBinary("/", x, axisX) , ExpressionBinary("/", x, axisX))

        Y = ExpressionBinary("*", ExpressionBinary("/", y, axisY) , ExpressionBinary("/", y, axisY))

        radial =ExpressionBinary("-", ExpressionBinary("+", X, Y), ExpressionConst("nbr", 1.0))

        Z = ExpressionBinary("/", z, axisZ)


        height= ExpressionUnary("abs", Z)

        return ExpressionBinary("max", height, radial)

    elif isisnstance(shape, primitives.Parallepiped):

        X = ExpressionBinary("-", ExpressionUnary("abs", x), ExpressionBinary("/", axisX, ExpressionConst("nbr", 2.0)))

        Y = ExpressionBinary("-", ExpressionUnary("abs", y), ExpressionBinary("/", axisY, ExpressionConst("nbr", 2.0)))

        Z = ExpressionBinary("-", ExpressionUnary("abs", z), ExpressionBinary("/", axisZ, ExpressionConst("nbr", 2.0)))

        return ExpressionBinary("-", ExpressionBinary("+", ExpressionBinary("+", X, Y), Z), ExpressionConst("nbr", 1.0))


def expressionTorus(shape):

    if not isinstance(primitives, primitives.Torus):
        raise TypeError, "I need a shape of type primitives.Torus!"

    x, y, z = getXYZ(shape)

    r = ExpressionConst("paramr", shape.r)

    R = ExpressionConst("paramR", shape.R)

    A = ExpressionBinary("*", ExpressionBinary("-", x, R), ExppressionBinary("-, x, R"))

    B = ExpressionBinary("*", z, z)

    C = ExpressionBinary("*", r, r)

    return ExpressionBinary("-", ExpressionBinary("+", A, B), C)


def expression(shape):

    if not isinstance(shape, primitives.Shape):
        raise TypeError, "I need a shape of type primtives.Shape"


    elif isinstance(shape, primitives.Union):
        return ExpressionBinary("min", expression(shape.first), expression(shape.second))

    elif isinstance(shape, primitives.Intersection):
        return ExpressionBinary("max", expression(shape.first), expression(shape.second))

    elif isinstance(shape, primitives.Difference):

        left=expression(shape.first)

        right=ExpressionUnary("-", expression(shape.second))

        return ExpressionBinary("max", left, right)


    elif isinstance(shape, primitives.Primitives):

        return expressionPrimitive(shape)

    elif isinstance(shape, primitives.Torus):

        return expressionTorus(shape)




def expressionTwist(shape)

    if not isisntance(shape, primitives.Twist):
        raise TypeError, "I need a shape of type primitives.Twist!"

    x, y, z = getXYZ(shape)

    expressionShape = expression(shape)

    rate=ExpressionConst("paramRate", shape.rate)

    theta_twist = ExpressionBinary("*", ExpressionBinary("*", ExpressionBinary("*",\
                  ExpressionConst("nbre", 2.0), ExpressionConst("nbr", math.pi)),rate), z)


    X = ExpressionBinary("-", ExpressionBinary("*", ExpressionUnary("cos", theta_twist), x), ExpressionBinary("*", ExpressionUnary("sin", theta_twist), y))

    Y = ExpressionBinary("+", ExpressionBinary("*", ExpressionUnary("sin", theta_twist), x), ExpressionBinary("*", ExpressionUnary("cos", theta_twist), y))

    Z = z

    if isinstance(expressionShape, ExpressionBinary):
        expressionShape.left = expresssionTwist(expressionShape.left)
        expressionShape.right = expresssionTwist(expressionShape.right)

    elif isinstance(expressionShape, ExpressionUnary):
        expressionShape.middle = expresssionTwist(expressionShape.middle)

    elif isinstance(expressionShape, ExpressionConst) and expressionShape.type=="var":

        if expressionShape.type == "x":
            expressionShape = X

        elif expressionShape.type == "y":
            expressionShape = Y

        elif expressionShape.type == "z":
                expressionShape = Z


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

        cpdef  duplicate(self):

            cdef ExtrusionOfShape2D newExtrusion=ExtrusionOfShape2D(self.shape2D, self.heigth, self.theta, self.phi, self.center)
            return newExtrusion


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

        cdef Point translationRotation(self,  Point point):

            cdef double x
            cdef double y
            cdef double z

            x1=self.cosTheta*(point.x-self.center.x)+self.sinTheta*(point.y-self.center.y)
            y=-self.sinTheta*(point.x-self.center.x)+self.cosTheta*(point.y-self.center.y)

            x=self.cosPhi*x1+self.sinPhi*(point.z-self.center.z)
            z=-self.sinPhi*x1+self.cosPhi*(point.z-self.center.z)
            cdef Point newpoint=Point(x,y,z)
            return newpoint


        cpdef double eval(self,  Point point):

            newpoint=self.translationRotation(point)

            cdef double eval = max(fabs(newpoint.z-self.heigth/2.0)-self.heigth/2.0,self.shape2D.eval(newpoint.projectTo2D()))

            return eval











cdef class Geometric_Transformation(Shape):

    def __init__(self, shape, theta, phi, center):

        Shape.__init__(self)

        self.index=generateNewIndex()

        self.shape=shape

        self.theta=theta
        self.phi=phi

        self.cosTheta=cos(theta)
        self.sinTheta=sin(theta)
        self.cosPhi=cos(phi)
        self.sinPhi=sin(phi)

        self.center=center

    cpdef duplicate(self):

        cdef Geometric_Transformation newGeometric_Transformation=Geometric_Transformation(self.shape,self.theta, self.phi, self.center, self.rate)
        return newGeometric_Transformation

    cdef Point translationRotation(self,  Point point):

        cdef double x
        cdef double y
        cdef double z

        x1=self.cosTheta*(point.x-self.center.x)+self.sinTheta*(point.y-self.center.y)
        y=-self.sinTheta*(point.x-self.center.x)+self.cosTheta*(point.y-self.center.y)

        x=self.cosPhi*x1+self.sinPhi*(point.z-self.center.z)
        z=-self.sinPhi*x1+self.cosPhi*(point.z-self.center.z)
        cdef Point newpoint=Point(x,y,z)

        return newpoint

    cpdef double eval(self,  Point point):

        newpoint=self.translationRotation(point)

        cdef double eval = self.shape.eval(newpoint)

        return eval
