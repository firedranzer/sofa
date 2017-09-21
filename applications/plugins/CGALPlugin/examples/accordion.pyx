#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 CNRS, INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - damien.marchal@univ-lille1.fr
##     - thomas.morzadec@inria.fr
##
####################################################################################################
# distutils: language=c++
import primitives
cimport numpy
cimport cython
cimport primitives
cimport accordion

## Shape generation function


cdef class TubeWithCavities(object):

    def __init__(self, heigth,radius,thickness):

        if thickness>radius:
            raise ValueError, "the thickness of the tube has to be less than the radius, otherwise it will be inefficient"

        elif radius>heigth/2.0:
            raise ValueError, "the radius of the tube has to be less than the half of the heigth, otherwise it will be strange"

        self.heigth=heigth
        self.radius=radius
        self.thickness=thickness
        self.listCavities=[]

    cpdef addCavity(self, list cavity):
        """
        The cavity as to be given as [heigth,"type",axisX,axisY,axisZ]
        """

        cdef double heigth,axisX,axisY,axisZ
        cdef str type

        [heigth,type,axisX,axisY,axisZ]=cavity

        if heigth>(self.heigth) or heigth<0.0:
            raise ValueError, "the heigth of the center of the cavity has to be between "+str(self.heigth)+ " and "+"0.0"+ " to ensure connectidness"

        elif max(axisX,axisY)<self.radius:
            raise ValueError, "the max dimension in the horizontal plane has to be bigger than the radius"


        self.listCavities.append(cavity)


cpdef tuple accordionFreeDimension(double heigthTube, double radiusTube, double thickness, list listCavities):
    """
     The Cavities have to be given as [heigth,"type",axisX,axisY,axisZ]
     The shape is given has the difference between ShapePlus and ShapeMinus
    """
    tube=TubeWithCavities(heigthTube,radiusTube,thickness)

    cdef list cavity
    cdef double heigth,axisX,axisY,axisZ
    cdef str type

    for cavity in listCavities:
        try:
            tube.addCavity(cavity)

        except:

            print "the ill-defined cavity"+ str(cavity)+ "has not been added"
            pass


    shapePlus=primitives.Cylinder("+",tube.radius,tube.radius,tube.heigth/2.0,0.0,0.0,primitives.Point(0.0,0.0,tube.heigth/2.0))

    shapeMinus=primitives.Cylinder("+",tube.radius-tube.thickness,tube.radius-tube.thickness,tube.heigth/2.0-tube.thickness,0.0,0.0,primitives.Point(0.0,0.0,tube.heigth/2.0))

    for cavity in tube.listCavities:

        [heigth,type,axisX,axisY,axisZ]=cavity


        if type=="ellipsoid":

            shapePlus=primitives.Union(shapePlus,primitives.Ellipsoid("+",axisX,axisY,axisZ,0.0,0.0,primitives.Point(0.0,0.0,heigth)))
            shapeMinus=primitives.Union(shapeMinus,primitives.Ellipsoid("+",axisX-tube.thickness,axisY-tube.thickness,axisZ-tube.thickness,0.0,0.0,primitives.Point(0.0,0.0,heigth)))

        elif type=="cylinder":

            shapePlus=primitives.Union(shapePlus,primitives.Cylinder("+",axisX,axisY,axisZ,0.0,0.0,primitives.Point(0.0,0.0,heigth)))
            shapeMinus=primitives.Union(shapeMinus,primitives.Cylinder("+",axisX-tube.thickness,axisY-tube.thickness,axisZ-tube.thickness,0.0,0.0,primitives.Point(0.0,0.0,heigth)))

        elif type=="frisbee":
            shapePlus=primitives.Union(shapePlus,primitives.Frisbee("+",axisX,axisY,axisZ,0.0,0.0,primitives.Point(0.0,0.0,heigth)))
            shapeMinus=primitives.Union(shapeMinus,primitives.Frisbee("+",axisX-tube.thickness,axisY-tube.thickness,axisZ-tube.thickness,0.0,0.0,primitives.Point(0.0,0.0,heigth)))

    shape=primitives.Difference(shapePlus,shapeMinus)

    return (shape,shapeMinus)


cpdef tuple accordionRecoveringGiven(double heigthTube,double radiusTube, double thickness, str typeCavities,
                              list listheigthsJoiningPoints, list listAxesX, list listAxesY, double Zrecovering):
    """
     There is a unique type of cavities. Only their heigth, axes among X and Y can variate.
     The heigths are not the heigths of the centers but the heigths of the joining points
    """

    if not((len(listheigthsJoiningPoints)+1)==len(listAxesX) and len(listAxesX)==len(listAxesY)):
        raise ValueError, "the lists have to have coordinated length"

    listheigthsJoiningPoints.insert(0,0.0)
    listheigthsJoiningPoints.append(heigthTube)
    listheigthsJoiningPoints.sort()

    if listheigthsJoiningPoints[0]<0.0 or listheigthsJoiningPoints[-1]>heigthTube:
        raise ValueError, "the extremal joining points do not belong to the tube"


    listCavities=[]

    for i in range(len(listheigthsJoiningPoints)-1):

        heigthCenter=(listheigthsJoiningPoints[i]+listheigthsJoiningPoints[i+1])/2.0

        axisZ=(1.0+float(Zrecovering))*(listheigthsJoiningPoints[i+1]-listheigthsJoiningPoints[i])/2.0

        listCavities.append([heigthCenter,typeCavities,listAxesX[i],listAxesY[i],axisZ])

    return accordionFreeDimension(heigthTube,radiusTube,thickness,listCavities)

cpdef tuple accordionUniform(double heigthTube,double radiusTube, double thickness, str typeCavities,int numberCavities, double axisX, double axisY, double Zrecovering):
    """
     There is a unique type of cavities, a unique axisX and axisY. The Cavities are uniformly dispatched
    """

    cdef int n=numberCavities
    cdef int i

    if n==0:
        return accordionFreeDimension(heigthTube,radiusTube,thickness,[])
    elif n==1:
        listheigthsJoiningPoints=[]
    else:
        listheigthsJoiningPoints=[float(i)*heigthTube/float(n) for i in range(1,n)]

    listAxesX,listAxesY=[axisX for i in range(n)],[axisY for i in range(n)]


    return accordionRecoveringGiven(heigthTube,radiusTube,thickness,typeCavities,listheigthsJoiningPoints,listAxesX,listAxesY,Zrecovering)
