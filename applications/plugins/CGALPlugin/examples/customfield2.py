import pyximport; pyximport.install()
import dfgeom
from dfgeom import *

x=0.5
r=0.0

bx = Box([1.0,1.0,1.0])
bx2 = Box([0.5,0.5,10])
bx3 = Box([0.5,10,0.5])

b1 = Sphere([0.0,0.0,1.0], 1.0)
b2 = Sphere([0.0,0.0,-1.0], 1.0)
b3 = Sphere([0.0,1.0,0.0], 1.0)
b4 = Sphere([-1.0,0.0,0.0], 1.0)

bu = Union(b1, Union(b2, Union( b3, b4)))

d = Union(bx2,bx3)
d = Union(d, bu)
u = Difference(bx,d)

def getCythonFunction():
    return dfgeom.getCythonRawFunction(u)

def evalField(x,y,z):
    return dfgeom.evalField(u, x, y, z)
