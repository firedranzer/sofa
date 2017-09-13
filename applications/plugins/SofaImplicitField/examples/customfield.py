import pyximport; pyximport.install()
import cythonfield

import primitives
import accordion

import math

class Sphere(object):
	def __init__(self, position=[0,0,0], radius=0.5):
		self.position = position
		self.radius = radius

	def eval(self, x,y,z):
		x = x - self.position[0]
		y = y - self.position[1]
		z = z - self.position[2]

		return math.sqrt( (x*x + y*y + z*z) ) -self.radius

class Difference(object):
	def __init__(self, a, b):
		self.left = a 
		self.right = b

	def eval(self, x,y,z):
		leftv = self.left.eval(x,y,z)
		rightv = self.right.eval(x,y,z)
		return max(leftv, -rightv) 		
#
#def evalField(x,y,z):
#        #f=Difference( Sphere([0.5, 0.5, 0.5], 0.2),
#        #			  Sphere([0.3, 0.5, 0.5], 0.2) )
#        f=Sphere([0.2,0.5,0.5], 0.3)#
#	return f.eval(x,y,z)

def evalField(x,y,z):
        p=primitives.Point(x,y,z)
        niceaccordion=accordion.accordionUniform(1.0,0.2,0.01, "ellipsoid",5,0.6,0.6,0.5)


        listheigthsJoiningPoints=[0.3,0.7,0.9]
        listAxesX=[0.5,0.5,0.5,0.5]
        listAxesY=[0.5,0.5,0.5,0.5]
        Zrecovering=0.1
        listcavities=[[0.25,"frisbee",0.5,0.5,0.2],[0.75,"frisbee",0.5,0.5,0.2],[0.5,"frisbee",0.5,0.5,0.2]]
#        niceaccordion=accordion.accordionFreeDimension(1.0,0.2,0.01, listcavities)
#        niceaccordion=accordion.accordionRecoveringGiven(1.0,0.2,0.01,"frisbee",listheigthsJoiningPoints, listAxesX, listAxesY,Zrecovering)
        return niceaccordion.eval(p)
