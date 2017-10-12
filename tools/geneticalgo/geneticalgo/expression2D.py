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

def getXY(theta, center, type,  index, subIndex):

    A = expression.ExpressionBinary("*",\
    expression.ExpressionUnary("cos", expression.ExpressionConst("paramTheta_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, theta)),\
    expression.ExpressionBinary("-",  expression.ExpressionConst("var", "x"), expression.ExpressionConst("paramCenterX_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, center.x)))

    B = expression.ExpressionBinary("*",\
    expression.ExpressionUnary("sin", expression.ExpressionConst("paramTheta_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, theta)),\
    expression.ExpressionBinary("-",  expression.ExpressionConst("var", "y"), expression.ExpressionConst("paramCenterY_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, center.y)))


    x = expression.ExpressionBinary("+", A, B)


    A = expression.ExpressionBinary("*",\
        expression.ExpressionUnary("-", expression.ExpressionUnary("sin", expression.ExpressionConst("paramTheta_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, theta))),\
        expression.ExpressionBinary("-",  expression.ExpressionConst("var", "x"), expression.ExpressionConst("paramCenterX_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, center.x)))

    B = expression.ExpressionBinary("*",\
        expression.ExpressionUnary("cos", expression.ExpressionConst("paramTheta_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, theta)),\
        expression.ExpressionBinary("-",  expression.ExpressionConst("var", "y"), expression.ExpressionConst("paramCenterY_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, center.y)))


    y = expression.ExpressionBinary("+", A, B)

    return x, y




def expressionPrimitive2D(shape2D):

    if not isinstance(shape2D, primitives2D.Primitives2D):
        raise TypeError, "I need a shape of type primitives2D.Primitives2D!"

    theta = shape2D.theta
    center = shape2D.center

    index=shape2D.index

    subIndex=shape2D.subIndex

    type=shape2D.type

    x, y = getXY(theta, center, type,  index, subIndex)


    axisX=expression.ExpressionConst("paramAxisX_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, shape2D.axisX)
    axisY=expression.ExpressionConst("paramAxisY_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, shape2D.axisY)

    if isinstance(shape2D, primitives2D.Ellipse):

        coordX = expression.ExpressionBinary("/", x, axisX)

        X = expression.ExpressionBinary("*", coordX, expression.duplicate(coordX))

        coordY = expression.ExpressionBinary("/", y, axisY)

        Y = expression.ExpressionBinary("*",  coordY, expression.duplicate(coordY))

        return expression.ExpressionBinary("-",\
                expression.ExpressionBinary("+", X, Y),\
                expression.ExpressionConst("nbr", 1.0)\
                )



def expressionHalfPlaneGivenByAVector2D(shape2D):

    if not isinstance(shape2D, primitives2D.HalfPlaneGivenByAVector2D):
        raise TypeError, "I need a shape of type primitives2D.HalfPlaneGivenByAVector2D!"

    vect = shape2D.vect
    center = vect.first

    index = shape2D.index
    subIndex = shape2D.subIndex
    type = shape2D.type

    l = expression.ExpressionUnary("sqrt",\
    expression.ExpressionBinary("+", expression.ExpressionBinary("*", expression.ExpressionBinary("-", expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.second.x), expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.first.x)),\
    expression.ExpressionBinary("-", expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.second.x), expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.first.x))),\
    expression.ExpressionBinary("*",expression.ExpressionBinary("-", expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.second.y), expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.first.y)),\
    expression.ExpressionBinary("-", expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.second.y), expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.first.y)))))

    sinTheta = expression.ExpressionBinary("/", expression.ExpressionBinary("-", expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.second.y), expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.first.y)), expression.duplicate(l))
    cosTheta = expression.ExpressionBinary("/", expression.ExpressionBinary("-", expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.second.x), expression.ExpressionConst("paramVect_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, vect.first.x)), expression.duplicate(l))



    A = expression.ExpressionBinary("*",\
        expression.ExpressionUnary("-", sinTheta),\
        expression.ExpressionBinary("-",  expression.ExpressionConst("var", "x"), expression.ExpressionConst("paramCenterX_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, center.x)))

    B = expression.ExpressionBinary("*",\
        cosTheta,\
        expression.ExpressionBinary("-",  expression.ExpressionConst("var", "y"), expression.ExpressionConst("paramCenterY_"+type+"_Shape3D_"+index+"_Shap2D_"+subIndex, center.y)))


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
