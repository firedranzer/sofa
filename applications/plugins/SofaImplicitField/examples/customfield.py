import math
from math import sqrt

class Sphere(object):
	def __init__(self, position=[0,0,0], radius=0.5):
		self.position = position
		self.radius = radius

	def eval(self, x,y,z):
		x = x - self.position[0]
		y = y - self.position[1]
		z = z - self.position[2]

		return math.sqrt( (x*x + y*y + z*z) ) - self.radius

class Difference(object):
	def __init__(self, a, b):
		self.left = a 
		self.right = b

	def eval(self, x,y,z):
		leftv = self.left.eval(x,y,z)
		rightv = self.right.eval(x,y,z)
		return max(leftv, -rightv) 		

def evalField(x,y,z):
        s1=Sphere([0.0,0.0,0.0], 1.0)
        s2=Sphere([0.5,0.0,0.0], 1.0)
        return Difference(s1,s2).eval(x,y,z)
