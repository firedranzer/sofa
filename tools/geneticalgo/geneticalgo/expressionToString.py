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



import math
import primitives2D
import primitives
import expression2D
import expression



def expressionToString(expressionTest):


    if not isinstance(expressionTest, expression.Expression):
        raise TypeError, "I need a expression of type expression.Expression"

    temp = ""

    if isinstance(expressionTest, expression.ExpressionBinary):
        if expressionTest.type == "max" or expressionTest.type == "min":

            temp = expressionToString(expressionTest.left)+  '\n'\
                 + expressionToString(expressionTest.right)+ "\n"

            return temp + "\n" + "double  A"+expressionTest.index+" = "+expressionTest.type+"(A"+expressionTest.left.index+", A"+expressionTest.right.index+");"


        else:
            temp = expressionToString(expressionTest.left)+  '\n'\
                 +  expressionToString(expressionTest.right)+ "\n"

            return temp + "\n" "double  A"+expressionTest.index+" = A"+expressionTest.left.index+expressionTest.type+ "A"+expressionTest.right.index+";"

    elif isinstance(expressionTest, expression.ExpressionUnary):
        temp = expressionToString(expressionTest.middle)+  '\n'

        return temp + "\n" + "double  A"+expressionTest.index+" = "+expressionTest.type+"(A"+expressionTest.middle.index+");"

    elif isinstance(expressionTest, expression.ExpressionConst):
        return "double  A"+expressionTest.index+" = "+str(expressionTest.const)+";"



def expressionToFile(expressionTest, filename):
    tempString=expressionToString(expressionTest)

    with open(filename, "w") as litteral_expression:
        litteral_expression.write(tempString)
        litteral_expression.close()
