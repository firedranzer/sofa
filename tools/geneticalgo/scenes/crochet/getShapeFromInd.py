import geneticalgo
import math
import accordionutils as accordion
from geneticalgo import primitives



def getShapeFromInd(ind):
    shape, shapeMinus = accordion.accordionFreeDimension(ind, ind.height, ind.radius, ind.thickness, ind.listCavities)
    print "I hope a Shape "+ str(isinstance(shape, primitives.Shape))
    print type(shape)
    return shape, shapeMinus
