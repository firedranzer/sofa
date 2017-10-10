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

import math
import primitives2D
import primitives
import expression2D

i=0

def getNewIndex():
    global i
    temp=str(i)
    i+=1
    return temp

def clearOut():
    global i
    i=0


class Expression(object):

    def __init__(self):

        self.index = getNewIndex()

    def toString():

       return ""

class ExpressionBinary(Expression):

    def __init__(self,type, left, right):

        if not (isinstance(left, Expression) and isinstance(right, Expression)):
#            print str(type(left))+str(type(right))
            print "left"+type(left)
            raise TypeError, "not an Expression"

        Expression.__init__(self)
        self.type = type
        self.left = left
        self.right = right

    def toString():

       return ""

class ExpressionUnary(Expression):

    def __init__(self, type, middle):
        Expression.__init__(self)
        self.type = type
        self.middle = middle

class ExpressionConst(Expression):

    def __init__(self, type,  const):
        Expression.__init__(self)
        self.type = type
        self.const = const



def getXYZ(theta, phi, center):

    A = ExpressionBinary("*",\
    ExpressionUnary("cos", ExpressionConst("paramTheta", theta)),\
    ExpressionBinary("-",  ExpressionConst("var", "x"), ExpressionConst("paramCenterX", center.x)))

    B = ExpressionBinary("*", \
    ExpressionUnary("sin", ExpressionConst("paramTheta", theta)),\
    ExpressionBinary("-",  ExpressionConst("var", "y"), ExpressionConst("paramCenterY", center.y)))


    x1 = ExpressionBinary("+", A, B)


    A = ExpressionBinary("*",\
        ExpressionUnary("-", ExpressionUnary("sin", ExpressionConst("paramTheta", theta))),\
        ExpressionBinary("-",  ExpressionConst("var", "x"), ExpressionConst("paramCenterX", center.x)))

    B = ExpressionBinary("*",\
        ExpressionUnary("cos", ExpressionConst("paramTheta", theta)),\
        ExpressionBinary("-",  ExpressionConst("var", "y"), ExpressionConst("paramCenterY", center.y)))


    y = ExpressionBinary("+", A, B)


    A = ExpressionBinary("*",\
    ExpressionUnary("cos", ExpressionConst("paramPhi", phi)),\
    x1)

    B = ExpressionBinary("*",\
    ExpressionUnary("sin", ExpressionConst("paramPhi", phi)),\
    ExpressionBinary("-",  ExpressionConst("var", "z"), ExpressionConst("paramCenterZ", center.z)))

    x= ExpressionBinary("+", A, B)


    A = ExpressionBinary("*",\
    ExpressionUnary("-", ExpressionUnary("sin", ExpressionConst("paramPhi", phi))),\
    x1)

    B = ExpressionBinary("*",\
    ExpressionUnary("cos", ExpressionConst("paramPhi", phi)),\
    ExpressionBinary("-",  ExpressionConst("var", "z"), ExpressionConst("paramCenterZ", center.z)))


    z = ExpressionBinary("+", A, B)

    return (x, y, z)



def expressionPrimitive(shape):

    if not isinstance(shape, primitives.Primitives):
        raise TypeError, "I need a shape of type primitives.Primitives!"

    theta = shape.theta
    phi = shape.phi
    center = shape.center

    (x, y, z) = getXYZ(theta, phi, center)

    axisX=ExpressionConst("paramAxisX", shape.axisX)
    axisY=ExpressionConst("paramAxisY", shape.axisY)
    axisZ=ExpressionConst("paramAxisZ", shape.axisZ)

    if isinstance(shape, primitives.Ellipsoid):

        X = ExpressionBinary("*", ExpressionBinary("/", x, axisX) , ExpressionBinary("/", x, axisX))

        Y = ExpressionBinary("*", ExpressionBinary("/", y, axisY) , ExpressionBinary("/", y, axisY))

        Z = ExpressionBinary("*", ExpressionBinary("/", z, axisZ) , ExpressionBinary("/", z, axisZ))

        return ExpressionBinary("-",\
                ExpressionBinary("+", X,\
                ExpressionBinary("+", Y, Z)),\
                ExpressionConst("nbr", 1.0)\
                )


    elif isinstance(shape, primitives.Frisbee):

        X = ExpressionBinary("*", ExpressionBinary("/", x, axisX) , ExpressionBinary("/", x, axisX))

        Y = ExpressionBinary("*", ExpressionBinary("/", y, axisY) , ExpressionBinary("/", y, axisY))

        Z = ExpressionBinary("/", z, axisZ)

        return ExpressionBinary("-",ExpressionBinary("+", ExpressionUnary("abs", Z),\
                                    ExpressionBinary("+", X, Y)),\
                                ExpressionConst("nbr", 1.0))



    elif isinstance(shape, primitives.Cylinder):

        X = ExpressionBinary("*", ExpressionBinary("/", x, axisX) , ExpressionBinary("/", x, axisX))

        Y = ExpressionBinary("*", ExpressionBinary("/", y, axisY) , ExpressionBinary("/", y, axisY))

        radial =ExpressionBinary("-", ExpressionBinary("+", X, Y), ExpressionConst("nbr", 1.0))

        Z = ExpressionBinary("/", z, axisZ)


        height= ExpressionUnary("abs", Z)

        return ExpressionBinary("max", height, radial)

    elif isinstance(shape, primitives.Parallepiped):

        X = ExpressionBinary("-", ExpressionUnary("abs", x), ExpressionBinary("/", axisX, ExpressionConst("nbr", 2.0)))

        Y = ExpressionBinary("-", ExpressionUnary("abs", y), ExpressionBinary("/", axisY, ExpressionConst("nbr", 2.0)))

        Z = ExpressionBinary("-", ExpressionUnary("abs", z), ExpressionBinary("/", axisZ, ExpressionConst("nbr", 2.0)))

        return ExpressionBinary("-", ExpressionBinary("+", ExpressionBinary("+", X, Y), Z), ExpressionConst("nbr", 1.0))


def expressionTorus(shape):

    if not isinstance(shape, primitives.Torus):
        raise TypeError, "I need a shape of type primitives.Torus!"

    theta = shape.theta
    phi = shape.phi
    center = shape.center

    (x, y, z) = getXYZ(theta, phi, center)

    r = ExpressionConst("paramr", shape.r)

    R = ExpressionConst("paramR", shape.R)

    A = ExpressionBinary("*", ExpressionBinary("-", x, R), ExpressionBinary("-", x, R))

    B = ExpressionBinary("*", z, z)

    C = ExpressionBinary("*", r, r)

    return ExpressionBinary("-", ExpressionBinary("+", A, B), C)


def expressionTwist(expressionShape, theta, phi, center, rate):

    (x, y, z) = getXYZ(theta, phi, center)

    Erate=ExpressionConst("paramRate", rate)

    theta_twist = ExpressionBinary("*", ExpressionBinary("*", ExpressionBinary("*",\
                  ExpressionConst("nbr", 2.0), ExpressionConst("nbr", math.pi)),Erate), z)


    X = ExpressionBinary("-", ExpressionBinary("*", ExpressionUnary("cos", theta_twist), x), ExpressionBinary("*", ExpressionUnary("sin", theta_twist), y))

    Y = ExpressionBinary("+", ExpressionBinary("*", ExpressionUnary("sin", theta_twist), x), ExpressionBinary("*", ExpressionUnary("cos", theta_twist), y))

    Z = z

    if isinstance(expressionShape, ExpressionBinary):
        expressionShape.left = expressionTwist(expressionShape.left, theta, phi, center, rate)
        expressionShape.right = expressionTwist(expressionShape.right, theta, phi, center, rate)

    elif isinstance(expressionShape, ExpressionUnary):
        expressionShape.middle = expressionTwist(expressionShape.middle, theta, phi, center, rate)

    elif isinstance(expressionShape, ExpressionConst) and expressionShape.type=="var":

        if expressionShape.const == "x":
            expressionShape = X

        elif expressionShape.const == "y":
            expressionShape = Y

        elif expressionShape.const == "z":
                expressionShape = Z

    return expressionShape


def expressionTwistShape(shape):

    if not isinstance(shape, primitives.Twist):
        raise TypeError, "I need a shape of type primitives.Twist!"

    theta = shape.theta
    phi = shape.phi
    center = shape.center

    shapeTemp=shape.shapeIn

    expressionShape = expression(shapeTemp)

    rate=shape.rate

    return expressionTwist(expressionShape, theta, phi, center, rate)


def expressionGeometric_Transformation(expressionShape, theta, phi, center):

    (x, y, z) = getXYZ(theta, phi, center)

    if isinstance(expressionShape, ExpressionBinary):
        expressionShape.left =  expressionGeometric_Transformation(expressionShape.left, theta, phi, center)
        expressionShape.right = expressionGeometric_Transformation(expressionShape.right, theta, phi, center)

    elif isinstance(expressionShape, ExpressionUnary):
        expressionShape.middle =  expressionGeometric_Transformation(expressionShape.middle, theta, phi, center)

    elif isinstance(expressionShape, ExpressionConst) and (expressionShape.const == "x" or expressionShape.const == "y" or expressionShape.const == "z"):

        if expressionShape.const == "x":
            expressionShape = x

        elif expressionShape.const == "y":
            expressionShape = y

        elif expressionShape.const == "z":
                expressionShape = z


    return expressionShape


def expressionGeometric_TransformationShape(shape):

    shapeTemp=shape.shapeIn

    theta = shape.theta

    phi = shape.phi

    center = shape.center

    expressionShape = expression(shapeTemp)

    return expressionGeometric_Transformation(expressionShape, theta, phi, center)


def expressionExtrusionOfShape2D(shape):

    if not isinstance(shape, primitives.ExtrusionOfShape2D):
        raise TypeError, "I need a shape of type primitives.ExtrusionOfShape2D!"

    shape2D=shape.shape2D

    theta = shape.theta
    phi = shape.phi
    center = shape.center

    heigth=ExpressionConst("paramHeight", shape.heigth )

    expressionD=expression2D.expressionShape2D(shape2D)

    zPart1 = ExpressionUnary("abs", ExpressionBinary("-", ExpressionConst("var", "z"), ExpressionBinary("/", heigth, ExpressionConst("nbr", 2.0))))

    zPart2 = ExpressionBinary("/", heigth, ExpressionConst("nbr", 2.0))

    zPart = ExpressionBinary("-", zPart1, zPart2)

    expressionShape = ExpressionBinary("max", zPart, expressionD)

    return expressionGeometric_Transformation(expressionShape, theta, phi, center)



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

    elif isinstance(shape, primitives.Twist):

        return expressionTwistShape(shape)

    elif isinstance(shape, primitives.Geometric_Transformation):

        return expressionGeometric_TransformationShape(shape)

    elif isinstance(shape, primitives.ExtrusionOfShape2D):

        return expressionExtrusionOfShape2D(shape)


