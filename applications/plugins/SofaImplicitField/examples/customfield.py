import pyximport; pyximport.install()
import sys
import SofaPython
from SofaPython import Tools
#sys.path.append( "../../python" )
import difigeometry
import dfgeom

import math
from math import sqrt

b = dfgeom.Box([1.0,0.5,1.0])
u = dfgeom.Sphere([0.0,0.0,0.0], 1.05)

s = dfgeom.Sphere([0.0,0.0,0.0], 0.5)

shape = dfgeom.Difference(
                dfgeom.Difference(b, u),
                dfgeom.Sphere([0.0,0.5,0.0], 1.0)
                )

shape = dfgeom.Difference(
                dfgeom.Union(s, shape),
                dfgeom.Box([1.2,0.1,1.2])
                )

def glslFunction():
    context = {}
    function = shape.toGLSL(context)
    return (context.values(), function);

def evalField(x,y,z):
    return dfgeom.evalField(shape, x,y,z)
