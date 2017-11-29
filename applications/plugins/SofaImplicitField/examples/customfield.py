import sys
import SofaPython
from SofaPython import Tools
#sys.path.append( "../../python" )
import difigeometry

import math
from math import sqrt

idx = 0

class Sphere(object):
    def __init__(self, position=[0,0,0], radius=0.5):
        global idx
        self.name = "Sphere"+str(idx)
        self.position = position
        self.radius = radius
        idx += 1

    def eval(self, x,y,z):
	x = x - self.position[0]
	y = y - self.position[1]
	z = z - self.position[2]
	return math.sqrt( (x*x + y*y + z*z) ) - self.radius

    def toGLSL(self, context):
        context["position"+self.name] = ("position"+self.name, "vec3", "uniform vec3 position"+self.name+";", SofaPython.Tools.listToStr(self.position))
        context["radius"+self.name] = ("radius"+self.name, "float", "uniform float radius"+self.name+";", str(self.radius))
        return "sdSphere(pos, position"+self.name+", radius"+self.name+")"  

class Difference(object):
    def __init__(self, a, b):
		self.left = a 
		self.right = b

    def eval(self, x,y,z):
		leftv = self.left.eval(x,y,z)
		rightv = self.right.eval(x,y,z)
		return max(leftv, -rightv) 		

    def toGLSL(self, context):
        left = self.left.toGLSL(context)
        right = self.right.toGLSL(context)
        return "sdDifference("+left+","+right+")"

shape = Difference(Sphere([0.2,0.0,0.0], 1.0),
                   Sphere([0.5,0.0,0.0], 1.0))

def glslFunction():
    context = {}
    function = shape.toGLSL(context)
    return (context.values(), function);

def evalField(x,y,z):
    return shape.eval(x,y,z)
