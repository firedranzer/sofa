#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - thomas.morzadec@inria.fr
##
####################################################################################################

# distutils: language=c++
# cython: profile=True

import numpy
import math
from libc.math cimport sin, cos, acos, exp, sqrt, fabs, M_PI
cimport numpy
cimport cython
cimport primitives

cdef class Point(object):

    def __init__(self, x, y, z):
        self.x=x
        self.y=y
        self.z=z

    cdef display(self,):
        print "("+str(self.x),str(self.y),str(self.z)+")"
        return

cdef class Shape(object):

    cpdef double eval(self, Point point):
        return self.eval(point)

    cpdef str toString(self):
        return self.toString()

    cpdef str gradX(self):
        return self.gradX()

    cpdef str gradY(self):
        return self.gradY()

    cpdef str gradZ(self):
        return self.gradZ()

cdef class Union(Shape):

    def __init__(self, first, second):
        self.first=first
        self.second=second

    cpdef double eval(self,Point point):
        return min(self.first.eval(point),self.second.eval(point))

    cpdef str toString(self):
        return "min("+self.first.toString()+","+self.second.toString()+")"

    cpdef str indPos(self):
        """
        This method return the Indicatrice function of first-second>0
        """
        temp="(max(("+self.first.toString()+")-("+self.second.toString()+"),0.0)/(max(("+self.first.toString()+")-("+self.second.toString()\
            +"),0.0)+max(("+self.second.toString()+")-("+self.first.toString()+"),0.0)+min(("+self.first.toString()+")-("+self.second.toString()\
            +"),-1.0)))"

        return temp
    cpdef str gradX(self):

        temp="(1.0-"+self.indPos()+")*"+self.first.gradX()\
            +"+"+self.indPos()+"*"+self.second.gradX()
        return temp

    cpdef str gradY(self):

        temp="(1.0-"+self.indPos()+")*"+self.first.gradY()\
            +"+"+self.indPos()+"*"+self.second.gradY()
        return temp

    cpdef str gradZ(self):

        temp="(1.0-"+self.indPos()+")*"+self.first.gradZ()\
            +"+"+self.indPos()+"*"+self.second.gradZ()
        return temp



cdef class Intersection(Shape):

    def __init__(self, first, second):
        self.first=first
        self.second=second

    cpdef double eval(self,Point point):
        return max(self.first.eval(point),self.second.eval(point))

    cpdef str toString(self):
        return "max("+self.first.toString()+","+self.second.toString()+")"

    cpdef str indPos(self):
        """
        This method return the Indicatrice function of first-second>0
        """
        temp="(max(("+self.first.toString()+")-("+self.second.toString()+"),0.0)/(max(("+self.first.toString()+\
             ")-("+self.second.toString()+"),0.0)+max(("+self.second.toString()+")-("+self.first.toString()+\
             "),0.0)+min(("+self.first.toString()+")-("+self.second.toString()+"),-1.0)))"

        return temp

    cpdef str gradX(self):

        temp=self.indPos()+"*"+self.first.gradX()\
            +"+(1.0-"+self.indPos()+")*"+self.second.gradX()
        return temp

    cpdef str gradY(self):

        temp=self.indPos()+"*"+self.first.gradY()\
            +"+(1.0-"+self.indPos()+")*"+self.second.gradY()
        return temp

    cpdef str gradZ(self):

        temp=self.indPos()+"*"+self.first.gradZ()\
            +"+(1.0-"+self.indPos()+")*"+self.second.gradZ()
        return temp

cdef class Difference(Shape):


    def __init__(self, first, second):
        self.first=first
        self.second=second

    cpdef double eval(self,Point point):
        return max(self.first.eval(point),-self.second.eval(point))

    cpdef str toString(self):
        return "max("+self.first.toString()+",-"+self.second.toString()+")"

    cpdef str indPosDiff(self):
        """
        This method return the Indicatrice function of first-second>0
        """
        temp="(max(("+self.first.toString()+")+("+self.second.toString()+"),0.0)/(max(("+self.first.toString()+\
             ")+("+self.second.toString()+"),0.0)+max(-("+self.second.toString()+")+("+self.first.toString()+\
             "),0.0)+min(("+self.first.toString()+")+("+self.second.toString()+"),-1.0)))"

        return temp

    cpdef str gradX(self):

        temp=self.indPosDiff()+"*"+self.first.gradX()\
            +"+(1.0-"+self.indPosDiff()+")*(-"+self.second.gradX()+")"
        return temp

    cpdef str gradY(self):

        temp=self.indPosDiff()+"*"+self.first.gradY()\
            +"+(1.0-"+self.indPosDiff()+")*(-"+self.second.gradY()+")"
        return temp

    cpdef str gradZ(self):

        temp=self.indPosDiff()+"*"+self.first.gradZ()\
            +"+(1.0-"+self.indPosDiff()+")*(-"+self.second.gradZ()+")"
        return temp


cdef class Primitives(Shape):

    def __init__(self, sign, axisX, axisY, axisZ, double theta, double phi, center):

        self.sign=sign
        self.axisX=axisX
        self.axisY=axisY
        self.axisZ=axisZ
        self.cosTheta=cos(theta)
        self.sinTheta=sin(theta)
        self.cosPhi=cos(phi)
        self.sinPhi=sin(phi)
        self.center=center

    cdef translationRotation(self,Point point):

        cdef double x
        cdef double y
        cdef double z

        x=self.cosTheta*(point.x-self.center.x)-self.sinTheta*(point.y-self.center.y)
        y=self.sinTheta*(point.x-self.center.x)+self.cosTheta*(point.y-self.center.y)

        x=self.cosPhi*point.x-self.sinPhi*(point.z-self.center.z)
        z=self.sinPhi*point.x+self.cosPhi*(point.z-self.center.z)
        point=Point(x,y,z)
        return point

    cpdef translationRotationToString(self):

        x=str(self.cosTheta)+"*(x-"+str(self.center.x)+")-"+str(self.sinTheta)+"*(y-"+str(self.center.y)+")"
        y=str(self.sinTheta)+"*(x-"+str(self.center.x)+")+"+str(self.cosTheta)+"*(y-"+str(self.center.y)+")"

        x=str(self.cosPhi)+"*x-"+str(self.sinPhi)+"*(z-"+str(self.center.z)+")"
        z=str(self.sinPhi)+"*x+"+str(self.cosPhi)+"*(z-"+str(self.center.z)+")"
        return ("("+x+")","("+y+")","("+z+")")


cdef class Ellipsoid(Primitives):


    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):

        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="ellipsoid"


    cpdef double eval(self,Point point):

        point=self.translationRotation(point)

        return (point.x/self.axisX)**2+(point.y/self.axisY)**2+(point.z/self.axisZ)**2-1

    cpdef str toString(self):

        (x,y,z)=self.translationRotationToString()

        temp="("+x+"/"+str(self.axisX)+")*("+x+"/"+str(self.axisX)+")+("+y+"/"+str(self.axisY)+")*("+y+"/"+str(self.axisY)+")+("+z+"/"+str(self.axisZ)+")*("+z+"/"+str(self.axisZ)+")-1.0"

        return temp

cdef class Frisbee(Primitives):

    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):
        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="frisbee"

    cpdef double eval(self,Point point):

        point=self.translationRotation(point)

        return abs(point.z/self.axisZ)+math.sqrt((point.x/self.axisX)**2+(point.y/self.axisY)**2)-1

    cpdef str toString(self):

        (x,y,z)=self.translationRotationToString()
        temp="abs("+z+"/"+str(self.axisZ)+")+sqrt(("+x+"/"+str(self.axisX)+")*("+x+"/"+str(self.axisX)+")+("+y+"/"+str(self.axisY)+")*("+y+"/"+str(self.axisY)+"))-1.0"

        return temp

cdef class Ball(Primitives):

    def __init__(self,sign,radius,center):
        Primitives.__init__(self,sign,radius,radius,radius,0,0,center)
        self.type="ball"


    cpdef double eval(self,Point point):
        return ((point.x**2+point.y**2+point.z**2)/self.axisX**2)-1.0


    cpdef str toString(self):

        (x,y,z)=self.translationRotationToString()

        return "(("+x+"*"+x+y+"*"+y+z+"*"+z+")/"+str(self.axisX)+"*"+str(self.axisX)+")-1.0"

cdef class Cylinder(Primitives):


    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):
        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="cylinder"


    cpdef double eval(self,Point point):
        point=self.translationRotation(point)

        return max(abs(point.z)-self.axisZ,(point.x/self.axisX)**2+(point.y/self.axisY)**2-1)

    cpdef str toString(self):

        (x,y,z)=self.translationRotationToString()

        temp="max(abs("+z+")-"+str(self.axisZ)+","+"("+x+"/"+str(self.axisX)+")*"+"("+x+"/"+str(self.axisX)+")"+"+("+y+"/"+str(self.axisY)+")*"+"("+y+"/"+str(self.axisY)+")-1.0)"
        return temp


cdef point=Point(2.0,3.0,1.5)

if __name__ == '__main__':

    print("A simple demo")
    b=Ball('+', 1.0, Point(0.0,0.0,0.0))
    print("ball: "+str(b.eval(Point(0.0,0.0,0.0))))
    print("ball: "+str(b.eval(Point(2.0,0.0,0.0))))
    print("ball: "+str(b.eval(Point(-2.0,0.0,0.0))))

    e=Ellipsoid('+',1.0,1.0,2.0,0,0,Point(0.0,0.0,0.0))
    print("ellipsoid: "+str(e.eval(Point(0.0,0.0,01.0))))
    print("ellipsoid: "+str(e.eval(Point(0.0,0.0,2.0))))
    print("ellipsoid: "+str(e.eval(Point(-2.0,0.0,4.0))))

    c=Cylinder('+',1.0,1.0,2.0,0,0,Point(0.0,0.0,0.0))
    print("cylinder: "+str(c.eval(Point(0.0,0.0,1.0))))
    print("cylinder: "+str(c.eval(Point(0.0,0.0,2.0))))
    print("cylinder: "+str(c.eval(Point(-2.0,0.0,4.0))))

    f=Frisbee('+',1.0,1.0,2.0,0,0,Point(0.0,0.0,0.0))
    print("frisbee: "+str(f.eval(Point(0.0,0.0,1.0))))
    print("frisbee: "+str(f.eval(Point(0.0,0.0,2.0))))
    print("frisbee: "+str(f.eval(Point(-2.0,0.0,4.0))))

    print "union: ", Union(b,e).eval(Point(1.0,1.0,1.0))
    print "intersection: ", Intersection(e,c).eval(Point(1.0,1.0,1.0))
    print "difference: ", Difference(c,f).eval(Point(1.0,1.0,1.0))

