import pyximport; pyximport.install()
import dfgeom
from dfgeom import *

r=0.7
s1 = Sphere([1.0,0.0,0.0],r)
s2 = Sphere([-1.0,0.0,0.0],r)
u = Union(s1,s2)

def installCythonHook(obj):
    return dfgeom.installCythonHook(obj)

def evalField(x,y,z):
    return dfgeom.evalField(u, x, y, z)
