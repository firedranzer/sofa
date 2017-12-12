import pyximport; pyximport.install()
import dfgeom
from dfgeom import *
import math
import minipsl

class Difference2(PythonShape):
        def __init__(self, a, b):
            self.a = a
            self.b = b

        def evalField(self, p):
                return  max(-self.a.evalField(p), self.b.evalField(p))



r = 0.7
dd = -0.5
s1 = Sphere(center=[1.0,0.0,0.0],radius=1.0)
s2 = Sphere(center=[dd,0.0,0.0],radius=1.3)
s3 = Sphere(center=[-1.0,0.0,0.0],radius=1.0)
u = Union(s1,s3)
d = Difference(u, s2)
u2 = Sphere(center=[dd,1.0,0],radius=r)
u3 = Sphere(center=[dd,-1.0,0],radius=r)
u4 = Sphere(center=[dd,0,1.0],radius=r)
u5 = Sphere(center=[dd,0,-1.0],radius=r)
u3= Union(u2, u3)
u4= Union(u4, u5)
u5= Union(u3,u4)
dd5 = Difference(u5, u2)
dd = Tiling(Sphere(center=[0.0,0.0,0.0],radius=0.2), [0.5,0.5,0.5])
dd = Scale(Sphere(center=[0.0,0.0,0.0],radius=1.0), 1.0)

u1 = Union( Cylinder([-0.8,0.8,0.1]),
            Cylinder([0.8,0.8,0.1]))
u2 = Union( Cylinder([0.8,-0.8,0.1]),
            Cylinder([-0.8,-0.8,0.1]))


a=Sphere(center=[1.0,0.0,0.0], radius=math.cos(0.25))
dd=-1.0
for i in range(0,60):
    dd+=0.05
    b=Sphere(center=[dd+math.cos(5.0*i/60.0*2),math.sin(5.0*i/60+dd),math.sin(3.0*i/60.0)], radius=math.fabs(0.4*math.cos(2.0*i/60)))
    a=Union(a,b)
dd=-1.0
for i in range(0,60):
    dd+=0.05
    b=Sphere(center=[dd+math.sin(3.0*i/60.0*2),math.cos(5.0*i/60+dd),math.sin(2.0*i/60.0)], radius=math.fabs(0.2*math.cos(10.0*i/60)))
    a=Union(a,b)
dd=0.0
for i in range(0,60):
        dd+=0.05
        b=Sphere(center=[math.sin(3.0*i/60.0*2),dd+math.cos(5.0*i/60),math.cos(2.0*i/60.0)], radius=math.fabs(0.1*math.cos(1.0*i/60)))
        a=Union(a,b)
for i in range(0,60):
                dd+=0.05
                b=Sphere(center=[0.5*math.sin(3.0*i/60.0*2)+0.5*math.cos(5.0*i/60),dd+math.cos(5.0*i/60),math.cos(2.0*i/60.0)], radius=math.fabs(0.2))
                a=Union(a,b)
dd=a

s = """
ImplicitShape : {
name : "shape1"
Difference : {
    Sphere : {
        center : [0.0,0.0,0.0]
        radius : 1.0
    }
    Union :{
    Union :{
        Sphere : {
            center : [-1.0,0.0,0.0]
            radius : 0.7
        }
        Sphere : {
            center : [1.0,0.0,0.0]
            radius : 0.7
        }
    }
    Union :{
        Sphere : {
            center : [0.0,1.0,0.0]
            radius : 0.6
        }
        Sphere : {
            center : [0,-1.0,0.0]
            radius : 0.6
        }
    }
    }
}
}
"""

def loadShape(root):
    #global d
    #d=minipsl.parse(root, s)
    None

def getCythonFunction():
    return dfgeom.getCythonRawFunction(dd)

def evalField(x, y, z):
        return dfgeom.evalField(dd, x, y, z)
