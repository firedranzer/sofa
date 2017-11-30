import sys, os
import main
import generate_expression



ind=main.generateIndividual("ellipsoid")
shape=main.getShapeFromInd(ind)
generate_expression.writeLitteralExpressionInFile(shape, "erwan.py")


