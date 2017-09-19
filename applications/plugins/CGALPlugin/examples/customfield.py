import pyximport; pyximport.install()
import dfgeom
from dfgeom import *
import math

class Difference2(PythonShape):
        def __init__(self, a, b):
            self.a = a
            self.b = b

        def evalField(self, p):
                return  max(-self.a.evalField(p), self.b.evalField(p))


r = 0.7
dd = -0.5
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


def getCythonFunction():
    return dfgeom.getCythonRawFunction(d)

def evalField(x, y, z):
        return dfgeom.evalField(d, x, y, z)
