import pyximport; pyximport.install()
import sys
import SofaPython
from SofaPython import Tools
#sys.path.append( "../../python" )
import difigeometry
import dfgeom

import math
from math import sqrt

def toPythonShape(node):
    print("TO PYY: "+str(node.name))
    type = node.getData("type")
    if type == None:
        for child in node.getChildren():
            cc = toPythonShape(child)
            if cc != None:
                return cc
        return None

    if node.type == "shape.ShapeTree":
        c = node.getChildren()
        if len(c) == 0:
            return None
        u=toPythonShape(c[0])
        for child in c[1:]:
                uc = toPythonShape(child)
                if uc != None:
                    u=dfgeom.Union(u, uc)

        print("OUT: "+str(u))
        return u
    elif node.type == "shape.Difference":
        c = node.getChildren()
        if len(c) == 0:
            return None
        left=toPythonShape(c[0])
        right=toPythonShape(c[1])
        return dfgeom.Difference(left, right)
    elif node.type == "shape.Box":
        return dfgeom.Box(node.size[0])
    elif node.type == "shape.Sphere":
        print("SPHERE GEOM: "+str(node.center[0]))
        return dfgeom.Sphere(center=node.center[0], radius=node.radius[0][0])
    Sofa.msg_error("Un-recognized type:"+str(node.type))
    return None

def buildShapeFromSofaTree(shapetree):
    print("RE-BUILDING A PYTHON SHAPE")
    return toPythonShape(shapetree)

def glslFunction(shape):
    context = {}
    function = shape.toGLSL(context)
    return (context.values(), function);

def evalField(x, y, z):
    return 1.0

def evalShape(shape, x, y, z):
    #print("EVALUATING SHAPE: " + str(shape)+" "+str((x,y,z)))
    if shape == None:
        return 1.0
    return dfgeom.evalField(shape, x,y,z)
