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


import primitives2D

import expression

def expressionPrimitive2D(shape2D):

    if not isinstance(shape2D, primitives2D.Primitives2D):
        raise TypeError, "I need a shape of type primitives2D.Primitives2D!"

    theta = shape2D.theta
    center = shape2D.center

    A = expression.ExpressionBinary("*",\
    expression.ExpressionUnary("cos", expression.ExpressionConst("paramTheta", theta)),\
    expression.ExpressionBinary("-",  expression.ExpressionConst("var", "x"), expression.ExpressionConst("paramCenterX", center.x)))

    B = expression.ExpressionBinary("*",\
    expression.ExpressionUnary("sin", expression.ExpressionConst("paramTheta", theta)),\
    expression.ExpressionBinary("-",  expression.ExpressionConst("var", "y"), expression.ExpressionConst("paramCenterY", center.y)))


    x = expression.ExpressionBinary("+", A, B)


    A = expression.ExpressionBinary("*",\
        expression.ExpressionUnary("-", expression.ExpressionUnary("cos", expression.ExpressionConst("paramTheta", theta))),\
        expression.ExpressionBinary("-",  expression.ExpressionConst("var", "x"), expression.ExpressionConst("paramCenterX", center.x)))

    B = expression.ExpressionBinary("*",\
        expression.ExpressionUnary("cos", expression.ExpressionConst("paramTheta", theta)),\
        expression.ExpressionBinary("-",  expression.ExpressionConst("var", "y"), expression.ExpressionConst("paramCenterY", center.y)))


    y = expression.ExpressionBinary("+", A, B)

    axisX=expression.ExpressionConst("paramAxisX", shape2D.axisX)
    axisY=expression.ExpressionConst("paramAxisY", shape2D.axisY)

    if isinstance(shape2D, primitives2D.Ellipse):

        X = expression.ExpressionBinary("*", expression.ExpressionBinary("/", x, axisX) , expression.ExpressionBinary("/", x, axisX))

        Y = expression.ExpressionBinary("*", expression.ExpressionBinary("/", y, axisY) , expression.ExpressionBinary("/", y, axisY))

        return expression.ExpressionBinary("-",\
                expression.ExpressionBinary("+", X, Y),\
                expression.ExpressionConst("nbr", 1.0)\
                )



def expressionHalfPlaneGivenByAVector2D(shape2D):

    if not isinstance(shape2D, primitives2D.HalfPlaneGivenByAVector2D):
        raise TypeError, "I need a shape of type primitives2D.HalfPlaneGivenByAVector2D!"

    vect = shape2D.vect
    center = vect.first

    l = expression.ExpressionUnary("sqrt",\
    expression.ExpressionBinary("+", expression.ExpressionBinary("*", expression.ExpressionBinary("-", expression.ExpressionConst("param", vect.second.x), expression.ExpressionConst("param", vect.first.x)),\
    expression.ExpressionBinary("-", expression.ExpressionConst("param", vect.second.x), expression.ExpressionConst("param", vect.first.x))),\
    expression.ExpressionBinary("*",expression.ExpressionBinary("-", expression.ExpressionConst("param", vect.second.y), expression.ExpressionConst("param", vect.first.y)),\
    expression.ExpressionBinary("-", expression.ExpressionConst("param", vect.second.y), expression.ExpressionConst("param", vect.first.y)))))

    sinTheta = expression.ExpressionBinary("/", expression.ExpressionBinary("-", expression.ExpressionConst("param", vect.second.y), expression.ExpressionConst("param", vect.first.y)), l)
    cosTheta = expression.ExpressionBinary("/", expression.ExpressionBinary("-", expression.ExpressionConst("param", vect.second.x), expression.ExpressionConst("param", vect.first.x)), l)



#    A = expression.ExpressionBinary("*",\
#    cosTheta,\
#    expression.ExpressionBinary("-",  expression.ExpressionConst("var", "x"), expression.ExpressionConst("paramCenterX", center.x))

#    B = expression.ExpressionBinary("*",\
#    sinTheta,\
#    expression.ExpressionBinary("-",  expression.ExpressionConst("var", "y"), expression.ExpressionConst("paramCenterY", center.y))


#    x = expression.ExpressionBinary("+", A, B)


    A = expression.ExpressionBinary("*",\
        expression.ExpressionUnary("-", sinTheta),\
        expression.ExpressionBinary("-",  expression.ExpressionConst("var", "x"), expression.ExpressionConst("paramCenterX", center.x)))

    B = expression.ExpressionBinary("*",\
        cosTheta,\
        expression.ExpressionBinary("-",  expression.ExpressionConst("var", "y"), expression.ExpressionConst("paramCenterY", center.y)))


    y = expression.ExpressionBinary("+", A, B)

    return y


def expressionShape2D(shape2D):

    if not isinstance(shape2D, primitives2D.Shape2D):

        raise TypeError, "I need a shape of type primitives2D.shape2D"


    elif isinstance(shape2D, primitives2D.Union):
        return expression.ExpressionBinary("min", expressionShape2D(shape2D.first), expressionShape2D(shape2D.second))

    elif isinstance(shape2D, primitives2D.Intersection):
        return expression.ExpressionBinary("max", expressionShape2D(shape2D.first), expressionShape2D(shape2D.second))

    elif isinstance(shape2D, primitives2D.Difference):

        left=expressionShape2D(shape2D.first)

        right=expression.ExpressionUnary("-", expressionShape2D(shape2D.second))

        return expression.ExpressionBinary("max", left, right)


    elif isinstance(shape2D, primitives2D.Primitives2D):

        return expressionPrimitive2D(shape2D)

    elif isinstance(shape2D, primitives2D.HalfPlaneGivenByAVector2D):

        return expressionHalfPlaneGivenByAVector2D(shape2D)
