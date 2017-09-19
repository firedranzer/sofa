import pyximport; pyximport.install()
import dfgeom
from dfgeom import *
import math

import accordion
import primitives


class Difference2(PythonShape):
        def __init__(self, a, b):
            self.a = a
            self.b = b

        def evalField(self, p):
                return  max(-self.a.evalField(p), self.b.evalField(p))


r = 0.
dd = -0.0
s1 = Sphere([1.0,0.0,0.0],1.0)
s2 = Sphere([dd,0.0,0.0],1.3)
s3 = Sphere([-1.0,0.0,0.0],1.0)
u = Union(s1,s3)
d = Difference(u, s2)
u2 = Sphere([dd,1.0,0],r)
u3 = Sphere([dd,-1.0,0],r)
u4 = Sphere([dd,0,1.0],r)
u5 = Sphere([dd,0,-1.0],r)
u3= Union(u2, u3)
u4= Union(u4, u5)
u5= Union(u3,u4)
d = Difference(u5, d)


#def getCythonFunction():
#    return dfgeom.getCythonRawFunction(d)

shape=accordion.accordionUniform(10.0,4.0,2.0,"frisbee",5,5.0,5.0,5.0)

def evalField(x, y, z):
    point=primitives.Point(x,y,z)
    return shape.eval(point) #dfgeom.evalField(d, x, y, z)
