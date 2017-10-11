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

i=0

def getIndex():
    global i
    temp= str(i)
    i+=1
    return temp

def value(expressionTest):

    if not isinstance(expressionTest, expression.Expression):
        raise TypeError, "I need a expression of type expression.Expression!"

    if isinstance(expressionTest, expression.ExpressionBinary):

        return (expressionTest.type, value(expressionTest.left), value(expressionTest.right))

    elif isinstance(expressionTest, expression.ExpressionUnary):

        return (expressionTest.type, value(expressionTest.middle))

    elif isinstance(expressionTest, expression.ExpressionConst):
        return (expressionTest.type, expressionTest.const)


def expressionToString(expressionTest, dict):


    if not isinstance(expressionTest, expression.Expression):
        raise TypeError, "I need a expression of type expression.Expression"

    if value(expressionTest) in dict:
        return ""

    else:

        index =getIndex()

        dict[value(expressionTest)]="A"+index

        if isinstance(expressionTest, expression.ExpressionBinary):


            if expressionTest.type == "max" or expressionTest.type == "min":

                temp = expressionToString(expressionTest.left, dict)+  '\n'\
                     + expressionToString(expressionTest.right, dict)+ "\n"

                return temp + "float  A"+index+" = "+str(expressionTest.type)+"("+dict[value(expressionTest.left)]+","+dict[value(expressionTest.right)]+");"

            else:

                temp = expressionToString(expressionTest.left, dict)+  '\n'\
                     +  expressionToString(expressionTest.right, dict)+ "\n"

                return temp + "float  A"+index+" = "+dict[value(expressionTest.left)]+str(expressionTest.type) + dict[value(expressionTest.right)]+";"


        elif isinstance(expressionTest, expression.ExpressionUnary):

            temp = expressionToString(expressionTest.middle, dict)+ '\n'

            return temp + "float  A"+index+" = "+str(expressionTest.type)+"("+dict[value(expressionTest.middle)]+");"

        elif isinstance(expressionTest, expression.ExpressionConst):
                return "float  A"+index+" = "+str(expressionTest.const)+";"+ "//"+ str(expressionTest.type)



def expressionToPython(expressionTest, dict):


    if not isinstance(expressionTest, expression.Expression):
        raise TypeError, "I need a expression of type expression.Expression"

    if value(expressionTest) in dict:
        return ""

    else:

        index =getIndex()

        dict[value(expressionTest)]="A"+index

        if isinstance(expressionTest, expression.ExpressionBinary):


            if expressionTest.type == "max" or expressionTest.type == "min":

                temp = expressionToPython(expressionTest.left, dict)+  '\n'\
                     + expressionToPython(expressionTest.right, dict)+ "\n"

                return temp + "    A"+index+" = "+str(expressionTest.type)+"("+dict[value(expressionTest.left)]+","+dict[value(expressionTest.right)]+")"

            else:

                temp = expressionToPython(expressionTest.left, dict)+  '\n'\
                     +  expressionToPython(expressionTest.right, dict)+ "\n"

                return temp + "    A"+index+" = "+dict[value(expressionTest.left)]+str(expressionTest.type) + dict[value(expressionTest.right)]+""


        elif isinstance(expressionTest, expression.ExpressionUnary):

            temp = expressionToPython(expressionTest.middle, dict)+ '\n'

            return temp + "    A"+index+" = "+str(expressionTest.type)+"("+dict[value(expressionTest.middle)]+")"

        elif isinstance(expressionTest, expression.ExpressionConst):
                return "    A"+index+" = "+str(expressionTest.const)+"   ## "+ expressionTest.type





def expressionWriting(expressionTest):

    global i

    i=0

    dict={}

    temp = expressionToPython(expressionTest, dict)

    temp = temp + "\n\n\n"+"return " + dict[value(expressionTest)]+";"

    return temp


def expressionWritingPython(expressionTest):

    global i

    i=0

    dict={}

    temp = expressionToPython(expressionTest, dict)

    temp = temp + "\n\n\n"+"    return " + dict[value(expressionTest)]

    return temp

def expressionToFilePython(expressionTest, filename):

    tempString=expressionWritingPython(expressionTest)

    with open(filename, "w") as litteral_expression:
        litteral_expression.write("from math import sin, cos, sqrt \n\n")
        litteral_expression.write("def evalField(x, y, z):\n\n")
        litteral_expression.write(tempString)
        litteral_expression.close()
