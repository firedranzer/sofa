import pyximport; pyximport.install()
import dfgeom
from dfgeom import *

x=0.0
r=0.7

s1 = Sphere([1.+x,1.0,1.0],r)
s2 = Sphere([-2+x,1.0,1.0],r)
u = Union(s1,s2)

def getCythonFunction():
    print("CYTHON GETTER CALLED")
    return dfgeom.getCythonRawFunction(u)

def evalField(x,y,z):
    return dfgeom.evalField(u, x, y, z)
