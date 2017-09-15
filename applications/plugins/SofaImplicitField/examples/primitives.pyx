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
listgradientDxPrimitives=[]
listgradientDyPrimitives=[]
listgradientDzPrimitives=[]

cpdef getListAandB():
    return listA, listB


cpdef getListPrimitives():

    return listPrimitives

cpdef getListgradientDxPrimitivess():

    return listgradientDxPrimitives

cpdef getListgradientDyPrimitivess():

    return listgradientDyPrimitives

cpdef getListgradientDzPrimitivess():

    return listgradientDzPrimitives


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

cpdef int generateNewPrimitives():
            global j
            cdef int temp=j
            j+=1
            return temp


cdef class Primitives(Shape):

    def __init__(self, sign, axisX, axisY, axisZ, double theta, double phi, center):

        self.sign=sign
        self.axisX=axisX
        self.axisY=axisY
        self.axisZ=axisZ
        self.theta=theta
        self.phi=phi

        self.cosTheta=cos(theta)
        self.sinTheta=sin(theta)
        self.cosPhi=cos(phi)

        self.sinPhi=sin(phi)
        self.center=center
        index=generateNewPrimitives()
        self.index=index
        self.type
        self.identifier

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


#        if self.cosTheta==0.0:

#            x1="(-"+minus("y",self.center.y)+")"
#            y=minus("x",self.center.x)


#        elif self.cosTheta==1.0:

#            x1=minus("x",self.center.x)
#            y=minus("y",self.center.y)


#        else:

        x1="(cosTheta"+str(self.index)+"*(x-center"+str(self.index)+".x)-sinTheta"+str(self.index)+"*(y-center"+str(self.index)+".y))"
        y="(sinTheta"+str(self.index)+"*(x-center"+str(self.index)+".x)+cosTheta"+str(self.index)+"*(y-center"+str(self.index)+".y)))"

#        if self.cosPhi==0.0:

#           x="(-"+minus("z",self.center.z)+")"
#           z=x1

#        elif self.cosPhi==1.0:

#            x=x1
#            z=minus("z",self.center.z)

#        else:

        x="cosPhi"+str(self.index)+"*"+x1+"-sinPhi"+str(self.index)+"*(z-center"+str(self.index)+".z))"
        z="(sinPhi"+str(self.index)+"*"+x1+"+cosPhi"+str(self.index)+"*(z-center"+str(self.index)+".z)))"

        return (x,y,z)


    cpdef gradXtranslationRotationToString(self):

#        (dxX,dxY,dxZ)=(multiply(str(self.cosPhi),str(self.cosTheta)), str(self.sinTheta),multiply(str(self.sinPhi),str(self.cosTheta)))
        (dxX,dxY,dxZ)=("cosPhi"+str(self.index)+"*"+"cosTheta"+str(self.index), "sinTheta"+str(self.index),"sinPhi"+str(self.index)+"*"+"cosTheta"+str(self.index))

        listgradientDxPrimitives.append("dxX"+str(self.index)+"="+dxX+"\n"+"dxY"+str(self.index)+"="+dxY+"\n"+"dxZ"+str(self.index)+"="+dxZ+"\n\n\n")
        return ("dxX"+str(self.index),"dxY"+str(self.index),"dxZ"+str(self.index))

    cpdef gradYtranslationRotationToString(self):

        (dyX,dyY,dyZ)=("(-cosPhi"+str(self.index)+")*sinTheta"+str(self.index), "cosTheta"+str(self.index),"-sinPhi"+str(self.index)+"*"+"sinTheta"+str(self.index))

        listgradientDyPrimitives.append("dyX"+str(self.index)+"="+dyX+"\n"+"dyY"+str(self.index)+"="+dyY+"\n"+"dyZ"+str(self.index)+"="+dyZ+"\n\n\n")
        return ("dyX"+str(self.index),"dyY"+str(self.index),"dyZ"+str(self.index))

    cpdef gradZtranslationRotationToString(self):

        (dzX,dzY,dzZ)=("sinPhi"+str(self.index), str(0.0),"cosPhi"+str(self.index))

        listgradientDzPrimitives.append("dzX"+str(self.index)+"="+dzX+"\n"+"dzY"+str(self.index)+"="+dzY+"\n"+"dzZ"+str(self.index)+"="+dzZ+"\n\n\n")
        return ("dzX"+str(self.index),"dzY"+str(self.index),"dzZ"+str(self.index))


cdef class Ellipsoid(Primitives):


    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):

        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="ellipsoid"
        identifier=[self.type,self.sign, self.axisX,self.axisY,self.axisZ,self.theta,self.phi,(self.center.x,self.center.y,self.center.z),self.index]
        self.identifier=identifier
        listPrimitives.append(identifier)


    cpdef double eval(self,Point point):

        point=self.translationRotation(point)

        return (point.x/self.axisX)**2+(point.y/self.axisY)**2+(point.z/self.axisZ)**2-1

    cpdef str gradX(self):

        (x,y,z)=self.translationRotationToString()
        (dxX,dxY,dxZ)=self.gradXtranslationRotationToString()

#        add(add(multiply(dxX,divide("2.0*"+x,self.axisX**2)),multiply(dxY,divide("2.0*"+y,self.axisY**2))),\
#        multiply(dxZ,divide("2.0*"+z,self.axisZ**2)))

        return "("+dxX+"*2.0*"+x+"/axis"+str(self.index)+"X*axis"+str(self.index)+"X)"+"+"+"("+dxY+"*2.0*"+y+"/axis"+str(self.index)+"Y*axis"+str(self.index)+"Y)"+"+"+\
                "("+dxZ+"*2.0*"+z+"/axis"+str(self.index)+"Z*axis"+str(self.index)+"Z)"


    cpdef str gradY(self):

        (x,y,z)=self.translationRotationToString()
        (dyX,dyY,dyZ)=self.gradYtranslationRotationToString()

#         add(add(multiply(dyX,divide("2.0*"+x,self.axisX**2)),multiply(dyY,divide("2.0*"+y,self.axisY**2))),\
#         multiply(dyZ,divide("2.0*"+z,self.axisZ**2)))

        return "("+dyX+"*2.0*"+x+"/axis"+str(self.index)+"X*axis"+str(self.index)+"X)"+"+"+"("+dyY+"*2.0*"+y+"/axis"+str(self.index)+"Y*axis"+str(self.index)+"Y)"+"+"+\
                "("+dyZ+"*2.0*"+z+"/axis"+str(self.index)+"Z*axis"+str(self.index)+"Z)"

    cpdef str gradZ(self):

        (x,y,z)=self.translationRotationToString()
        (dzX,dzY,dzZ)=self.gradZtranslationRotationToString()

#        add(add(multiply(dzX,divide("2.0*"+x,self.axisX**2)),multiply(dzY,divide("2.0*"+y,self.axisY**2))),\
#        multiply(dzZ,divide("2.0*"+z,self.axisZ**2)))

        return "("+dzX+"*2.0*"+x+"/axis"+str(self.index)+"X*axis"+str(self.index)+"X)"+"+"+"("+dzY+"*2.0*"+y+"/axis"+str(self.index)+"Y*axis"+str(self.index)+"Y)"+"+"+\
            "("+dzZ+"*2.0*"+z+"/axis"+str(self.index)+"Z*axis"+str(self.index)+"Z)"

    cpdef tuple grad(self):
        return (self.gradX(),self.gradY(),self.gradZ())

    cpdef tuple toString(self):

        (x,y,z)=self.translationRotationToString()

        expression="("+x+"/axis"+str(self.index)+"X)*("+x+"/axis"+str(self.index)+"X)"+"+"+"("+y+"/axis"+str(self.index)+"Y)*("+y+"/axis"+str(self.index)+"Y)"+"+"\
                  +"("+z+"/axis"+str(self.index)+"Z)*("+z+"/axis"+str(self.index)+"Z)"

#        divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+divide(y,self.axisY)+"*"+divide(y,self.axisY)+"+"+divide(z,self.axisZ)+"*"+\
#        divide(z,self.axisZ)+"-1.0"

        return (expression, self.grad())

cdef class Frisbee(Primitives):

    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):
        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="frisbee"
        identifier=[self.type,self.sign, self.axisX,self.axisY,self.axisZ,self.theta,self.phi,(self.center.x,self.center.y,self.center.z),self.index]
        self.identifier=identifier
        listPrimitives.append(identifier)

    cpdef double eval(self,Point point):

        point=self.translationRotation(point)

        return abs(point.z/self.axisZ)+math.sqrt((point.x/self.axisX)**2+(point.y/self.axisY)**2)-1


    cpdef str gradX(self):

        (x,y,z)=self.translationRotationToString()
        (dxX,dxY,dxZ)=self.gradXtranslationRotationToString()

        return dxX+"*("+x+"/sqrt(("+x+"/axis"+str(self.index)+"X)"+"*"+"("+x+"/axis"+str(self.index)+"X)"+"+"+"("+y+"/axis"+str(self.index)+"Y)"+"*"+"("+y+"/axis"+str(self.index)+"Y)"+")"\
              +dxY+"*("+y+"/sqrt(("+x+"/axis"+str(self.index)+"X)"+"*"+"("+x+"/axis"+str(self.index)+"X)"+"+"+"("+y+"/axis"+str(self.index)+"Y)"+"*"+"("+y+"/axis"+str(self.index)+"Y)"+")"\
              +dxZ+"*"+"sign("+z+")/axis"+str(self.index)+"Z)"

    cpdef str gradY(self):

        (x,y,z)=self.translationRotationToString()
        (dyX,dyY,dyZ)=self.gradXtranslationRotationToString()

        return dyX+"*("+x+"/sqrt(("+x+"/axis"+str(self.index)+"X)"+"*"+"("+x+"/axis"+str(self.index)+"X)"+"+"+"("+y+"/axis"+str(self.index)+"Y)"+"*"+"("+y+"/axis"+str(self.index)+"Y)"+")"\
              +dyY+"*("+y+"/sqrt(("+x+"/axis"+str(self.index)+"X)"+"*"+"("+x+"/axis"+str(self.index)+"X)"+"+"+"("+y+"/axis"+str(self.index)+"Y)"+"*"+"("+y+"/axis"+str(self.index)+"Y)"+")"\
              +dyZ+"*"+"sign("+z+")/axis"+str(self.index)+"Z)"


    cpdef str gradZ(self):

        (x,y,z)=self.translationRotationToString()
        (dzX,dzY,dzZ)=self.gradXtranslationRotationToString()

        return dzX+"*("+x+"/sqrt(("+x+"/axis"+str(self.index)+"X)"+"*"+"("+x+"/axis"+str(self.index)+"X)"+"+"+"("+y+"/axis"+str(self.index)+"Y)"+"*"+"("+y+"/axis"+str(self.index)+"Y)"+")"\
              +dzY+"*("+y+"/sqrt(("+x+"/axis"+str(self.index)+"X)"+"*"+"("+x+"/axis"+str(self.index)+"X)"+"+"+"("+y+"/axis"+str(self.index)+"Y)"+"*"+"("+y+"/axis"+str(self.index)+"Y)"+")"\
              +dzZ+"*"+"sign("+z+")/axis"+str(self.index)+"Z)"



    cpdef tuple grad(self):
        return (self.gradX(),self.gradY(),self.gradZ())

    cpdef tuple toString(self):

        (x,y,z)=self.translationRotationToString()

        expression="abs("+z+"/axis"+str(self.index)+"Z)"+"+"\
                    +"sqrt(("+x+"/axis"+str(self.index)+"X)"+"*"+"("+x+"/axis"+str(self.index)+"X)"\
                    +"+"+"("+y+"/axis"+str(self.index)+"Y)"+"*"+"("+y+"/axis"+str(self.index)+"Y)"+")-1.0"

        return (expression, self.grad())


cdef class Cylinder(Primitives):


    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):
        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="cylinder"
        (x,y,z)=self.translationRotationToString()

        self.radial="("+x+"/axis"+str(self.index)+"X)"+"("+x+"/axis"+str(self.index)+"X)"+"+"+"("+y+"/axis"+str(self.index)+"Y)"+"("+y+"/axis"+str(self.index)+"Y)"+"-1.0"
        self.height="abs("+z+"-axis"+str(self.index)+"Z)"
        identifier=[self.type,self.sign, self.axisX,self.axisY,self.axisZ,self.theta,self.phi,(self.center.x,self.center.y,self.center.z),self.radial,self.height,self.index]
        self.identifier=identifier
        listPrimitives.append(identifier)

    cpdef double eval(self,Point point):
        point=self.translationRotation(point)

        return max(abs(point.z)-self.axisZ,(point.x/self.axisX)**2+(point.y/self.axisY)**2-1)


    cpdef str gradX(self):

        (x,y,z)=self.translationRotationToString()
        (dxX,dxY,dxZ)=self.gradXtranslationRotationToString()

        return dxX+"*ind(radial,height)"+"*2.0*"+x+"/(axis"+str(self.index)+"X"+"*"+"axis"+str(self.index)+"X)"\
              +dxY+"*ind(radial,height)"+"*2.0*"+y+"/(axis"+str(self.index)+"Y"+"*"+"axis"+str(self.index)+"Y)"\
              +dxZ+"*ind(height"+str(self.index)+",radial"+str(self.index)+")*sign("+z+")"

    cpdef str gradY(self):

        (x,y,z)=self.translationRotationToString()
        (dyX,dyY,dyZ)=self.gradXtranslationRotationToString()

        return dyX+"*ind(radial,height)"+"*2.0*"+x+"/(axis"+str(self.index)+"X"+"*"+"axis"+str(self.index)+"X)"\
              +dyY+"*ind(radial,height)"+"*2.0*"+y+"/(axis"+str(self.index)+"Y"+"*"+"axis"+str(self.index)+"Y)"\
              +dyZ+"*ind(height"+str(self.index)+",radial"+str(self.index)+")*sign("+z+")"

    cpdef str gradZ(self):

        (x,y,z)=self.translationRotationToString()
        (dzX,dzY,dzZ)=self.gradXtranslationRotationToString()

        return dzX+"*ind(radial,height)"+"*2.0*"+x+"/(axis"+str(self.index)+"X"+"*"+"axis"+str(self.index)+"X)"\
              +dzY+"*ind(radial,height)"+"*2.0*"+y+"/(axis"+str(self.index)+"Y"+"*"+"axis"+str(self.index)+"Y)"\
              +dzZ+"*ind(height"+str(self.index)+",radial"+str(self.index)+")*sign("+z+")"

    cpdef tuple grad(self):
        return (self.gradX(),self.gradY(),self.gradZ())

    cpdef tuple toString(self):

        (x,y,z)=self.translationRotationToString()

        expression="max(height"+str(self.index)+",radial"+str(self.index)+")"

        return (expression, self.grad())

