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
listgradientXTemp=[]
listgradientYTemp=[]
listgradientZTemp=[]
listSqrt=[]

cpdef getListAandB():
    return listA, listB

cpdef getListPrimitives():

    return listPrimitives

cpdef getListgradientDxPrimitives():

    return listgradientDxPrimitives

cpdef getListgradientDyPrimitives():

    return listgradientDyPrimitives

cpdef getListgradientDzPrimitives():

    return listgradientDzPrimitives

cpdef getListgradientXTemp():

    return listgradientXTemp

cpdef getListgradientYTemp():

    return listgradientYTemp

cpdef getListgradientZTemp():

    return listgradientZTemp

cpdef getListSqrt():
    return listSqrt

cdef int i=0
cdef int j=0
cdef int k=0

cpdef str generateNewIndex():
    global i
    cdef str index=str(i)
    i=i+1
    return index

cpdef str generateNewJindex():
            global j
            cdef str temp=str(j)
            j+=1
            return temp

cpdef str generateNewKindex():
            global k
            cdef str temp=str(k)
            k+=1
            return temp

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

    temp="cdef double sign(double x):\n"\
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

        index=generateNewIndex()
        listA.append("A"+index+"="+expression1)
        listB.append("B"+index+"="+expression2)
        listgradientXTemp.append("gradA"+index+"X="+grad1X)
        listgradientYTemp.append("gradA"+index+"Y="+grad1Y)
        listgradientZTemp.append("gradA"+index+"Z="+grad1Z)

        listgradientXTemp.append("gradB"+index+"X="+grad2X)
        listgradientYTemp.append("gradB"+index+"Y="+grad2Y)
        listgradientZTemp.append("gradB"+index+"Z="+grad2Z)

        expression="min(A"+index+",B"+index+")"

        gradX="(1.0-ind(A"+index+",B"+index+"))*gradA"+index+"X"+"+"+"ind(A"+index+",B"+index+")*gradB"+index+"X"
        gradY="(1.0-ind(A"+index+",B"+index+"))*gradA"+index+"Y"+"+"+"ind(A"+index+",B"+index+")*gradB"+index+"Y"
        gradZ="(1.0-ind(A"+index+",B"+index+"))*gradA"+index+"Z"+"+"+"ind(A"+index+",B"+index+")*gradB"+index+"Z"

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

        index=generateNewIndex()
        listA.append("A"+index+"="+expression1)
        listB.append("B"+index+"="+expression2)
        listgradientXTemp.append("gradA"+index+"X="+grad1X)
        listgradientYTemp.append("gradA"+index+"Y="+grad1Y)
        listgradientZTemp.append("gradA"+index+"Z="+grad1Z)

        listgradientXTemp.append("gradB"+index+"X="+grad2X)
        listgradientYTemp.append("gradB"+index+"Y="+grad2Y)
        listgradientZTemp.append("gradB"+index+"Z="+grad2Z)

        expression="max(A"+index+",B"+index+")"

        gradX="ind(A"+index+",B"+index+")*gradA"+index+"X"+"+"+"(1.0-ind(A"+index+",B"+index+"))*gradB"+index+"X"
        gradY="ind(A"+index+",B"+index+")*gradA"+index+"Y"+"+"+"(1.0-ind(A"+index+",B"+index+"))*gradB"+index+"Y"
        gradZ="ind(A"+index+",B"+index+")*gradA"+index+"Z"+"+"+"(1.0-ind(A"+index+",B"+index+"))*gradB"+index+"Z"

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

        index=generateNewIndex()
        listA.append("A"+index+"="+expression1)
        listB.append("B"+index+"="+expression2)
        listgradientXTemp.append("gradA"+index+"X="+grad1X)
        listgradientYTemp.append("gradA"+index+"Y="+grad1Y)
        listgradientZTemp.append("gradA"+index+"Z="+grad1Z)

        listgradientXTemp.append("gradB"+index+"X="+grad2X)
        listgradientYTemp.append("gradB"+index+"Y="+grad2Y)
        listgradientZTemp.append("gradB"+index+"Z="+grad2Z)

        expression="max(A"+index+",B"+index+")"

        gradX="ind(A"+index+",-B"+index+")*gradA"+index+"X"+"+"+"(1.0-ind(A"+index+",-B"+index+"))*(-gradB"+index+"X"+")"
        gradY="ind(A"+index+",-B"+index+")*gradA"+index+"Y"+"+"+"(1.0-ind(A"+index+",-B"+index+"))*(-gradB"+index+"Y"+")"
        gradZ="ind(A"+index+",-B"+index+")*gradA"+index+"Z"+"+"+"(1.0-ind(A"+index+",-B"+index+"))*(-gradB"+index+"Z"+")"

        return (expression,(gradX,gradY,gradZ))


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
        index=generateNewJindex()
        self.index=index
        self.type
        self.identifier


#        cpdef translationRotationToString(self):

        #        if self.cosTheta==0.0:

        #            x1="(-"+minus("y",self.center.y)+")"
        #            y=minus("x",self.center.x)


        #        elif self.cosTheta==1.0:

        #            x1=minus("x",self.center.x)
        #            y=minus("y",self.center.y)


        #        else:

        x1="cosTheta"+self.index+"*(x-center"+self.index+"x)-sinTheta"+self.index+"*(y-center"+self.index+"y)"
        y="sinTheta"+self.index+"*(x-center"+self.index+"x)+cosTheta"+self.index+"*(y-center"+self.index+"y)"

        #        if self.cosPhi==0.0:

        #           x="(-"+minus("z",self.center.z)+")"
        #           z=x1

        #        elif self.cosPhi==1.0:

        #            x=x1
        #            z=minus("z",self.center.z)

        #        else:

        x="cosPhi"+self.index+"*"+x1+"-sinPhi"+self.index+"*(z-center"+self.index+"z)"
        z="sinPhi"+self.index+"*"+x1+"+cosPhi"+self.index+"*(z-center"+self.index+"z)"

#        listCoordPrimitives.append("x"+self.index+"="+x+"\n"+"y"+self.index+"="+y+"\n"+"z"+self.index+"="+z+"\n\n\n")
        #        print self.index+"      "+"("+x+","+y+","+z+")"
#            return ("x"+self.index,"y"+self.index,"z"+self.index)

        self.coord=(x,y,z)

#        cpdef gradXtranslationRotationToString(self):

        #        (dxX,dxY,dxZ)=(multiply(str(self.cosPhi),str(self.cosTheta)), str(self.sinTheta),multiply(str(self.sinPhi),str(self.cosTheta)))
        (dxX,dxY,dxZ)=("cosPhi"+self.index+"*"+"cosTheta"+self.index, "sinTheta"+self.index,"sinPhi"+self.index+"*"+"cosTheta"+self.index)

        listgradientDxPrimitives.append("dxX"+self.index+"="+dxX+"\n"+"dxY"+self.index+"="+dxY+"\n"+"dxZ"+self.index+"="+dxZ+"\n\n\n")
#            return ("dxX"+self.index,"dxY"+self.index,"dxZ"+self.index)

#        cpdef gradYtranslationRotationToString(self):

        (dyX,dyY,dyZ)=("(-cosPhi"+self.index+")*sinTheta"+self.index, "cosTheta"+self.index,"-sinPhi"+self.index+"*"+"sinTheta"+self.index)

        listgradientDyPrimitives.append("dyX"+self.index+"="+dyX+"\n"+"dyY"+self.index+"="+dyY+"\n"+"dyZ"+self.index+"="+dyZ+"\n\n\n")
#            return ("dyX"+self.index,"dyY"+self.index,"dyZ"+self.index)

#        cpdef gradZtranslationRotationToString(self):

        (dzX,dzY,dzZ)=("sinPhi"+self.index, str(0.0),"cosPhi"+self.index)

        listgradientDzPrimitives.append("dzX"+self.index+"="+dzX+"\n"+"dzY"+self.index+"="+dzY+"\n"+"dzZ"+self.index+"="+dzZ+"\n\n\n")
#            return ("dzX"+self.index,"dzY"+self.index,"dzZ"+self.index)


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




cdef class Ellipsoid(Primitives):


    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):

        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="ellipsoid"
        identifier=[self.type,self.sign, self.axisX,self.axisY,self.axisZ,self.theta,self.phi,(self.center.x,self.center.y,self.center.z),self.coord,self.index]
        self.identifier=identifier
        listPrimitives.append(identifier)


    cpdef double eval(self,Point point):

        point=self.translationRotation(point)

        return (point.x/self.axisX)**2+(point.y/self.axisY)**2+(point.z/self.axisZ)**2-1

    cpdef tuple toString(self):

        (x,y,z)=("x"+self.index,"y"+self.index,"z"+self.index)#self.translationRotationToString()
        (dxX,dxY,dxZ)=("dxX"+self.index,"dxY"+self.index,"dxZ"+self.index)#self.gradXtranslationRotationToString()

#        add(add(multiply(dxX,divide("2.0*"+x,self.axisX**2)),multiply(dxY,divide("2.0*"+y,self.axisY**2))),\
#        multiply(dxZ,divide("2.0*"+z,self.axisZ**2)))

        gradX="("+dxX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dxY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"+"+"+\
                "("+dxZ+"*2.0*"+z+"/axis"+self.index+"Z*axis"+self.index+"Z)"


        (dyX,dyY,dyZ)=("dyX"+self.index,"dyY"+self.index,"dyZ"+self.index)#self.gradYtranslationRotationToString()

#         add(add(multiply(dyX,divide("2.0*"+x,self.axisX**2)),multiply(dyY,divide("2.0*"+y,self.axisY**2))),\
#         multiply(dyZ,divide("2.0*"+z,self.axisZ**2)))

        gradY="("+dyX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dyY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"+"+"+\
                "("+dyZ+"*2.0*"+z+"/axis"+self.index+"Z*axis"+self.index+"Z)"

        (dzX,dzY,dzZ)=("dzX"+self.index,"dzY"+self.index,"dzZ"+self.index)#self.gradZtranslationRotationToString()

#        add(add(multiply(dzX,divide("2.0*"+x,self.axisX**2)),multiply(dzY,divide("2.0*"+y,self.axisY**2))),\
#        multiply(dzZ,divide("2.0*"+z,self.axisZ**2)))

        gradZ="("+dzX+"*2.0*"+x+"/axis"+self.index+"X*axis"+self.index+"X)"+"+"+"("+dzY+"*2.0*"+y+"/axis"+self.index+"Y*axis"+self.index+"Y)"+"+"+\
            "("+dzZ+"*2.0*"+z+"/axis"+self.index+"Z*axis"+self.index+"Z)"

        grad=(gradX,gradY,gradZ)

        expression="("+x+"/axis"+self.index+"X)*("+x+"/axis"+self.index+"X)"+"+"+"("+y+"/axis"+self.index+"Y)*("+y+"/axis"+self.index+"Y)"+"+"\
                  +"("+z+"/axis"+self.index+"Z)*("+z+"/axis"+self.index+"Z)-1.0"
#        divide(x,self.axisX)+"*"+divide(x,self.axisX)+"+"+divide(y,self.axisY)+"*"+divide(y,self.axisY)+"+"+divide(z,self.axisZ)+"*"+\
#        divide(z,self.axisZ)+"-1.0"

        return (expression, grad)

cdef class Frisbee(Primitives):

    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):
        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="frisbee"
        identifier=[self.type,self.sign, self.axisX,self.axisY,self.axisZ,self.theta,self.phi,(self.center.x,self.center.y,self.center.z),self.coord,self.index]
        self.identifier=identifier
        listPrimitives.append(identifier)

    cpdef double eval(self,Point point):

        point=self.translationRotation(point)

        return abs(point.z/self.axisZ)+math.sqrt((point.x/self.axisX)**2+(point.y/self.axisY)**2)-1


    cpdef tuple toString(self):

        (x,y,z)=("x"+self.index,"y"+self.index,"z"+self.index)#self.translationRotationToString()

        sqrtTemp="sqrt(("+x+"/axis"+self.index+"X)"+"*"+"("+x+"/axis"+self.index+"X)"+"+"+"("+y+"/axis"+self.index+"Y)"+"*"+"("+y+"/axis"+self.index+"Y)"+")"
        listSqrt.append("sqrt"+self.index+"="+sqrtTemp+"\n\n")


        (dxX,dxY,dxZ)=("dxX"+self.index,"dxY"+self.index,"dxZ"+self.index)#self.gradXtranslationRotationToString()
        gradX=dxX+"*("+x+"/sqrt"+self.index+")"\
              +"+"+dxY+"*("+y+"/sqrt"+self.index+")"\
              +"+"+dxZ+"*"+"sign("+z+")/axis"+self.index+"Z"

        (dyX,dyY,dyZ)=("dyX"+self.index,"dyY"+self.index,"dyZ"+self.index)#self.gradYtranslationRotationToString()

        gradY=dyX+"*("+x+"/sqrt"+self.index+")"\
              +"+"+dyY+"*("+y+"/sqrt"+self.index+")"\
              +"+"+dyZ+"*"+"sign("+z+")/axis"+self.index+"Z"

        (dzX,dzY,dzZ)=("dzX"+self.index,"dzY"+self.index,"dzZ"+self.index)#self.gradZtranslationRotationToString()

        gradZ=dzX+"*("+x+"/sqrt"+self.index+")"\
              +"+"+dzY+"*("+y+"/sqrt"+self.index+")"\
              +"+"+dzZ+"*"+"sign("+z+")/axis"+self.index+"Z"

        grad=(gradX,gradY,gradZ)

        expression="abs("+z+"/axis"+self.index+"Z)"+"+"\
                    +"sqrt"+self.index+"-1.0"

        return (expression, grad)


cdef class Cylinder(Primitives):


    def __init__(self,sign,axisX,axisY,axisZ,theta,phi,center):
        Primitives.__init__(self,sign,axisX,axisY,axisZ,theta,phi,center)
        self.type="cylinder"
#        (x,y,z)=("x"+self.index,"y"+self.index,"z"+self.index)#self.translationRotationToString()

        self.radial="(x"+self.index+"/axis"+self.index+"X)"+"*"+"(x"+self.index+"/axis"+self.index+"X)"+"+"+"(y"+self.index+"/axis"+self.index+"Y)"+"*"+"(y"+self.index+"/axis"+self.index+"Y)"+"-1.0"
        self.height="abs(z"+self.index+"-axis"+self.index+"Z)"
        identifier=[self.type,self.sign, self.axisX,self.axisY,self.axisZ,self.theta,self.phi,(self.center.x,self.center.y,self.center.z),self.coord,self.radial,self.height,self.index]
        self.identifier=identifier
        listPrimitives.append(identifier)


    cpdef double eval(self,Point point):
        point=self.translationRotation(point)

        return max(abs(point.z)-self.axisZ,(point.x/self.axisX)**2+(point.y/self.axisY)**2-1)


    cpdef tuple toString(self):

        (x,y,z)=("x"+self.index,"y"+self.index,"z"+self.index)#self.translationRotationToString()
        (dxX,dxY,dxZ)=("dxX"+self.index,"dxY"+self.index,"dxZ"+self.index)#self.gradXtranslationRotationToString()

        gradX=dxX+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+x+"/(axis"+self.index+"X"+"*"+"axis"+self.index+"X)+"\
              +dxY+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+y+"/(axis"+self.index+"Y"+"*"+"axis"+self.index+"Y)+"\
              +dxZ+"*ind(height"+self.index+",radial"+self.index+")*sign("+z+")"

        (dyX,dyY,dyZ)=("dyX"+self.index,"dyY"+self.index,"dyZ"+self.index)#self.gradYtranslationRotationToString()

        gradY=dyX+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+x+"/(axis"+self.index+"X"+"*"+"axis"+self.index+"X)+"\
              +dyY+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+y+"/(axis"+self.index+"Y"+"*"+"axis"+self.index+"Y)+"\
              +dyZ+"*ind(height"+self.index+",radial"+self.index+")*sign("+z+")"

        (dzX,dzY,dzZ)=("dzX"+self.index,"dzY"+self.index,"dzZ"+self.index)#self.gradZtranslationRotationToString()

        gradZ=dzX+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+x+"/(axis"+self.index+"X"+"*"+"axis"+self.index+"X)+"\
              +dzY+"*ind(radial"+self.index+",height"+self.index+")"+"*2.0*"+y+"/(axis"+self.index+"Y"+"*"+"axis"+self.index+"Y)+"\
              +dzZ+"*ind(height"+self.index+",radial"+self.index+")*sign("+z+")"

        grad=(gradX,gradY,gradZ)


        expression="max(height"+self.index+",radial"+self.index+")"

        return (expression, grad)
