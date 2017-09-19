import pyximport; pyximport.install()
import dfgeom
from dfgeom import *

x=0.4
r=0.3
s1 = Sphere([1.0+x,0.0,0.0],r)
s2 = Sphere([-1.0+x,0.0,0.0],r)
u = Union(s1,s2)

def getCythonFunction():
    print("CYTHON GETTER CALLED")
    return dfgeom.getCythonRawFunction(u)

def evalField(x,y,z):
    return dfgeom.evalField(u, x, y, z)
