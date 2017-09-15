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

listA=[]
listB=[]
listPrimitives=[]

cdef int i=0
cdef int j=0

cpdef tuple generateNewAandB():
    global i
    cdef str a="a"+str(i)
    cdef str b="b"+str(i)
    i=i+1
    return (a,b)

cpdef str ind():

    temp="cdef double ind(double a, double b):\n"\
         +"     if a>b:\n"\
         +"         return 1.0\n\n"\
         +"     elif a<b:\n"\
         +"         return 0.0\n\n"\
         +"     else:\n"\
         +"         return 0.5\n\n"

    return temp


cpdef str sign():

    temp="cdef double sing(double x):\n"\
         +"     if x>0.0:\n"\
         +"         return 1.0\n\n"\
         +"     else:\n"\
         +"         return -1.0\n\n"

    return temp


cpdef str minus(str expression, double x):
    if x==0.0:
        return expression
    else:
        return "("+expression+"-"+str(x)+")"

cpdef str divide(str expression, double x):
    if x==1.0:
        return expression
    else:
        return "("+expression+"/"+str(x)+")"

cpdef str multiply(str expression1,str expression2):

    if expression1=="0.0" or expression2=="0.0" or expression1=="-0.0" or expression2=="-0.0" or expression1=="" or expression2=="":

            return ""

    elif expression1=="1.0":

        return expression2

    elif expression2=="1.0":

        return expression1

    else:

        return "("+expression1+"*"+expression2+")"

cpdef str add(str expression1,str expression2):

    if expression1=="" or expression1=="0.0":

        if expression2!="0.0":
            return expression2
        else:
            return ""

    else:

        if expression2=="0.0" or expression2=="":
            return expression1
        else:
            return "("+expression1+"+"+expression2+")"

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

    cpdef tuple toString(self):
        return self.toString()

cdef class Union(Shape):

    def __init__(self, first, second):
        self.first=first
        self.second=second

    cpdef double eval(self,Point point):
        return min(self.first.eval(point),self.second.eval(point))


    cpdef tuple toString(self):

        (expression1,(grad1X,grad1Y,grad1Z))=self.first.toString()
        (expression2,(grad2X,grad2Y,grad2Z))=self.second.toString()

        (a,b)=generateNewAandB()
        listA.append(a+"="+expression1)
        listB.append(b+"="+expression2)

        expression="min("+a+","+b+")"



        gradX="((1.0-ind("+a+","+b+"))*"+grad1X+"ind("+a+","+b+")*"+grad2X+")"
        gradY="((1.0-ind("+a+","+b+"))*"+grad1Y+"ind("+a+","+b+")*"+grad2Y+")"
        gradZ="((1.0-ind("+a+","+b+"))*"+grad1Z+"ind("+a+","+b+")*"+grad2Z+")"

        return (expression,(gradX,gradY,gradZ))


cdef class Intersection(Shape):

    def __init__(self, first, second):
        self.first=first
        self.second=second

    cpdef double eval(self,Point point):
        return max(self.first.eval(point),self.second.eval(point))

    cpdef tuple toString(self):

        (expression1,(grad1X,grad1Y,grad1Z))=self.first.toString()
        (expression2,(grad2X,grad2Y,grad2Z))=self.second.toString()

        (a,b)=generateNewAandB()
        listA.append(a+"="+expression1)
        listB.append(b+"="+expression2)

        expression="max("+a+","+b+")"

        gradX="(ind("+a+","+b+")*"+grad1X+"(1.0-ind("+a+","+b+"))*"+grad2X+")"
        gradY="(ind("+a+","+b+")*"+grad1Y+"(1.0-ind("+a+","+b+"))*"+grad2Y+")"
        gradZ="(ind("+a+","+b+")*"+grad1Z+"(1.0-ind("+a+","+b+"))*"+grad2Z+")"

        return (expression,(gradX,gradY,gradZ))

cdef class Difference(Shape):

    def __init__(self, first, second):
        self.first=first
        self.second=second

    cpdef double eval(self,Point point):
        return max(self.first.eval(point),-self.second.eval(point))


    cpdef tuple toString(self):

        (expression1,(grad1X,grad1Y,grad1Z))=self.first.toString()
        (expression2,(grad2X,grad2Y,grad2Z))=self.second.toString()

        (a,b)=generateNewAandB()
        listA.append(a+"="+expression1)
        listB.append(b+"="+expression2)

        expression="max("+a+",-"+b+")"

        gradX="(ind("+a+",-"+b+")*"+grad1X+"(1.0-ind("+a+",-"+b+"))*(-("+grad2X+")))"
        gradY="(ind("+a+",-"+b+")*"+grad1Y+"(1.0-ind("+a+",-"+b+"))*(-("+grad2Y+")))"
        gradZ="(ind("+a+",-"+b+")*"+grad1Z+"(1.0-ind("+a+",-"+b+"))*(-("+grad2Z+")))"

        return (expression,(gradX,gradY,gradZ))

cpdef tuple generateNewPrimitives():
            global j
            j+=1
            return j


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
        index=generateNewPrimitives()
        self.index=index
        identifier=["pmt"+str(index),sign, axisX,axisY,axisZ,theta,phi,(self.center.x,self.center.y,self.center.z),index]
        self.identifier=identifier
        listPrimitives.append(identifier)

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

        if self.cosTheta==0.0:

            x1="(-"+minus("y",self.center.y)+")"
            y=minus("x",self.center.x)


        elif self.cosTheta==1.0:

            x1=minus("x",self.center.x)
            y=minus("y",self.center.y)


        else:

            x1="("+str(self.cosTheta)+"*(x-"+str(self.center.x)+")-"+str(self.sinTheta)+"*(y-"+str(self.center.y)+"))"
            y="("+str(self.sinTheta)+"*(x-"+str(self.center.x)+")+"+str(self.cosTheta)+"*(y-"+str(self.center.y)+"))"

        if self.cosPhi==0.0:

           x="(-"+minus("z",self.center.z)+")"
           z=x1

        elif self.cosPhi==1.0:

            x=x1
            z=minus("z",self.center.z)

        else:
            x="("+str(self.cosPhi)+"*"+x1+"-"+str(self.sinPhi)+"*(z-"+str(self.center.z)+"))"
            z="("+str(self.sinPhi)+"*"+x1+"+"+str(self.cosPhi)+"*(z-"+str(self.center.z)+"))"

        return (x,y,z)


    cpdef gradXtranslationRotationToString(self):

        (dxX,dxY,dxZ)=(multiply(str(self.cosPhi),str(self.cosTheta)), str(self.sinTheta),multiply(str(self.sinPhi),str(self.cosTheta)))

        return (dxX,dxY,dxZ)

    cpdef gradYtranslationRotationToString(self):

        (dyX,dyY,dyZ)=(multiply(str(-self.cosPhi),str(self.sinTheta)), str(self.cosTheta),multiply(str(-self.sinPhi),str(self.sinTheta)))

        return (dyX,dyY,dyZ)

    cpdef gradZtranslationRotationToString(self):

        (dzX,dzY,dzZ)=(str(-self.sinPhi), str(0.0),str(self.cosPhi))

        return (dzX,dzY,dzZ)




cdef class Ellipsoid(Primitives):


    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):

        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="ellipsoid"


    cpdef double eval(self,Point point):

        point=self.translationRotation(point)

        return (point.x/self.axisX)**2+(point.y/self.axisY)**2+(point.z/self.axisZ)**2-1

    cpdef str gradX(self):

        (x,y,z)=self.translationRotationToString()
        (dxX,dxY,dxZ)=self.gradXtranslationRotationToString()

        return add(add(multiply(dxX,divide("2.0*"+x,self.axisX**2)),multiply(dxY,divide("2.0*"+y,self.axisY**2))),\
               multiply(dxZ,divide("2.0*"+z,self.axisZ**2)))

    cpdef str gradY(self):

        (x,y,z)=self.translationRotationToString()
        (dyX,dyY,dyZ)=self.gradYtranslationRotationToString()

        return add(add(multiply(dyX,divide("2.0*"+x,self.axisX**2)),multiply(dyY,divide("2.0*"+y,self.axisY**2))),\
               multiply(dyZ,divide("2.0*"+z,self.axisZ**2)))

    cpdef str gradZ(self):

        (x,y,z)=self.translationRotationToString()
        (dzX,dzY,dzZ)=self.gradZtranslationRotationToString()

        return add(add(multiply(dzX,divide("2.0*"+x,self.axisX**2)),multiply(dzY,divide("2.0*"+y,self.axisY**2))),\
               multiply(dzZ,divide("2.0*"+z,self.axisZ**2)))

    cpdef tuple grad(self):
        return (self.gradX(),self.gradY(),self.gradZ())

    cpdef tuple toString(self):

        (x,y,z)=self.translationRotationToString()

        expression=divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+divide(y,self.axisY)+"*"+divide(y,self.axisY)+"+"+divide(z,self.axisZ)+"*"+\
             divide(z,self.axisZ)+"-1.0"

        return (expression, self.grad())

cdef class Frisbee(Primitives):

    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):
        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="frisbee"

    cpdef double eval(self,Point point):

        point=self.translationRotation(point)

        return abs(point.z/self.axisZ)+math.sqrt((point.x/self.axisX)**2+(point.y/self.axisY)**2)-1


    cpdef str gradX(self):

        (x,y,z)=self.translationRotationToString()

        return "("+x+"/sqrt("+divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+divide(y,self.axisY)+"*"+divide(y,self.axisY)+")"

    cpdef str gradY(self):

        (x,y,z)=self.translationRotationToString()

        return "("+y+"/sqrt("+divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+divide(y,self.axisY)+"*"+divide(y,self.axisY)+")"


    cpdef str gradZ(self):

        (x,y,z)=self.translationRotationToString()

        return divide("sign("+z+")",self.axisZ)


    cpdef tuple grad(self):
        return (self.gradX(),self.gradY(),self.gradZ())

    cpdef tuple toString(self):

        (x,y,z)=self.translationRotationToString()

        expression="abs("+divide(z,self.axisZ)+")+sqrt("+divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+\
             divide(y,self.axisY)+"*"+divide(y,self.axisY)+")-1.0"

        return (expression, self.grad())

#cdef class Ball(Primitives):

#    def __init__(self,sign,radius,center):
#        Primitives.__init__(self,sign,radius,radius,radius,0,0,center)
#        self.type="ball"


#    cpdef double eval(self,Point point):
#        return ((point.x**2+point.y**2+point.z**2)/self.axisX**2)-1.0


#    cpdef str toString(self):

#        (x,y,z)=self.translationRotationToString()

#        return "(("+x+"*"+x+y+"*"+y+z+"*"+z+")/"+str(self.axisX)+"*"+str(self.axisX)+")-1.0"

#    cpdef tuple grad(self):
#        return (self.gradX(),self.gradY(),self.gradZ())

cdef class Cylinder(Primitives):


    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):
        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="cylinder"


    cpdef double eval(self,Point point):
        point=self.translationRotation(point)

        return max(abs(point.z)-self.axisZ,(point.x/self.axisX)**2+(point.y/self.axisY)**2-1)


    cpdef str gradX(self):

        (x,y,z)=self.translationRotationToString()

        return "ind("+divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+divide(y,self.axisY)+"*"+divide(y,self.axisY)+"-1.0,"\
                +"abs("+minus(z,self.axisZ)+"))*"+divide("2.0*"+x,self.axisX*self.axisX)

    cpdef str gradY(self):

        (x,y,z)=self.translationRotationToString()

        return "ind("+divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+divide(y,self.axisY)+"*"+divide(y,self.axisY)+"-1.0,"\
                +"abs("+minus(z,self.axisZ)+"))*"+divide("2.0*"+y,self.axisY*self.axisY)

    cpdef str gradZ(self):

        (x,y,z)=self.translationRotationToString()
        return "ind(abs("+minus(z,self.axisZ)+","+divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+divide(y,self.axisY)+"*"+divide(y,self.axisY)+\
               "-1.0))*sign("+z+")"

    cpdef tuple grad(self):
        return (self.gradX(),self.gradY(),self.gradZ())

    cpdef tuple toString(self):

        (x,y,z)=self.translationRotationToString()

        expression="max(abs("+minus(z,self.axisZ)+"),"+divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+\
        divide(y,self.axisY)+"*"+divide(y,self.axisY)+"-1.0)"

        return (expression, self.grad())

cdef point=Point(2.0,3.0,1.5)

cpdef getList():

    return listA, listB

if __name__ == '__main__':

#    print("A simple demo")
#    b=Ball('+', 1.0, Point(0.0,0.0,0.0))
#    print("ball: "+str(b.eval(Point(0.0,0.0,0.0))))
#    print("ball: "+str(b.eval(Point(2.0,0.0,0.0))))
#    print("ball: "+str(b.eval(Point(-2.0,0.0,0.0))))

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

    print "union: ", Union(c,e).eval(Point(1.0,1.0,1.0))
    print "intersection: ", Intersection(e,c).eval(Point(1.0,1.0,1.0))
    print "difference: ", Difference(c,f).eval(Point(1.0,1.0,1.0))

