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
from math import cos, sin, sqrt
from libc.math cimport sin, cos, acos, exp, sqrt, fabs, M_PI
cimport numpy
cimport cython
cimport primitives

cdef double x=1.0
cdef double y=2.0
cdef double z=3.0


cdef double sign(double x):
     if x>0.0:
         return 1.0

     else:
         return -1.0

cdef double ind(double a, double b):
     if a>b:
         return 1.0

     elif a<b:
         return 0.0

     else:
         return 0.5






#LISTE DES PARAMETRES DES PRIMITIVES

#############
##PRIMITIVE   0
############


type0='ellipsoid'
sign0='+'
axis0X=1.0
axis0Y=2.0
axis0Z=2.0
theta0=0.785398163397
phi0=1.57079632679
cosTheta0=cos(0.785398163397)
cosPhi0=cos(1.57079632679)
sinTheta0=sin(0.785398163397)
sinPhi0=sin(1.57079632679)
(center0x,center0y,center0z)=(1.0, 0.0, 0.0)

(x0,y0,z0)=(cosPhi0*cosTheta0*(x-center0x)-sinTheta0*(y-center0y)-sinPhi0*(z-center0z),sinTheta0*(x-center0x)+cosTheta0*(y-center0y),sinPhi0*cosTheta0*(x-center0x)-sinTheta0*(y-center0y)+cosPhi0*(z-center0z))



#############
##PRIMITIVE   1
############


type1='ellipsoid'
sign1='+'
axis1X=1.0
axis1Y=1.0
axis1Z=1.0
theta1=0.0
phi1=0.0
cosTheta1=cos(0.0)
cosPhi1=cos(0.0)
sinTheta1=sin(0.0)
sinPhi1=sin(0.0)
(center1x,center1y,center1z)=(0.0, 0.0, 0.0)

(x1,y1,z1)=(cosPhi1*cosTheta1*(x-center1x)-sinTheta1*(y-center1y)-sinPhi1*(z-center1z),sinTheta1*(x-center1x)+cosTheta1*(y-center1y),sinPhi1*cosTheta1*(x-center1x)-sinTheta1*(y-center1y)+cosPhi1*(z-center1z))



#############
##PRIMITIVE   2
############


type2='ellipsoid'
sign2='+'
axis2X=1.0
axis2Y=1.0
axis2Z=1.0
theta2=0.0
phi2=0.0
cosTheta2=cos(0.0)
cosPhi2=cos(0.0)
sinTheta2=sin(0.0)
sinPhi2=sin(0.0)
(center2x,center2y,center2z)=(5.5, 0.0, 0.0)

(x2,y2,z2)=(cosPhi2*cosTheta2*(x-center2x)-sinTheta2*(y-center2y)-sinPhi2*(z-center2z),sinTheta2*(x-center2x)+cosTheta2*(y-center2y),sinPhi2*cosTheta2*(x-center2x)-sinTheta2*(y-center2y)+cosPhi2*(z-center2z))



#############
##PRIMITIVE   3
############


type3='cylinder'
sign3='+'
axis3X=1.0
axis3Y=2.0
axis3Z=2.0
theta3=0.785398163397
phi3=1.57079632679
cosTheta3=cos(0.785398163397)
cosPhi3=cos(1.57079632679)
sinTheta3=sin(0.785398163397)
sinPhi3=sin(1.57079632679)
(center3x,center3y,center3z)=(1.0, 0.0, 0.0)

(x3,y3,z3)=(cosPhi3*cosTheta3*(x-center3x)-sinTheta3*(y-center3y)-sinPhi3*(z-center3z),sinTheta3*(x-center3x)+cosTheta3*(y-center3y),sinPhi3*cosTheta3*(x-center3x)-sinTheta3*(y-center3y)+cosPhi3*(z-center3z))


radial3=(x3/axis3X)*(x3/axis3X)+(y3/axis3Y)*(y3/axis3Y)-1.0

height3=abs(z3-axis3Z)





#############
##PRIMITIVE   4
############


type4='cylinder'
sign4='+'
axis4X=1.0
axis4Y=2.0
axis4Z=2.0
theta4=0.785398163397
phi4=1.57079632679
cosTheta4=cos(0.785398163397)
cosPhi4=cos(1.57079632679)
sinTheta4=sin(0.785398163397)
sinPhi4=sin(1.57079632679)
(center4x,center4y,center4z)=(1.0, 0.0, 0.0)

(x4,y4,z4)=(cosPhi4*cosTheta4*(x-center4x)-sinTheta4*(y-center4y)-sinPhi4*(z-center4z),sinTheta4*(x-center4x)+cosTheta4*(y-center4y),sinPhi4*cosTheta4*(x-center4x)-sinTheta4*(y-center4y)+cosPhi4*(z-center4z))


radial4=(x4/axis4X)*(x4/axis4X)+(y4/axis4Y)*(y4/axis4Y)-1.0

height4=abs(z4-axis4Z)





#############
##PRIMITIVE   5
############


type5='cylinder'
sign5='+'
axis5X=2.0
axis5Y=2.0
axis5Z=5.0
theta5=0.0
phi5=0.0
cosTheta5=cos(0.0)
cosPhi5=cos(0.0)
sinTheta5=sin(0.0)
sinPhi5=sin(0.0)
(center5x,center5y,center5z)=(0.0, 0.0, 5.0)

(x5,y5,z5)=(cosPhi5*cosTheta5*(x-center5x)-sinTheta5*(y-center5y)-sinPhi5*(z-center5z),sinTheta5*(x-center5x)+cosTheta5*(y-center5y),sinPhi5*cosTheta5*(x-center5x)-sinTheta5*(y-center5y)+cosPhi5*(z-center5z))


radial5=(x5/axis5X)*(x5/axis5X)+(y5/axis5Y)*(y5/axis5Y)-1.0

height5=abs(z5-axis5Z)





#############
##PRIMITIVE   6
############


type6='cylinder'
sign6='+'
axis6X=1.9
axis6Y=1.9
axis6Z=4.9
theta6=0.0
phi6=0.0
cosTheta6=cos(0.0)
cosPhi6=cos(0.0)
sinTheta6=sin(0.0)
sinPhi6=sin(0.0)
(center6x,center6y,center6z)=(0.0, 0.0, 5.0)

(x6,y6,z6)=(cosPhi6*cosTheta6*(x-center6x)-sinTheta6*(y-center6y)-sinPhi6*(z-center6z),sinTheta6*(x-center6x)+cosTheta6*(y-center6y),sinPhi6*cosTheta6*(x-center6x)-sinTheta6*(y-center6y)+cosPhi6*(z-center6z))


radial6=(x6/axis6X)*(x6/axis6X)+(y6/axis6Y)*(y6/axis6Y)-1.0

height6=abs(z6-axis6Z)





#############
##PRIMITIVE   7
############


type7='frisbee'
sign7='+'
axis7X=4.0
axis7Y=4.0
axis7Z=2.75
theta7=0.0
phi7=0.0
cosTheta7=cos(0.0)
cosPhi7=cos(0.0)
sinTheta7=sin(0.0)
sinPhi7=sin(0.0)
(center7x,center7y,center7z)=(0.0, 0.0, 0.5)

(x7,y7,z7)=(cosPhi7*cosTheta7*(x-center7x)-sinTheta7*(y-center7y)-sinPhi7*(z-center7z),sinTheta7*(x-center7x)+cosTheta7*(y-center7y),sinPhi7*cosTheta7*(x-center7x)-sinTheta7*(y-center7y)+cosPhi7*(z-center7z))



#############
##PRIMITIVE   8
############


type8='frisbee'
sign8='+'
axis8X=3.9
axis8Y=3.9
axis8Z=2.65
theta8=0.0
phi8=0.0
cosTheta8=cos(0.0)
cosPhi8=cos(0.0)
sinTheta8=sin(0.0)
sinPhi8=sin(0.0)
(center8x,center8y,center8z)=(0.0, 0.0, 0.5)

(x8,y8,z8)=(cosPhi8*cosTheta8*(x-center8x)-sinTheta8*(y-center8y)-sinPhi8*(z-center8z),sinTheta8*(x-center8x)+cosTheta8*(y-center8y),sinPhi8*cosTheta8*(x-center8x)-sinTheta8*(y-center8y)+cosPhi8*(z-center8z))



#############
##PRIMITIVE   9
############


type9='frisbee'
sign9='+'
axis9X=4.0
axis9Y=4.0
axis9Z=2.75
theta9=0.0
phi9=0.0
cosTheta9=cos(0.0)
cosPhi9=cos(0.0)
sinTheta9=sin(0.0)
sinPhi9=sin(0.0)
(center9x,center9y,center9z)=(0.0, 0.0, 1.5)

(x9,y9,z9)=(cosPhi9*cosTheta9*(x-center9x)-sinTheta9*(y-center9y)-sinPhi9*(z-center9z),sinTheta9*(x-center9x)+cosTheta9*(y-center9y),sinPhi9*cosTheta9*(x-center9x)-sinTheta9*(y-center9y)+cosPhi9*(z-center9z))



#############
##PRIMITIVE   10
############


type10='frisbee'
sign10='+'
axis10X=3.9
axis10Y=3.9
axis10Z=2.65
theta10=0.0
phi10=0.0
cosTheta10=cos(0.0)
cosPhi10=cos(0.0)
sinTheta10=sin(0.0)
sinPhi10=sin(0.0)
(center10x,center10y,center10z)=(0.0, 0.0, 1.5)

(x10,y10,z10)=(cosPhi10*cosTheta10*(x-center10x)-sinTheta10*(y-center10y)-sinPhi10*(z-center10z),sinTheta10*(x-center10x)+cosTheta10*(y-center10y),sinPhi10*cosTheta10*(x-center10x)-sinTheta10*(y-center10y)+cosPhi10*(z-center10z))



#############
##PRIMITIVE   11
############


type11='frisbee'
sign11='+'
axis11X=4.0
axis11Y=4.0
axis11Z=2.75
theta11=0.0
phi11=0.0
cosTheta11=cos(0.0)
cosPhi11=cos(0.0)
sinTheta11=sin(0.0)
sinPhi11=sin(0.0)
(center11x,center11y,center11z)=(0.0, 0.0, 2.5)

(x11,y11,z11)=(cosPhi11*cosTheta11*(x-center11x)-sinTheta11*(y-center11y)-sinPhi11*(z-center11z),sinTheta11*(x-center11x)+cosTheta11*(y-center11y),sinPhi11*cosTheta11*(x-center11x)-sinTheta11*(y-center11y)+cosPhi11*(z-center11z))



#############
##PRIMITIVE   12
############


type12='frisbee'
sign12='+'
axis12X=3.9
axis12Y=3.9
axis12Z=2.65
theta12=0.0
phi12=0.0
cosTheta12=cos(0.0)
cosPhi12=cos(0.0)
sinTheta12=sin(0.0)
sinPhi12=sin(0.0)
(center12x,center12y,center12z)=(0.0, 0.0, 2.5)

(x12,y12,z12)=(cosPhi12*cosTheta12*(x-center12x)-sinTheta12*(y-center12y)-sinPhi12*(z-center12z),sinTheta12*(x-center12x)+cosTheta12*(y-center12y),sinPhi12*cosTheta12*(x-center12x)-sinTheta12*(y-center12y)+cosPhi12*(z-center12z))



#############
##PRIMITIVE   13
############


type13='frisbee'
sign13='+'
axis13X=4.0
axis13Y=4.0
axis13Z=2.75
theta13=0.0
phi13=0.0
cosTheta13=cos(0.0)
cosPhi13=cos(0.0)
sinTheta13=sin(0.0)
sinPhi13=sin(0.0)
(center13x,center13y,center13z)=(0.0, 0.0, 3.5)

(x13,y13,z13)=(cosPhi13*cosTheta13*(x-center13x)-sinTheta13*(y-center13y)-sinPhi13*(z-center13z),sinTheta13*(x-center13x)+cosTheta13*(y-center13y),sinPhi13*cosTheta13*(x-center13x)-sinTheta13*(y-center13y)+cosPhi13*(z-center13z))



#############
##PRIMITIVE   14
############


type14='frisbee'
sign14='+'
axis14X=3.9
axis14Y=3.9
axis14Z=2.65
theta14=0.0
phi14=0.0
cosTheta14=cos(0.0)
cosPhi14=cos(0.0)
sinTheta14=sin(0.0)
sinPhi14=sin(0.0)
(center14x,center14y,center14z)=(0.0, 0.0, 3.5)

(x14,y14,z14)=(cosPhi14*cosTheta14*(x-center14x)-sinTheta14*(y-center14y)-sinPhi14*(z-center14z),sinTheta14*(x-center14x)+cosTheta14*(y-center14y),sinPhi14*cosTheta14*(x-center14x)-sinTheta14*(y-center14y)+cosPhi14*(z-center14z))



#############
##PRIMITIVE   15
############


type15='frisbee'
sign15='+'
axis15X=4.0
axis15Y=4.0
axis15Z=2.75
theta15=0.0
phi15=0.0
cosTheta15=cos(0.0)
cosPhi15=cos(0.0)
sinTheta15=sin(0.0)
sinPhi15=sin(0.0)
(center15x,center15y,center15z)=(0.0, 0.0, 4.5)

(x15,y15,z15)=(cosPhi15*cosTheta15*(x-center15x)-sinTheta15*(y-center15y)-sinPhi15*(z-center15z),sinTheta15*(x-center15x)+cosTheta15*(y-center15y),sinPhi15*cosTheta15*(x-center15x)-sinTheta15*(y-center15y)+cosPhi15*(z-center15z))



#############
##PRIMITIVE   16
############


type16='frisbee'
sign16='+'
axis16X=3.9
axis16Y=3.9
axis16Z=2.65
theta16=0.0
phi16=0.0
cosTheta16=cos(0.0)
cosPhi16=cos(0.0)
sinTheta16=sin(0.0)
sinPhi16=sin(0.0)
(center16x,center16y,center16z)=(0.0, 0.0, 4.5)

(x16,y16,z16)=(cosPhi16*cosTheta16*(x-center16x)-sinTheta16*(y-center16y)-sinPhi16*(z-center16z),sinTheta16*(x-center16x)+cosTheta16*(y-center16y),sinPhi16*cosTheta16*(x-center16x)-sinTheta16*(y-center16y)+cosPhi16*(z-center16z))



#############
##PRIMITIVE   17
############


type17='frisbee'
sign17='+'
axis17X=4.0
axis17Y=4.0
axis17Z=2.75
theta17=0.0
phi17=0.0
cosTheta17=cos(0.0)
cosPhi17=cos(0.0)
sinTheta17=sin(0.0)
sinPhi17=sin(0.0)
(center17x,center17y,center17z)=(0.0, 0.0, 5.5)

(x17,y17,z17)=(cosPhi17*cosTheta17*(x-center17x)-sinTheta17*(y-center17y)-sinPhi17*(z-center17z),sinTheta17*(x-center17x)+cosTheta17*(y-center17y),sinPhi17*cosTheta17*(x-center17x)-sinTheta17*(y-center17y)+cosPhi17*(z-center17z))



#############
##PRIMITIVE   18
############


type18='frisbee'
sign18='+'
axis18X=3.9
axis18Y=3.9
axis18Z=2.65
theta18=0.0
phi18=0.0
cosTheta18=cos(0.0)
cosPhi18=cos(0.0)
sinTheta18=sin(0.0)
sinPhi18=sin(0.0)
(center18x,center18y,center18z)=(0.0, 0.0, 5.5)

(x18,y18,z18)=(cosPhi18*cosTheta18*(x-center18x)-sinTheta18*(y-center18y)-sinPhi18*(z-center18z),sinTheta18*(x-center18x)+cosTheta18*(y-center18y),sinPhi18*cosTheta18*(x-center18x)-sinTheta18*(y-center18y)+cosPhi18*(z-center18z))



#############
##PRIMITIVE   19
############


type19='frisbee'
sign19='+'
axis19X=4.0
axis19Y=4.0
axis19Z=2.75
theta19=0.0
phi19=0.0
cosTheta19=cos(0.0)
cosPhi19=cos(0.0)
sinTheta19=sin(0.0)
sinPhi19=sin(0.0)
(center19x,center19y,center19z)=(0.0, 0.0, 6.5)

(x19,y19,z19)=(cosPhi19*cosTheta19*(x-center19x)-sinTheta19*(y-center19y)-sinPhi19*(z-center19z),sinTheta19*(x-center19x)+cosTheta19*(y-center19y),sinPhi19*cosTheta19*(x-center19x)-sinTheta19*(y-center19y)+cosPhi19*(z-center19z))



#############
##PRIMITIVE   20
############


type20='frisbee'
sign20='+'
axis20X=3.9
axis20Y=3.9
axis20Z=2.65
theta20=0.0
phi20=0.0
cosTheta20=cos(0.0)
cosPhi20=cos(0.0)
sinTheta20=sin(0.0)
sinPhi20=sin(0.0)
(center20x,center20y,center20z)=(0.0, 0.0, 6.5)

(x20,y20,z20)=(cosPhi20*cosTheta20*(x-center20x)-sinTheta20*(y-center20y)-sinPhi20*(z-center20z),sinTheta20*(x-center20x)+cosTheta20*(y-center20y),sinPhi20*cosTheta20*(x-center20x)-sinTheta20*(y-center20y)+cosPhi20*(z-center20z))



#############
##PRIMITIVE   21
############


type21='frisbee'
sign21='+'
axis21X=4.0
axis21Y=4.0
axis21Z=2.75
theta21=0.0
phi21=0.0
cosTheta21=cos(0.0)
cosPhi21=cos(0.0)
sinTheta21=sin(0.0)
sinPhi21=sin(0.0)
(center21x,center21y,center21z)=(0.0, 0.0, 7.5)

(x21,y21,z21)=(cosPhi21*cosTheta21*(x-center21x)-sinTheta21*(y-center21y)-sinPhi21*(z-center21z),sinTheta21*(x-center21x)+cosTheta21*(y-center21y),sinPhi21*cosTheta21*(x-center21x)-sinTheta21*(y-center21y)+cosPhi21*(z-center21z))



#############
##PRIMITIVE   22
############


type22='frisbee'
sign22='+'
axis22X=3.9
axis22Y=3.9
axis22Z=2.65
theta22=0.0
phi22=0.0
cosTheta22=cos(0.0)
cosPhi22=cos(0.0)
sinTheta22=sin(0.0)
sinPhi22=sin(0.0)
(center22x,center22y,center22z)=(0.0, 0.0, 7.5)

(x22,y22,z22)=(cosPhi22*cosTheta22*(x-center22x)-sinTheta22*(y-center22y)-sinPhi22*(z-center22z),sinTheta22*(x-center22x)+cosTheta22*(y-center22y),sinPhi22*cosTheta22*(x-center22x)-sinTheta22*(y-center22y)+cosPhi22*(z-center22z))



#############
##PRIMITIVE   23
############


type23='frisbee'
sign23='+'
axis23X=4.0
axis23Y=4.0
axis23Z=2.75
theta23=0.0
phi23=0.0
cosTheta23=cos(0.0)
cosPhi23=cos(0.0)
sinTheta23=sin(0.0)
sinPhi23=sin(0.0)
(center23x,center23y,center23z)=(0.0, 0.0, 8.5)

(x23,y23,z23)=(cosPhi23*cosTheta23*(x-center23x)-sinTheta23*(y-center23y)-sinPhi23*(z-center23z),sinTheta23*(x-center23x)+cosTheta23*(y-center23y),sinPhi23*cosTheta23*(x-center23x)-sinTheta23*(y-center23y)+cosPhi23*(z-center23z))



#############
##PRIMITIVE   24
############


type24='frisbee'
sign24='+'
axis24X=3.9
axis24Y=3.9
axis24Z=2.65
theta24=0.0
phi24=0.0
cosTheta24=cos(0.0)
cosPhi24=cos(0.0)
sinTheta24=sin(0.0)
sinPhi24=sin(0.0)
(center24x,center24y,center24z)=(0.0, 0.0, 8.5)

(x24,y24,z24)=(cosPhi24*cosTheta24*(x-center24x)-sinTheta24*(y-center24y)-sinPhi24*(z-center24z),sinTheta24*(x-center24x)+cosTheta24*(y-center24y),sinPhi24*cosTheta24*(x-center24x)-sinTheta24*(y-center24y)+cosPhi24*(z-center24z))



#############
##PRIMITIVE   25
############


type25='frisbee'
sign25='+'
axis25X=4.0
axis25Y=4.0
axis25Z=2.75
theta25=0.0
phi25=0.0
cosTheta25=cos(0.0)
cosPhi25=cos(0.0)
sinTheta25=sin(0.0)
sinPhi25=sin(0.0)
(center25x,center25y,center25z)=(0.0, 0.0, 9.5)

(x25,y25,z25)=(cosPhi25*cosTheta25*(x-center25x)-sinTheta25*(y-center25y)-sinPhi25*(z-center25z),sinTheta25*(x-center25x)+cosTheta25*(y-center25y),sinPhi25*cosTheta25*(x-center25x)-sinTheta25*(y-center25y)+cosPhi25*(z-center25z))



#############
##PRIMITIVE   26
############


type26='frisbee'
sign26='+'
axis26X=3.9
axis26Y=3.9
axis26Z=2.65
theta26=0.0
phi26=0.0
cosTheta26=cos(0.0)
cosPhi26=cos(0.0)
sinTheta26=sin(0.0)
sinPhi26=sin(0.0)
(center26x,center26y,center26z)=(0.0, 0.0, 9.5)

(x26,y26,z26)=(cosPhi26*cosTheta26*(x-center26x)-sinTheta26*(y-center26y)-sinPhi26*(z-center26z),sinTheta26*(x-center26x)+cosTheta26*(y-center26y),sinPhi26*cosTheta26*(x-center26x)-sinTheta26*(y-center26y)+cosPhi26*(z-center26z))








#LISTE DES CALCULS PRELIMINAIRES DE RACINES


sqrt7=sqrt((x7/axis7X)*(x7/axis7X)+(y7/axis7Y)*(y7/axis7Y))

sqrt9=sqrt((x9/axis9X)*(x9/axis9X)+(y9/axis9Y)*(y9/axis9Y))

sqrt11=sqrt((x11/axis11X)*(x11/axis11X)+(y11/axis11Y)*(y11/axis11Y))

sqrt13=sqrt((x13/axis13X)*(x13/axis13X)+(y13/axis13Y)*(y13/axis13Y))

sqrt15=sqrt((x15/axis15X)*(x15/axis15X)+(y15/axis15Y)*(y15/axis15Y))

sqrt17=sqrt((x17/axis17X)*(x17/axis17X)+(y17/axis17Y)*(y17/axis17Y))

sqrt19=sqrt((x19/axis19X)*(x19/axis19X)+(y19/axis19Y)*(y19/axis19Y))

sqrt21=sqrt((x21/axis21X)*(x21/axis21X)+(y21/axis21Y)*(y21/axis21Y))

sqrt23=sqrt((x23/axis23X)*(x23/axis23X)+(y23/axis23Y)*(y23/axis23Y))

sqrt25=sqrt((x25/axis25X)*(x25/axis25X)+(y25/axis25Y)*(y25/axis25Y))

sqrt8=sqrt((x8/axis8X)*(x8/axis8X)+(y8/axis8Y)*(y8/axis8Y))

sqrt10=sqrt((x10/axis10X)*(x10/axis10X)+(y10/axis10Y)*(y10/axis10Y))

sqrt12=sqrt((x12/axis12X)*(x12/axis12X)+(y12/axis12Y)*(y12/axis12Y))

sqrt14=sqrt((x14/axis14X)*(x14/axis14X)+(y14/axis14Y)*(y14/axis14Y))

sqrt16=sqrt((x16/axis16X)*(x16/axis16X)+(y16/axis16Y)*(y16/axis16Y))

sqrt18=sqrt((x18/axis18X)*(x18/axis18X)+(y18/axis18Y)*(y18/axis18Y))

sqrt20=sqrt((x20/axis20X)*(x20/axis20X)+(y20/axis20Y)*(y20/axis20Y))

sqrt22=sqrt((x22/axis22X)*(x22/axis22X)+(y22/axis22Y)*(y22/axis22Y))

sqrt24=sqrt((x24/axis24X)*(x24/axis24X)+(y24/axis24Y)*(y24/axis24Y))

sqrt26=sqrt((x26/axis26X)*(x26/axis26X)+(y26/axis26Y)*(y26/axis26Y))

#LISTE DES EXPRESSIONS INTERMEDIAIRES

A0=max(height5,radial5)

B0=abs(z7/axis7Z)+sqrt7-1.0


A1=min(A0,B0)

B1=abs(z9/axis9Z)+sqrt9-1.0


A2=min(A1,B1)

B2=abs(z11/axis11Z)+sqrt11-1.0


A3=min(A2,B2)

B3=abs(z13/axis13Z)+sqrt13-1.0


A4=min(A3,B3)

B4=abs(z15/axis15Z)+sqrt15-1.0


A5=min(A4,B4)

B5=abs(z17/axis17Z)+sqrt17-1.0


A6=min(A5,B5)

B6=abs(z19/axis19Z)+sqrt19-1.0


A7=min(A6,B6)

B7=abs(z21/axis21Z)+sqrt21-1.0


A8=min(A7,B7)

B8=abs(z23/axis23Z)+sqrt23-1.0


A9=min(A8,B8)

B9=abs(z25/axis25Z)+sqrt25-1.0


A10=max(height6,radial6)

B10=abs(z8/axis8Z)+sqrt8-1.0


A11=min(A10,B10)

B11=abs(z10/axis10Z)+sqrt10-1.0


A12=min(A11,B11)

B12=abs(z12/axis12Z)+sqrt12-1.0


A13=min(A12,B12)

B13=abs(z14/axis14Z)+sqrt14-1.0


A14=min(A13,B13)

B14=abs(z16/axis16Z)+sqrt16-1.0


A15=min(A14,B14)

B15=abs(z18/axis18Z)+sqrt18-1.0


A16=min(A15,B15)

B16=abs(z20/axis20Z)+sqrt20-1.0


A17=min(A16,B16)

B17=abs(z22/axis22Z)+sqrt22-1.0


A18=min(A17,B17)

B18=abs(z24/axis24Z)+sqrt24-1.0


A19=min(A18,B18)

B19=abs(z26/axis26Z)+sqrt26-1.0


A20=min(A9,B9)

B20=min(A19,B19)


A21=max(height5,radial5)

B21=abs(z7/axis7Z)+sqrt7-1.0


A22=min(A21,B21)

B22=abs(z9/axis9Z)+sqrt9-1.0


A23=min(A22,B22)

B23=abs(z11/axis11Z)+sqrt11-1.0


A24=min(A23,B23)

B24=abs(z13/axis13Z)+sqrt13-1.0


A25=min(A24,B24)

B25=abs(z15/axis15Z)+sqrt15-1.0


A26=min(A25,B25)

B26=abs(z17/axis17Z)+sqrt17-1.0


A27=min(A26,B26)

B27=abs(z19/axis19Z)+sqrt19-1.0


A28=min(A27,B27)

B28=abs(z21/axis21Z)+sqrt21-1.0


A29=min(A28,B28)

B29=abs(z23/axis23Z)+sqrt23-1.0


A30=min(A29,B29)

B30=abs(z25/axis25Z)+sqrt25-1.0


A31=max(height6,radial6)

B31=abs(z8/axis8Z)+sqrt8-1.0


A32=min(A31,B31)

B32=abs(z10/axis10Z)+sqrt10-1.0


A33=min(A32,B32)

B33=abs(z12/axis12Z)+sqrt12-1.0


A34=min(A33,B33)

B34=abs(z14/axis14Z)+sqrt14-1.0


A35=min(A34,B34)

B35=abs(z16/axis16Z)+sqrt16-1.0


A36=min(A35,B35)

B36=abs(z18/axis18Z)+sqrt18-1.0


A37=min(A36,B36)

B37=abs(z20/axis20Z)+sqrt20-1.0


A38=min(A37,B37)

B38=abs(z22/axis22Z)+sqrt22-1.0


A39=min(A38,B38)

B39=abs(z24/axis24Z)+sqrt24-1.0


A40=min(A39,B39)

B40=abs(z26/axis26Z)+sqrt26-1.0


A41=min(A30,B30)

B41=min(A40,B40)


#Litteral expression  of the IMPLICIT FIELD is 
expression=max(A41,B41)


#LISTE DES GRADIENTS DES PRIMITIVES SELON X


dxX0=cosPhi0*cosTheta0
dxY0=sinTheta0
dxZ0=sinPhi0*cosTheta0


dxX1=cosPhi1*cosTheta1
dxY1=sinTheta1
dxZ1=sinPhi1*cosTheta1


dxX2=cosPhi2*cosTheta2
dxY2=sinTheta2
dxZ2=sinPhi2*cosTheta2


dxX3=cosPhi3*cosTheta3
dxY3=sinTheta3
dxZ3=sinPhi3*cosTheta3


dxX4=cosPhi4*cosTheta4
dxY4=sinTheta4
dxZ4=sinPhi4*cosTheta4


dxX5=cosPhi5*cosTheta5
dxY5=sinTheta5
dxZ5=sinPhi5*cosTheta5


dxX6=cosPhi6*cosTheta6
dxY6=sinTheta6
dxZ6=sinPhi6*cosTheta6


dxX7=cosPhi7*cosTheta7
dxY7=sinTheta7
dxZ7=sinPhi7*cosTheta7


dxX8=cosPhi8*cosTheta8
dxY8=sinTheta8
dxZ8=sinPhi8*cosTheta8


dxX9=cosPhi9*cosTheta9
dxY9=sinTheta9
dxZ9=sinPhi9*cosTheta9


dxX10=cosPhi10*cosTheta10
dxY10=sinTheta10
dxZ10=sinPhi10*cosTheta10


dxX11=cosPhi11*cosTheta11
dxY11=sinTheta11
dxZ11=sinPhi11*cosTheta11


dxX12=cosPhi12*cosTheta12
dxY12=sinTheta12
dxZ12=sinPhi12*cosTheta12


dxX13=cosPhi13*cosTheta13
dxY13=sinTheta13
dxZ13=sinPhi13*cosTheta13


dxX14=cosPhi14*cosTheta14
dxY14=sinTheta14
dxZ14=sinPhi14*cosTheta14


dxX15=cosPhi15*cosTheta15
dxY15=sinTheta15
dxZ15=sinPhi15*cosTheta15


dxX16=cosPhi16*cosTheta16
dxY16=sinTheta16
dxZ16=sinPhi16*cosTheta16


dxX17=cosPhi17*cosTheta17
dxY17=sinTheta17
dxZ17=sinPhi17*cosTheta17


dxX18=cosPhi18*cosTheta18
dxY18=sinTheta18
dxZ18=sinPhi18*cosTheta18


dxX19=cosPhi19*cosTheta19
dxY19=sinTheta19
dxZ19=sinPhi19*cosTheta19


dxX20=cosPhi20*cosTheta20
dxY20=sinTheta20
dxZ20=sinPhi20*cosTheta20


dxX21=cosPhi21*cosTheta21
dxY21=sinTheta21
dxZ21=sinPhi21*cosTheta21


dxX22=cosPhi22*cosTheta22
dxY22=sinTheta22
dxZ22=sinPhi22*cosTheta22


dxX23=cosPhi23*cosTheta23
dxY23=sinTheta23
dxZ23=sinPhi23*cosTheta23


dxX24=cosPhi24*cosTheta24
dxY24=sinTheta24
dxZ24=sinPhi24*cosTheta24


dxX25=cosPhi25*cosTheta25
dxY25=sinTheta25
dxZ25=sinPhi25*cosTheta25


dxX26=cosPhi26*cosTheta26
dxY26=sinTheta26
dxZ26=sinPhi26*cosTheta26


#LISTE DES GRADIENTS INTERMEDIAIRES SELON X


gradA0X=dxX5*ind(radial5,height5)*2.0*x5/(axis5X*axis5X)+dxY5*ind(radial5,height5)*2.0*y5/(axis5Y*axis5Y)+dxZ5*ind(height5,radial5)*sign(z5)

gradB0X=dxX7*(x7/sqrt7)+dxY7*(y7/sqrt7)+dxZ7*sign(z7)/axis7Z

gradA1X=(1.0-ind(A0,B0))*gradA0X+ind(A0,B0)*gradB0X

gradB1X=dxX9*(x9/sqrt9)+dxY9*(y9/sqrt9)+dxZ9*sign(z9)/axis9Z

gradA2X=(1.0-ind(A1,B1))*gradA1X+ind(A1,B1)*gradB1X

gradB2X=dxX11*(x11/sqrt11)+dxY11*(y11/sqrt11)+dxZ11*sign(z11)/axis11Z

gradA3X=(1.0-ind(A2,B2))*gradA2X+ind(A2,B2)*gradB2X

gradB3X=dxX13*(x13/sqrt13)+dxY13*(y13/sqrt13)+dxZ13*sign(z13)/axis13Z

gradA4X=(1.0-ind(A3,B3))*gradA3X+ind(A3,B3)*gradB3X

gradB4X=dxX15*(x15/sqrt15)+dxY15*(y15/sqrt15)+dxZ15*sign(z15)/axis15Z

gradA5X=(1.0-ind(A4,B4))*gradA4X+ind(A4,B4)*gradB4X

gradB5X=dxX17*(x17/sqrt17)+dxY17*(y17/sqrt17)+dxZ17*sign(z17)/axis17Z

gradA6X=(1.0-ind(A5,B5))*gradA5X+ind(A5,B5)*gradB5X

gradB6X=dxX19*(x19/sqrt19)+dxY19*(y19/sqrt19)+dxZ19*sign(z19)/axis19Z

gradA7X=(1.0-ind(A6,B6))*gradA6X+ind(A6,B6)*gradB6X

gradB7X=dxX21*(x21/sqrt21)+dxY21*(y21/sqrt21)+dxZ21*sign(z21)/axis21Z

gradA8X=(1.0-ind(A7,B7))*gradA7X+ind(A7,B7)*gradB7X

gradB8X=dxX23*(x23/sqrt23)+dxY23*(y23/sqrt23)+dxZ23*sign(z23)/axis23Z

gradA9X=(1.0-ind(A8,B8))*gradA8X+ind(A8,B8)*gradB8X

gradB9X=dxX25*(x25/sqrt25)+dxY25*(y25/sqrt25)+dxZ25*sign(z25)/axis25Z

gradA10X=dxX6*ind(radial6,height6)*2.0*x6/(axis6X*axis6X)+dxY6*ind(radial6,height6)*2.0*y6/(axis6Y*axis6Y)+dxZ6*ind(height6,radial6)*sign(z6)

gradB10X=dxX8*(x8/sqrt8)+dxY8*(y8/sqrt8)+dxZ8*sign(z8)/axis8Z

gradA11X=(1.0-ind(A10,B10))*gradA10X+ind(A10,B10)*gradB10X

gradB11X=dxX10*(x10/sqrt10)+dxY10*(y10/sqrt10)+dxZ10*sign(z10)/axis10Z

gradA12X=(1.0-ind(A11,B11))*gradA11X+ind(A11,B11)*gradB11X

gradB12X=dxX12*(x12/sqrt12)+dxY12*(y12/sqrt12)+dxZ12*sign(z12)/axis12Z

gradA13X=(1.0-ind(A12,B12))*gradA12X+ind(A12,B12)*gradB12X

gradB13X=dxX14*(x14/sqrt14)+dxY14*(y14/sqrt14)+dxZ14*sign(z14)/axis14Z

gradA14X=(1.0-ind(A13,B13))*gradA13X+ind(A13,B13)*gradB13X

gradB14X=dxX16*(x16/sqrt16)+dxY16*(y16/sqrt16)+dxZ16*sign(z16)/axis16Z

gradA15X=(1.0-ind(A14,B14))*gradA14X+ind(A14,B14)*gradB14X

gradB15X=dxX18*(x18/sqrt18)+dxY18*(y18/sqrt18)+dxZ18*sign(z18)/axis18Z

gradA16X=(1.0-ind(A15,B15))*gradA15X+ind(A15,B15)*gradB15X

gradB16X=dxX20*(x20/sqrt20)+dxY20*(y20/sqrt20)+dxZ20*sign(z20)/axis20Z

gradA17X=(1.0-ind(A16,B16))*gradA16X+ind(A16,B16)*gradB16X

gradB17X=dxX22*(x22/sqrt22)+dxY22*(y22/sqrt22)+dxZ22*sign(z22)/axis22Z

gradA18X=(1.0-ind(A17,B17))*gradA17X+ind(A17,B17)*gradB17X

gradB18X=dxX24*(x24/sqrt24)+dxY24*(y24/sqrt24)+dxZ24*sign(z24)/axis24Z

gradA19X=(1.0-ind(A18,B18))*gradA18X+ind(A18,B18)*gradB18X

gradB19X=dxX26*(x26/sqrt26)+dxY26*(y26/sqrt26)+dxZ26*sign(z26)/axis26Z

gradA20X=(1.0-ind(A9,B9))*gradA9X+ind(A9,B9)*gradB9X

gradB20X=(1.0-ind(A19,B19))*gradA19X+ind(A19,B19)*gradB19X

gradA21X=dxX5*ind(radial5,height5)*2.0*x5/(axis5X*axis5X)+dxY5*ind(radial5,height5)*2.0*y5/(axis5Y*axis5Y)+dxZ5*ind(height5,radial5)*sign(z5)

gradB21X=dxX7*(x7/sqrt7)+dxY7*(y7/sqrt7)+dxZ7*sign(z7)/axis7Z

gradA22X=(1.0-ind(A21,B21))*gradA21X+ind(A21,B21)*gradB21X

gradB22X=dxX9*(x9/sqrt9)+dxY9*(y9/sqrt9)+dxZ9*sign(z9)/axis9Z

gradA23X=(1.0-ind(A22,B22))*gradA22X+ind(A22,B22)*gradB22X

gradB23X=dxX11*(x11/sqrt11)+dxY11*(y11/sqrt11)+dxZ11*sign(z11)/axis11Z

gradA24X=(1.0-ind(A23,B23))*gradA23X+ind(A23,B23)*gradB23X

gradB24X=dxX13*(x13/sqrt13)+dxY13*(y13/sqrt13)+dxZ13*sign(z13)/axis13Z

gradA25X=(1.0-ind(A24,B24))*gradA24X+ind(A24,B24)*gradB24X

gradB25X=dxX15*(x15/sqrt15)+dxY15*(y15/sqrt15)+dxZ15*sign(z15)/axis15Z

gradA26X=(1.0-ind(A25,B25))*gradA25X+ind(A25,B25)*gradB25X

gradB26X=dxX17*(x17/sqrt17)+dxY17*(y17/sqrt17)+dxZ17*sign(z17)/axis17Z

gradA27X=(1.0-ind(A26,B26))*gradA26X+ind(A26,B26)*gradB26X

gradB27X=dxX19*(x19/sqrt19)+dxY19*(y19/sqrt19)+dxZ19*sign(z19)/axis19Z

gradA28X=(1.0-ind(A27,B27))*gradA27X+ind(A27,B27)*gradB27X

gradB28X=dxX21*(x21/sqrt21)+dxY21*(y21/sqrt21)+dxZ21*sign(z21)/axis21Z

gradA29X=(1.0-ind(A28,B28))*gradA28X+ind(A28,B28)*gradB28X

gradB29X=dxX23*(x23/sqrt23)+dxY23*(y23/sqrt23)+dxZ23*sign(z23)/axis23Z

gradA30X=(1.0-ind(A29,B29))*gradA29X+ind(A29,B29)*gradB29X

gradB30X=dxX25*(x25/sqrt25)+dxY25*(y25/sqrt25)+dxZ25*sign(z25)/axis25Z

gradA31X=dxX6*ind(radial6,height6)*2.0*x6/(axis6X*axis6X)+dxY6*ind(radial6,height6)*2.0*y6/(axis6Y*axis6Y)+dxZ6*ind(height6,radial6)*sign(z6)

gradB31X=dxX8*(x8/sqrt8)+dxY8*(y8/sqrt8)+dxZ8*sign(z8)/axis8Z

gradA32X=(1.0-ind(A31,B31))*gradA31X+ind(A31,B31)*gradB31X

gradB32X=dxX10*(x10/sqrt10)+dxY10*(y10/sqrt10)+dxZ10*sign(z10)/axis10Z

gradA33X=(1.0-ind(A32,B32))*gradA32X+ind(A32,B32)*gradB32X

gradB33X=dxX12*(x12/sqrt12)+dxY12*(y12/sqrt12)+dxZ12*sign(z12)/axis12Z

gradA34X=(1.0-ind(A33,B33))*gradA33X+ind(A33,B33)*gradB33X

gradB34X=dxX14*(x14/sqrt14)+dxY14*(y14/sqrt14)+dxZ14*sign(z14)/axis14Z

gradA35X=(1.0-ind(A34,B34))*gradA34X+ind(A34,B34)*gradB34X

gradB35X=dxX16*(x16/sqrt16)+dxY16*(y16/sqrt16)+dxZ16*sign(z16)/axis16Z

gradA36X=(1.0-ind(A35,B35))*gradA35X+ind(A35,B35)*gradB35X

gradB36X=dxX18*(x18/sqrt18)+dxY18*(y18/sqrt18)+dxZ18*sign(z18)/axis18Z

gradA37X=(1.0-ind(A36,B36))*gradA36X+ind(A36,B36)*gradB36X

gradB37X=dxX20*(x20/sqrt20)+dxY20*(y20/sqrt20)+dxZ20*sign(z20)/axis20Z

gradA38X=(1.0-ind(A37,B37))*gradA37X+ind(A37,B37)*gradB37X

gradB38X=dxX22*(x22/sqrt22)+dxY22*(y22/sqrt22)+dxZ22*sign(z22)/axis22Z

gradA39X=(1.0-ind(A38,B38))*gradA38X+ind(A38,B38)*gradB38X

gradB39X=dxX24*(x24/sqrt24)+dxY24*(y24/sqrt24)+dxZ24*sign(z24)/axis24Z

gradA40X=(1.0-ind(A39,B39))*gradA39X+ind(A39,B39)*gradB39X

gradB40X=dxX26*(x26/sqrt26)+dxY26*(y26/sqrt26)+dxZ26*sign(z26)/axis26Z

gradA41X=(1.0-ind(A30,B30))*gradA30X+ind(A30,B30)*gradB30X

gradB41X=(1.0-ind(A40,B40))*gradA40X+ind(A40,B40)*gradB40X

#Litteral expression  of the  GRADX is 
gradX=ind(A41,-B41)*gradA41X+(1.0-ind(A41,-B41))*(-gradB41X)


#LISTE DES GRADIENTS DES PRIMITIVES SELON Y


dyX0=(-cosPhi0)*sinTheta0
dyY0=cosTheta0
dyZ0=-sinPhi0*sinTheta0


dyX1=(-cosPhi1)*sinTheta1
dyY1=cosTheta1
dyZ1=-sinPhi1*sinTheta1


dyX2=(-cosPhi2)*sinTheta2
dyY2=cosTheta2
dyZ2=-sinPhi2*sinTheta2


dyX3=(-cosPhi3)*sinTheta3
dyY3=cosTheta3
dyZ3=-sinPhi3*sinTheta3


dyX4=(-cosPhi4)*sinTheta4
dyY4=cosTheta4
dyZ4=-sinPhi4*sinTheta4


dyX5=(-cosPhi5)*sinTheta5
dyY5=cosTheta5
dyZ5=-sinPhi5*sinTheta5


dyX6=(-cosPhi6)*sinTheta6
dyY6=cosTheta6
dyZ6=-sinPhi6*sinTheta6


dyX7=(-cosPhi7)*sinTheta7
dyY7=cosTheta7
dyZ7=-sinPhi7*sinTheta7


dyX8=(-cosPhi8)*sinTheta8
dyY8=cosTheta8
dyZ8=-sinPhi8*sinTheta8


dyX9=(-cosPhi9)*sinTheta9
dyY9=cosTheta9
dyZ9=-sinPhi9*sinTheta9


dyX10=(-cosPhi10)*sinTheta10
dyY10=cosTheta10
dyZ10=-sinPhi10*sinTheta10


dyX11=(-cosPhi11)*sinTheta11
dyY11=cosTheta11
dyZ11=-sinPhi11*sinTheta11


dyX12=(-cosPhi12)*sinTheta12
dyY12=cosTheta12
dyZ12=-sinPhi12*sinTheta12


dyX13=(-cosPhi13)*sinTheta13
dyY13=cosTheta13
dyZ13=-sinPhi13*sinTheta13


dyX14=(-cosPhi14)*sinTheta14
dyY14=cosTheta14
dyZ14=-sinPhi14*sinTheta14


dyX15=(-cosPhi15)*sinTheta15
dyY15=cosTheta15
dyZ15=-sinPhi15*sinTheta15


dyX16=(-cosPhi16)*sinTheta16
dyY16=cosTheta16
dyZ16=-sinPhi16*sinTheta16


dyX17=(-cosPhi17)*sinTheta17
dyY17=cosTheta17
dyZ17=-sinPhi17*sinTheta17


dyX18=(-cosPhi18)*sinTheta18
dyY18=cosTheta18
dyZ18=-sinPhi18*sinTheta18


dyX19=(-cosPhi19)*sinTheta19
dyY19=cosTheta19
dyZ19=-sinPhi19*sinTheta19


dyX20=(-cosPhi20)*sinTheta20
dyY20=cosTheta20
dyZ20=-sinPhi20*sinTheta20


dyX21=(-cosPhi21)*sinTheta21
dyY21=cosTheta21
dyZ21=-sinPhi21*sinTheta21


dyX22=(-cosPhi22)*sinTheta22
dyY22=cosTheta22
dyZ22=-sinPhi22*sinTheta22


dyX23=(-cosPhi23)*sinTheta23
dyY23=cosTheta23
dyZ23=-sinPhi23*sinTheta23


dyX24=(-cosPhi24)*sinTheta24
dyY24=cosTheta24
dyZ24=-sinPhi24*sinTheta24


dyX25=(-cosPhi25)*sinTheta25
dyY25=cosTheta25
dyZ25=-sinPhi25*sinTheta25


dyX26=(-cosPhi26)*sinTheta26
dyY26=cosTheta26
dyZ26=-sinPhi26*sinTheta26


#LISTE DES GRADIENTS INTERMEDIAIRES SELON Y


gradA0Y=dyX5*ind(radial5,height5)*2.0*x5/(axis5X*axis5X)+dyY5*ind(radial5,height5)*2.0*y5/(axis5Y*axis5Y)+dyZ5*ind(height5,radial5)*sign(z5)

gradB0Y=dyX7*(x7/sqrt7)+dyY7*(y7/sqrt7)+dyZ7*sign(z7)/axis7Z

gradA1Y=(1.0-ind(A0,B0))*gradA0Y+ind(A0,B0)*gradB0Y

gradB1Y=dyX9*(x9/sqrt9)+dyY9*(y9/sqrt9)+dyZ9*sign(z9)/axis9Z

gradA2Y=(1.0-ind(A1,B1))*gradA1Y+ind(A1,B1)*gradB1Y

gradB2Y=dyX11*(x11/sqrt11)+dyY11*(y11/sqrt11)+dyZ11*sign(z11)/axis11Z

gradA3Y=(1.0-ind(A2,B2))*gradA2Y+ind(A2,B2)*gradB2Y

gradB3Y=dyX13*(x13/sqrt13)+dyY13*(y13/sqrt13)+dyZ13*sign(z13)/axis13Z

gradA4Y=(1.0-ind(A3,B3))*gradA3Y+ind(A3,B3)*gradB3Y

gradB4Y=dyX15*(x15/sqrt15)+dyY15*(y15/sqrt15)+dyZ15*sign(z15)/axis15Z

gradA5Y=(1.0-ind(A4,B4))*gradA4Y+ind(A4,B4)*gradB4Y

gradB5Y=dyX17*(x17/sqrt17)+dyY17*(y17/sqrt17)+dyZ17*sign(z17)/axis17Z

gradA6Y=(1.0-ind(A5,B5))*gradA5Y+ind(A5,B5)*gradB5Y

gradB6Y=dyX19*(x19/sqrt19)+dyY19*(y19/sqrt19)+dyZ19*sign(z19)/axis19Z

gradA7Y=(1.0-ind(A6,B6))*gradA6Y+ind(A6,B6)*gradB6Y

gradB7Y=dyX21*(x21/sqrt21)+dyY21*(y21/sqrt21)+dyZ21*sign(z21)/axis21Z

gradA8Y=(1.0-ind(A7,B7))*gradA7Y+ind(A7,B7)*gradB7Y

gradB8Y=dyX23*(x23/sqrt23)+dyY23*(y23/sqrt23)+dyZ23*sign(z23)/axis23Z

gradA9Y=(1.0-ind(A8,B8))*gradA8Y+ind(A8,B8)*gradB8Y

gradB9Y=dyX25*(x25/sqrt25)+dyY25*(y25/sqrt25)+dyZ25*sign(z25)/axis25Z

gradA10Y=dyX6*ind(radial6,height6)*2.0*x6/(axis6X*axis6X)+dyY6*ind(radial6,height6)*2.0*y6/(axis6Y*axis6Y)+dyZ6*ind(height6,radial6)*sign(z6)

gradB10Y=dyX8*(x8/sqrt8)+dyY8*(y8/sqrt8)+dyZ8*sign(z8)/axis8Z

gradA11Y=(1.0-ind(A10,B10))*gradA10Y+ind(A10,B10)*gradB10Y

gradB11Y=dyX10*(x10/sqrt10)+dyY10*(y10/sqrt10)+dyZ10*sign(z10)/axis10Z

gradA12Y=(1.0-ind(A11,B11))*gradA11Y+ind(A11,B11)*gradB11Y

gradB12Y=dyX12*(x12/sqrt12)+dyY12*(y12/sqrt12)+dyZ12*sign(z12)/axis12Z

gradA13Y=(1.0-ind(A12,B12))*gradA12Y+ind(A12,B12)*gradB12Y

gradB13Y=dyX14*(x14/sqrt14)+dyY14*(y14/sqrt14)+dyZ14*sign(z14)/axis14Z

gradA14Y=(1.0-ind(A13,B13))*gradA13Y+ind(A13,B13)*gradB13Y

gradB14Y=dyX16*(x16/sqrt16)+dyY16*(y16/sqrt16)+dyZ16*sign(z16)/axis16Z

gradA15Y=(1.0-ind(A14,B14))*gradA14Y+ind(A14,B14)*gradB14Y

gradB15Y=dyX18*(x18/sqrt18)+dyY18*(y18/sqrt18)+dyZ18*sign(z18)/axis18Z

gradA16Y=(1.0-ind(A15,B15))*gradA15Y+ind(A15,B15)*gradB15Y

gradB16Y=dyX20*(x20/sqrt20)+dyY20*(y20/sqrt20)+dyZ20*sign(z20)/axis20Z

gradA17Y=(1.0-ind(A16,B16))*gradA16Y+ind(A16,B16)*gradB16Y

gradB17Y=dyX22*(x22/sqrt22)+dyY22*(y22/sqrt22)+dyZ22*sign(z22)/axis22Z

gradA18Y=(1.0-ind(A17,B17))*gradA17Y+ind(A17,B17)*gradB17Y

gradB18Y=dyX24*(x24/sqrt24)+dyY24*(y24/sqrt24)+dyZ24*sign(z24)/axis24Z

gradA19Y=(1.0-ind(A18,B18))*gradA18Y+ind(A18,B18)*gradB18Y

gradB19Y=dyX26*(x26/sqrt26)+dyY26*(y26/sqrt26)+dyZ26*sign(z26)/axis26Z

gradA20Y=(1.0-ind(A9,B9))*gradA9Y+ind(A9,B9)*gradB9Y

gradB20Y=(1.0-ind(A19,B19))*gradA19Y+ind(A19,B19)*gradB19Y

gradA21Y=dyX5*ind(radial5,height5)*2.0*x5/(axis5X*axis5X)+dyY5*ind(radial5,height5)*2.0*y5/(axis5Y*axis5Y)+dyZ5*ind(height5,radial5)*sign(z5)

gradB21Y=dyX7*(x7/sqrt7)+dyY7*(y7/sqrt7)+dyZ7*sign(z7)/axis7Z

gradA22Y=(1.0-ind(A21,B21))*gradA21Y+ind(A21,B21)*gradB21Y

gradB22Y=dyX9*(x9/sqrt9)+dyY9*(y9/sqrt9)+dyZ9*sign(z9)/axis9Z

gradA23Y=(1.0-ind(A22,B22))*gradA22Y+ind(A22,B22)*gradB22Y

gradB23Y=dyX11*(x11/sqrt11)+dyY11*(y11/sqrt11)+dyZ11*sign(z11)/axis11Z

gradA24Y=(1.0-ind(A23,B23))*gradA23Y+ind(A23,B23)*gradB23Y

gradB24Y=dyX13*(x13/sqrt13)+dyY13*(y13/sqrt13)+dyZ13*sign(z13)/axis13Z

gradA25Y=(1.0-ind(A24,B24))*gradA24Y+ind(A24,B24)*gradB24Y

gradB25Y=dyX15*(x15/sqrt15)+dyY15*(y15/sqrt15)+dyZ15*sign(z15)/axis15Z

gradA26Y=(1.0-ind(A25,B25))*gradA25Y+ind(A25,B25)*gradB25Y

gradB26Y=dyX17*(x17/sqrt17)+dyY17*(y17/sqrt17)+dyZ17*sign(z17)/axis17Z

gradA27Y=(1.0-ind(A26,B26))*gradA26Y+ind(A26,B26)*gradB26Y

gradB27Y=dyX19*(x19/sqrt19)+dyY19*(y19/sqrt19)+dyZ19*sign(z19)/axis19Z

gradA28Y=(1.0-ind(A27,B27))*gradA27Y+ind(A27,B27)*gradB27Y

gradB28Y=dyX21*(x21/sqrt21)+dyY21*(y21/sqrt21)+dyZ21*sign(z21)/axis21Z

gradA29Y=(1.0-ind(A28,B28))*gradA28Y+ind(A28,B28)*gradB28Y

gradB29Y=dyX23*(x23/sqrt23)+dyY23*(y23/sqrt23)+dyZ23*sign(z23)/axis23Z

gradA30Y=(1.0-ind(A29,B29))*gradA29Y+ind(A29,B29)*gradB29Y

gradB30Y=dyX25*(x25/sqrt25)+dyY25*(y25/sqrt25)+dyZ25*sign(z25)/axis25Z

gradA31Y=dyX6*ind(radial6,height6)*2.0*x6/(axis6X*axis6X)+dyY6*ind(radial6,height6)*2.0*y6/(axis6Y*axis6Y)+dyZ6*ind(height6,radial6)*sign(z6)

gradB31Y=dyX8*(x8/sqrt8)+dyY8*(y8/sqrt8)+dyZ8*sign(z8)/axis8Z

gradA32Y=(1.0-ind(A31,B31))*gradA31Y+ind(A31,B31)*gradB31Y

gradB32Y=dyX10*(x10/sqrt10)+dyY10*(y10/sqrt10)+dyZ10*sign(z10)/axis10Z

gradA33Y=(1.0-ind(A32,B32))*gradA32Y+ind(A32,B32)*gradB32Y

gradB33Y=dyX12*(x12/sqrt12)+dyY12*(y12/sqrt12)+dyZ12*sign(z12)/axis12Z

gradA34Y=(1.0-ind(A33,B33))*gradA33Y+ind(A33,B33)*gradB33Y

gradB34Y=dyX14*(x14/sqrt14)+dyY14*(y14/sqrt14)+dyZ14*sign(z14)/axis14Z

gradA35Y=(1.0-ind(A34,B34))*gradA34Y+ind(A34,B34)*gradB34Y

gradB35Y=dyX16*(x16/sqrt16)+dyY16*(y16/sqrt16)+dyZ16*sign(z16)/axis16Z

gradA36Y=(1.0-ind(A35,B35))*gradA35Y+ind(A35,B35)*gradB35Y

gradB36Y=dyX18*(x18/sqrt18)+dyY18*(y18/sqrt18)+dyZ18*sign(z18)/axis18Z

gradA37Y=(1.0-ind(A36,B36))*gradA36Y+ind(A36,B36)*gradB36Y

gradB37Y=dyX20*(x20/sqrt20)+dyY20*(y20/sqrt20)+dyZ20*sign(z20)/axis20Z

gradA38Y=(1.0-ind(A37,B37))*gradA37Y+ind(A37,B37)*gradB37Y

gradB38Y=dyX22*(x22/sqrt22)+dyY22*(y22/sqrt22)+dyZ22*sign(z22)/axis22Z

gradA39Y=(1.0-ind(A38,B38))*gradA38Y+ind(A38,B38)*gradB38Y

gradB39Y=dyX24*(x24/sqrt24)+dyY24*(y24/sqrt24)+dyZ24*sign(z24)/axis24Z

gradA40Y=(1.0-ind(A39,B39))*gradA39Y+ind(A39,B39)*gradB39Y

gradB40Y=dyX26*(x26/sqrt26)+dyY26*(y26/sqrt26)+dyZ26*sign(z26)/axis26Z

gradA41Y=(1.0-ind(A30,B30))*gradA30Y+ind(A30,B30)*gradB30Y

gradB41Y=(1.0-ind(A40,B40))*gradA40Y+ind(A40,B40)*gradB40Y

#Litteral expression  of the  GRADY is gradY
gradY=ind(A41,-B41)*gradA41Y+(1.0-ind(A41,-B41))*(-gradB41Y)


#LISTE DES GRADIENTS DES PRIMITIVES SELON Z


dzX0=sinPhi0
dzY0=0.0
dzZ0=cosPhi0


dzX1=sinPhi1
dzY1=0.0
dzZ1=cosPhi1


dzX2=sinPhi2
dzY2=0.0
dzZ2=cosPhi2


dzX3=sinPhi3
dzY3=0.0
dzZ3=cosPhi3


dzX4=sinPhi4
dzY4=0.0
dzZ4=cosPhi4


dzX5=sinPhi5
dzY5=0.0
dzZ5=cosPhi5


dzX6=sinPhi6
dzY6=0.0
dzZ6=cosPhi6


dzX7=sinPhi7
dzY7=0.0
dzZ7=cosPhi7


dzX8=sinPhi8
dzY8=0.0
dzZ8=cosPhi8


dzX9=sinPhi9
dzY9=0.0
dzZ9=cosPhi9


dzX10=sinPhi10
dzY10=0.0
dzZ10=cosPhi10


dzX11=sinPhi11
dzY11=0.0
dzZ11=cosPhi11


dzX12=sinPhi12
dzY12=0.0
dzZ12=cosPhi12


dzX13=sinPhi13
dzY13=0.0
dzZ13=cosPhi13


dzX14=sinPhi14
dzY14=0.0
dzZ14=cosPhi14


dzX15=sinPhi15
dzY15=0.0
dzZ15=cosPhi15


dzX16=sinPhi16
dzY16=0.0
dzZ16=cosPhi16


dzX17=sinPhi17
dzY17=0.0
dzZ17=cosPhi17


dzX18=sinPhi18
dzY18=0.0
dzZ18=cosPhi18


dzX19=sinPhi19
dzY19=0.0
dzZ19=cosPhi19


dzX20=sinPhi20
dzY20=0.0
dzZ20=cosPhi20


dzX21=sinPhi21
dzY21=0.0
dzZ21=cosPhi21


dzX22=sinPhi22
dzY22=0.0
dzZ22=cosPhi22


dzX23=sinPhi23
dzY23=0.0
dzZ23=cosPhi23


dzX24=sinPhi24
dzY24=0.0
dzZ24=cosPhi24


dzX25=sinPhi25
dzY25=0.0
dzZ25=cosPhi25


dzX26=sinPhi26
dzY26=0.0
dzZ26=cosPhi26


#LISTE DES GRADIENTS INTERMEDIAIRES SELON Z


gradA0Z=dzX5*ind(radial5,height5)*2.0*x5/(axis5X*axis5X)+dzY5*ind(radial5,height5)*2.0*y5/(axis5Y*axis5Y)+dzZ5*ind(height5,radial5)*sign(z5)

gradB0Z=dzX7*(x7/sqrt7)+dzY7*(y7/sqrt7)+dzZ7*sign(z7)/axis7Z

gradA1Z=(1.0-ind(A0,B0))*gradA0Z+ind(A0,B0)*gradB0Z

gradB1Z=dzX9*(x9/sqrt9)+dzY9*(y9/sqrt9)+dzZ9*sign(z9)/axis9Z

gradA2Z=(1.0-ind(A1,B1))*gradA1Z+ind(A1,B1)*gradB1Z

gradB2Z=dzX11*(x11/sqrt11)+dzY11*(y11/sqrt11)+dzZ11*sign(z11)/axis11Z

gradA3Z=(1.0-ind(A2,B2))*gradA2Z+ind(A2,B2)*gradB2Z

gradB3Z=dzX13*(x13/sqrt13)+dzY13*(y13/sqrt13)+dzZ13*sign(z13)/axis13Z

gradA4Z=(1.0-ind(A3,B3))*gradA3Z+ind(A3,B3)*gradB3Z

gradB4Z=dzX15*(x15/sqrt15)+dzY15*(y15/sqrt15)+dzZ15*sign(z15)/axis15Z

gradA5Z=(1.0-ind(A4,B4))*gradA4Z+ind(A4,B4)*gradB4Z

gradB5Z=dzX17*(x17/sqrt17)+dzY17*(y17/sqrt17)+dzZ17*sign(z17)/axis17Z

gradA6Z=(1.0-ind(A5,B5))*gradA5Z+ind(A5,B5)*gradB5Z

gradB6Z=dzX19*(x19/sqrt19)+dzY19*(y19/sqrt19)+dzZ19*sign(z19)/axis19Z

gradA7Z=(1.0-ind(A6,B6))*gradA6Z+ind(A6,B6)*gradB6Z

gradB7Z=dzX21*(x21/sqrt21)+dzY21*(y21/sqrt21)+dzZ21*sign(z21)/axis21Z

gradA8Z=(1.0-ind(A7,B7))*gradA7Z+ind(A7,B7)*gradB7Z

gradB8Z=dzX23*(x23/sqrt23)+dzY23*(y23/sqrt23)+dzZ23*sign(z23)/axis23Z

gradA9Z=(1.0-ind(A8,B8))*gradA8Z+ind(A8,B8)*gradB8Z

gradB9Z=dzX25*(x25/sqrt25)+dzY25*(y25/sqrt25)+dzZ25*sign(z25)/axis25Z

gradA10Z=dzX6*ind(radial6,height6)*2.0*x6/(axis6X*axis6X)+dzY6*ind(radial6,height6)*2.0*y6/(axis6Y*axis6Y)+dzZ6*ind(height6,radial6)*sign(z6)

gradB10Z=dzX8*(x8/sqrt8)+dzY8*(y8/sqrt8)+dzZ8*sign(z8)/axis8Z

gradA11Z=(1.0-ind(A10,B10))*gradA10Z+ind(A10,B10)*gradB10Z

gradB11Z=dzX10*(x10/sqrt10)+dzY10*(y10/sqrt10)+dzZ10*sign(z10)/axis10Z

gradA12Z=(1.0-ind(A11,B11))*gradA11Z+ind(A11,B11)*gradB11Z

gradB12Z=dzX12*(x12/sqrt12)+dzY12*(y12/sqrt12)+dzZ12*sign(z12)/axis12Z

gradA13Z=(1.0-ind(A12,B12))*gradA12Z+ind(A12,B12)*gradB12Z

gradB13Z=dzX14*(x14/sqrt14)+dzY14*(y14/sqrt14)+dzZ14*sign(z14)/axis14Z

gradA14Z=(1.0-ind(A13,B13))*gradA13Z+ind(A13,B13)*gradB13Z

gradB14Z=dzX16*(x16/sqrt16)+dzY16*(y16/sqrt16)+dzZ16*sign(z16)/axis16Z

gradA15Z=(1.0-ind(A14,B14))*gradA14Z+ind(A14,B14)*gradB14Z

gradB15Z=dzX18*(x18/sqrt18)+dzY18*(y18/sqrt18)+dzZ18*sign(z18)/axis18Z

gradA16Z=(1.0-ind(A15,B15))*gradA15Z+ind(A15,B15)*gradB15Z

gradB16Z=dzX20*(x20/sqrt20)+dzY20*(y20/sqrt20)+dzZ20*sign(z20)/axis20Z

gradA17Z=(1.0-ind(A16,B16))*gradA16Z+ind(A16,B16)*gradB16Z

gradB17Z=dzX22*(x22/sqrt22)+dzY22*(y22/sqrt22)+dzZ22*sign(z22)/axis22Z

gradA18Z=(1.0-ind(A17,B17))*gradA17Z+ind(A17,B17)*gradB17Z

gradB18Z=dzX24*(x24/sqrt24)+dzY24*(y24/sqrt24)+dzZ24*sign(z24)/axis24Z

gradA19Z=(1.0-ind(A18,B18))*gradA18Z+ind(A18,B18)*gradB18Z

gradB19Z=dzX26*(x26/sqrt26)+dzY26*(y26/sqrt26)+dzZ26*sign(z26)/axis26Z

gradA20Z=(1.0-ind(A9,B9))*gradA9Z+ind(A9,B9)*gradB9Z

gradB20Z=(1.0-ind(A19,B19))*gradA19Z+ind(A19,B19)*gradB19Z

gradA21Z=dzX5*ind(radial5,height5)*2.0*x5/(axis5X*axis5X)+dzY5*ind(radial5,height5)*2.0*y5/(axis5Y*axis5Y)+dzZ5*ind(height5,radial5)*sign(z5)

gradB21Z=dzX7*(x7/sqrt7)+dzY7*(y7/sqrt7)+dzZ7*sign(z7)/axis7Z

gradA22Z=(1.0-ind(A21,B21))*gradA21Z+ind(A21,B21)*gradB21Z

gradB22Z=dzX9*(x9/sqrt9)+dzY9*(y9/sqrt9)+dzZ9*sign(z9)/axis9Z

gradA23Z=(1.0-ind(A22,B22))*gradA22Z+ind(A22,B22)*gradB22Z

gradB23Z=dzX11*(x11/sqrt11)+dzY11*(y11/sqrt11)+dzZ11*sign(z11)/axis11Z

gradA24Z=(1.0-ind(A23,B23))*gradA23Z+ind(A23,B23)*gradB23Z

gradB24Z=dzX13*(x13/sqrt13)+dzY13*(y13/sqrt13)+dzZ13*sign(z13)/axis13Z

gradA25Z=(1.0-ind(A24,B24))*gradA24Z+ind(A24,B24)*gradB24Z

gradB25Z=dzX15*(x15/sqrt15)+dzY15*(y15/sqrt15)+dzZ15*sign(z15)/axis15Z

gradA26Z=(1.0-ind(A25,B25))*gradA25Z+ind(A25,B25)*gradB25Z

gradB26Z=dzX17*(x17/sqrt17)+dzY17*(y17/sqrt17)+dzZ17*sign(z17)/axis17Z

gradA27Z=(1.0-ind(A26,B26))*gradA26Z+ind(A26,B26)*gradB26Z

gradB27Z=dzX19*(x19/sqrt19)+dzY19*(y19/sqrt19)+dzZ19*sign(z19)/axis19Z

gradA28Z=(1.0-ind(A27,B27))*gradA27Z+ind(A27,B27)*gradB27Z

gradB28Z=dzX21*(x21/sqrt21)+dzY21*(y21/sqrt21)+dzZ21*sign(z21)/axis21Z

gradA29Z=(1.0-ind(A28,B28))*gradA28Z+ind(A28,B28)*gradB28Z

gradB29Z=dzX23*(x23/sqrt23)+dzY23*(y23/sqrt23)+dzZ23*sign(z23)/axis23Z

gradA30Z=(1.0-ind(A29,B29))*gradA29Z+ind(A29,B29)*gradB29Z

gradB30Z=dzX25*(x25/sqrt25)+dzY25*(y25/sqrt25)+dzZ25*sign(z25)/axis25Z

gradA31Z=dzX6*ind(radial6,height6)*2.0*x6/(axis6X*axis6X)+dzY6*ind(radial6,height6)*2.0*y6/(axis6Y*axis6Y)+dzZ6*ind(height6,radial6)*sign(z6)

gradB31Z=dzX8*(x8/sqrt8)+dzY8*(y8/sqrt8)+dzZ8*sign(z8)/axis8Z

gradA32Z=(1.0-ind(A31,B31))*gradA31Z+ind(A31,B31)*gradB31Z

gradB32Z=dzX10*(x10/sqrt10)+dzY10*(y10/sqrt10)+dzZ10*sign(z10)/axis10Z

gradA33Z=(1.0-ind(A32,B32))*gradA32Z+ind(A32,B32)*gradB32Z

gradB33Z=dzX12*(x12/sqrt12)+dzY12*(y12/sqrt12)+dzZ12*sign(z12)/axis12Z

gradA34Z=(1.0-ind(A33,B33))*gradA33Z+ind(A33,B33)*gradB33Z

gradB34Z=dzX14*(x14/sqrt14)+dzY14*(y14/sqrt14)+dzZ14*sign(z14)/axis14Z

gradA35Z=(1.0-ind(A34,B34))*gradA34Z+ind(A34,B34)*gradB34Z

gradB35Z=dzX16*(x16/sqrt16)+dzY16*(y16/sqrt16)+dzZ16*sign(z16)/axis16Z

gradA36Z=(1.0-ind(A35,B35))*gradA35Z+ind(A35,B35)*gradB35Z

gradB36Z=dzX18*(x18/sqrt18)+dzY18*(y18/sqrt18)+dzZ18*sign(z18)/axis18Z

gradA37Z=(1.0-ind(A36,B36))*gradA36Z+ind(A36,B36)*gradB36Z

gradB37Z=dzX20*(x20/sqrt20)+dzY20*(y20/sqrt20)+dzZ20*sign(z20)/axis20Z

gradA38Z=(1.0-ind(A37,B37))*gradA37Z+ind(A37,B37)*gradB37Z

gradB38Z=dzX22*(x22/sqrt22)+dzY22*(y22/sqrt22)+dzZ22*sign(z22)/axis22Z

gradA39Z=(1.0-ind(A38,B38))*gradA38Z+ind(A38,B38)*gradB38Z

gradB39Z=dzX24*(x24/sqrt24)+dzY24*(y24/sqrt24)+dzZ24*sign(z24)/axis24Z

gradA40Z=(1.0-ind(A39,B39))*gradA39Z+ind(A39,B39)*gradB39Z

gradB40Z=dzX26*(x26/sqrt26)+dzY26*(y26/sqrt26)+dzZ26*sign(z26)/axis26Z

gradA41Z=(1.0-ind(A30,B30))*gradA30Z+ind(A30,B30)*gradB30Z

gradB41Z=(1.0-ind(A40,B40))*gradA40Z+ind(A40,B40)*gradB40Z

#Litteral expression  of the  GRADZ is
gradZ=ind(A41,-B41)*gradA41Z+(1.0-ind(A41,-B41))*(-gradB41Z)


print 'expression='+str(expression)
print 'gradX='+str(gradX)
print 'gradY='+str(gradY)
print 'gradZ='+str(gradZ)
