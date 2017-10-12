#!/usr/bin/env
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
import copy

i=0
j=0

def getNewIndex():
    global i
    temp=str(i)
    i+=1
    return temp

def clearOut():
    global i
    i=0

def getListOfExpressionIndices(expression):

    listTemp=[expression.index]

    if isinstance(expression, ExpressionBinary):
        listTemp.append(getListOfExpressionIndices(expression.left))
        listTemp.append(getListOfExpressionIndices(expression.right))

    elif isinstance(expression, ExpressionUnary):
        listTemp.append(getListOfExpressionIndices(expression.middle))

    return listTemp

def IsSubExpressionsInList(expression, listTemp):

    if expression.index in listTemp:

        return True

    if isinstance(expression, ExpressionBinary):
        return IsSubExpressionsInList(expression.left, listTemp) or IsSubExpressionsInList(expression.right, listTemp)

    elif isinstance(expression, ExpressionUnary):
        return IsSubExpressionsInList(expression.middle, listTemp)

    return False

def IsTree(expression):

    if isinstance(expression, ExpressionBinary):

        listTemp=getListOfExpressionIndices(expression.right)

        if IsSubExpressionsInList(expression.left, listTemp):
            raise Error, "the expression is not a tree, it has loops"

        return IsTree(expression.left) and IsTree(expression.right)

    elif isinstance(expression, ExpressionUnary):
        return IsTree(expression.middle)

    else:

        return True

class Expression(object):

    def __init__(self):

        self.index = getNewIndex()

    def display(self):

       return self.display()

def duplicate(expression):

    if isinstance(expression, ExpressionBinary):
        return ExpressionBinary(expression.type, duplicate(expression.left), duplicate(expression.right))

    elif isinstance(expression, ExpressionUnary):
        return ExpressionUnary(expression.type, duplicate(expression.middle))

    elif isinstance(expression, ExpressionConst):
        return ExpressionConst(expression.type, copy.deepcopy(expression.const))





class ExpressionBinary(Expression):

    def __init__(self,type, left, right):

        if not (isinstance(left, Expression) and isinstance(right, Expression)):
            print "left"+type(left)
            raise TypeError, "not an Expression"
        if left == right:
            raise ValueError, "the two sides are equal, you would not get a tree!"

        Expression.__init__(self)
        self.type = type
        self.left = left
        self.right = right

    def display(self):

        if self.type == "max" or self.type == "min":

            return self.type+"("+self.left.display()+","+self.right.display()+")"
        else:
            return "("+self.left.display()+self.type+self.right.display()+")"
    def __str__(self):
        return self.type

class ExpressionUnary(Expression):

    def __init__(self, type, middle):
        Expression.__init__(self)
        self.type = type
        self.middle = middle

    def display(self):
        return self.type+"("+self.middle.display()+")"

    def __str__(self):
        return self.type

class ExpressionConst(Expression):

    def __init__(self, type,  const):
        Expression.__init__(self)
        self.type = type
        self.const = const

    def display(self):
        return str(self.type)

    def __str__(self):
        return self.type

def getXYZ(theta, phi, center, type, index):

    global j
    j+=1
    print j


    A = ExpressionBinary("*",\
    ExpressionUnary("cos", ExpressionConst("paramTheta_" + type + "_"+index, theta)),\
    ExpressionBinary("-",  ExpressionConst("var", "x"), ExpressionConst("paramCenterX_" + type + "_"+index, center.x)))

    B = ExpressionBinary("*", \
    ExpressionUnary("sin", ExpressionConst("paramTheta_" + type + "_"+index, theta)),\
    ExpressionBinary("-",  ExpressionConst("var", "y"), ExpressionConst("paramCenterY_" + type + "_"+index, center.y)))


    x1 = ExpressionBinary("+", A, B)

    A = ExpressionBinary("*",\
        ExpressionUnary("-", ExpressionUnary("sin", ExpressionConst("paramTheta_" + type + "_"+index, theta))),\
        ExpressionBinary("-",  ExpressionConst("var", "x"), ExpressionConst("paramCenterX_" + type + "_"+index, center.x)))

    B = ExpressionBinary("*",\
        ExpressionUnary("cos", ExpressionConst("paramTheta_" + type + "_"+index, theta)),\
        ExpressionBinary("-",  ExpressionConst("var", "y"), ExpressionConst("paramCenterY_" + type + "_"+index, center.y)))


    y = ExpressionBinary("+", A, B)

    A = ExpressionBinary("*",\
    ExpressionUnary("cos", ExpressionConst("paramPhi_" + type + "_"+index, phi)),\
    x1)

    B = ExpressionBinary("*",\
    ExpressionUnary("sin", ExpressionConst("paramPhi_" + type + "_"+index, phi)),\
    ExpressionBinary("-",  ExpressionConst("var", "z"), ExpressionConst("paramCenterZ_" + type + "_"+index, center.z)))

    x= ExpressionBinary("+", A, B)

    A = ExpressionBinary("*",\
    ExpressionUnary("-", ExpressionUnary("sin", ExpressionConst("paramPhi_" + type + "_"+index, phi))),\
    x1)

    B = ExpressionBinary("*",\
    ExpressionUnary("cos", ExpressionConst("paramPhi_" + type + "_"+index, phi)),\
    ExpressionBinary("-",  ExpressionConst("var", "z"), ExpressionConst("paramCenterZ_" + type + "_"+index, center.z)))


    z = ExpressionBinary("+", A, B)


    return (x, y, z)



def expressionPrimitive(shape):

    if not isinstance(shape, primitives.Primitives):
        raise TypeError, "I need a shape of type primitives.Primitives!"

    theta = shape.theta
    phi = shape.phi
    center = shape.center
    index = shape.index
    type= shape.type

    (x, y, z) = getXYZ(theta, phi, center,type, index)

    axisX=ExpressionConst("paramAxisX_" + type + "_"+index, shape.axisX)
    axisY=ExpressionConst("paramAxisY_" + type + "_"+index, shape.axisY)
    axisZ=ExpressionConst("paramAxisZ_" + type + "_"+index, shape.axisZ)

    if isinstance(shape, primitives.Ellipsoid):

        coordX = ExpressionBinary("/", x, axisX)

        X = ExpressionBinary("*", duplicate(coordX), duplicate(coordX))

        coordY= ExpressionBinary("/", y, axisY)

        Y = ExpressionBinary("*", duplicate(coordY), duplicate(coordY))

        coordZ= ExpressionBinary("/", z, axisZ)

        Z = ExpressionBinary("*", duplicate(coordZ), duplicate(coordZ))


        return ExpressionBinary("-",\
                ExpressionBinary("+", X,\
                ExpressionBinary("+", Y, Z)),\
                ExpressionConst("nbr", 1.0)\
                )


    elif isinstance(shape, primitives.Frisbee):

        coordX = ExpressionBinary("/", x, axisX)

        X = ExpressionBinary("*", duplicate(coordX), duplicate(coordX))

        coordY= ExpressionBinary("/", y, axisY)

        Y = ExpressionBinary("*", duplicate(coordY), duplicate(coordY))

        Z = ExpressionBinary("/", duplicate(z), duplicate(axisZ))

        return ExpressionBinary("-",ExpressionBinary("+", ExpressionUnary("abs", Z),\
                                    ExpressionUnary("sqrt", ExpressionBinary("+", X, Y))),\
                                ExpressionConst("nbr", 1.0))



    elif isinstance(shape, primitives.Cylinder):

        coordX = ExpressionBinary("/", x, axisX)

        X = ExpressionBinary("*", duplicate(coordX), duplicate(coordX))

        coordY= ExpressionBinary("/", y, axisY)

        Y = ExpressionBinary("*", duplicate(coordY), duplicate(coordY))

        radial =ExpressionBinary("-", ExpressionBinary("+", X, Y), ExpressionConst("nbr", 1.0))

        height = ExpressionBinary("-", ExpressionUnary("abs", duplicate(z)), duplicate(axisZ))

        return ExpressionBinary("max", height, radial)

    elif isinstance(shape, primitives.Parallepiped):

        X = ExpressionBinary("-", ExpressionUnary("abs", x), ExpressionBinary("/", axisX, ExpressionConst("nbr", 2.0)))

        Y = ExpressionBinary("-", ExpressionUnary("abs", y), ExpressionBinary("/", axisY, ExpressionConst("nbr", 2.0)))

        Z = ExpressionBinary("-", ExpressionUnary("abs", z), ExpressionBinary("/", axisZ, ExpressionConst("nbr", 2.0)))

        return ExpressionBinary("max", ExpressionBinary("max", X, Y), Z)


def expressionTorus(shape):

    if not isinstance(shape, primitives.Torus):
        raise TypeError, "I need a shape of type primitives.Torus!"

    theta = shape.theta
    phi = shape.phi
    center = shape.center

    index=shape.index
    type = shape.type
    (x, y, z) = getXYZ(theta, phi, center, type, index)

    X = ExpressionUnary("sqrt", ExpressionBinary("+", ExpressionBinary("*", duplicate(x), duplicate(x)), ExpressionBinary("*", duplicate(y), duplicate(y))))

    Z = z

    r = ExpressionConst("paramr_" + type + "_"+index, shape.r)

    R = ExpressionConst("paramR_" + type + "_"+index, shape.R)

    A = ExpressionBinary("*", ExpressionBinary("-", X, R), ExpressionBinary("-", duplicate(X), duplicate(R)))

    B = ExpressionBinary("*", Z, duplicate(Z))

    C = ExpressionBinary("*", r, duplicate(r))

    return ExpressionBinary("-", ExpressionBinary("+", A, B), C)


def expressionTwist(expressionShape, theta, phi, center, rate, type, index):



    if isinstance(expressionShape, ExpressionBinary):
        expressionShape.left = expressionTwist(expressionShape.left, theta, phi, center, rate, type, index)
        expressionShape.right = expressionTwist(expressionShape.right, theta, phi, center, rate, type, index)

    elif isinstance(expressionShape, ExpressionUnary):
        expressionShape.middle = expressionTwist(expressionShape.middle, theta, phi, center, rate, type, index)

    elif isinstance(expressionShape, ExpressionConst) and expressionShape.type=="var":


        (x, y, z) = getXYZ(theta, phi, center, type, index)

        Erate=ExpressionConst("paramRate_" + type + "_"+index, rate)

        theta_twist = ExpressionBinary("*", ExpressionBinary("*", ExpressionBinary("*",\
                      ExpressionConst("nbr", 2.0), ExpressionConst("nbr", math.pi)),Erate), z)


        X = ExpressionBinary("-", ExpressionBinary("*", ExpressionUnary("cos", theta_twist), x), ExpressionBinary("*", ExpressionUnary("sin", theta_twist), y))

        Y = ExpressionBinary("+", ExpressionBinary("*", ExpressionUnary("sin", theta_twist), x), ExpressionBinary("*", ExpressionUnary("cos", theta_twist), duplicate(y)))

        Z = z

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

    return expressionTwist(expressionShape, theta, phi, center, rate, shape.type, shape.index)


def expressionGeometric_Transformation(expressionShape, theta, phi, center, type, index, spacing = ""):


    print spacing + "Transformation "+ str(expressionShape)

    if isinstance(expressionShape, ExpressionBinary):
        expressionShape.left =  expressionGeometric_Transformation(expressionShape.left, theta, phi, center, type, index, spacing+" ")
        expressionShape.right = expressionGeometric_Transformation(expressionShape.right, theta, phi, center, type, index, spacing+" ")

    elif isinstance(expressionShape, ExpressionUnary):
        expressionShape.middle =  expressionGeometric_Transformation(expressionShape.middle, theta, phi, center, type, index, spacing+" ")

    elif isinstance(expressionShape, ExpressionConst) and (expressionShape.const == "x" or expressionShape.const == "y" or expressionShape.const == "z"):

        (x, y, z) = getXYZ(theta, phi, center, type, index)

        if expressionShape.const == "x":

            print spacing+"x"

            expressionShape = x

        elif expressionShape.const == "y":
            print spacing+"y"
            expressionShape = y

        elif expressionShape.const == "z":
            print spacing+"z"
            expressionShape = z


    return expressionShape


def expressionGeometric_TransformationShape(shape):


    shapeTemp=shape.shapeIn



    expressionShape = expression(shapeTemp)
    print "TShape1 "+ expressionShape.display()
    print "TShape2 "+ str(shape)
    return expressionGeometric_Transformation(expressionShape, shape.theta, shape.phi, shape.center, shape.type, shape.index)


def expressionExtrusionOfShape2D(shape):

    if not isinstance(shape, primitives.ExtrusionOfShape2D):
        raise TypeError, "I need a shape of type primitives.ExtrusionOfShape2D!"

    shape2D=shape.shape2D

    theta = shape.theta
    phi = shape.phi
    center = shape.center
    index= shape.index
    type = shape.type

    heigth=ExpressionConst("paramHeight_" + type + "_"+index, shape.heigth )

    expressionD=expression2D.expressionShape2D(shape2D)

    zPart1 = ExpressionUnary("abs", ExpressionBinary("-", ExpressionConst("var", "z"), ExpressionBinary("/", heigth, ExpressionConst("nbr", 2.0))))

    zPart2 = ExpressionBinary("/", heigth, ExpressionConst("nbr", 2.0))

    zPart = ExpressionBinary("-", zPart1, zPart2)

    expressionShape = ExpressionBinary("max", zPart, expressionD)

    return expressionGeometric_Transformation(expressionShape, theta, phi, center, type, index)



def expression(shape):

    print "Expression "+ str(shape)

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


