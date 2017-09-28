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

from geneticalgo import primitives

def create(self, height, radius, thickness):
        if thickness > radius:
            raise ValueError, "the thickness of the tube has to be less than the radius, otherwise it will be inefficient"

        elif radius > height/2.0:
            raise ValueError, "the radius of the tube has to be less than the half of the height, otherwise it will be strange"

        self.height=height
        self.radius=radius
        self.thickness=thickness
        self.listCavities=[]

def addCavity(self, cavity):
        """
        The cavity as to be given as [height,"type",axisX,axisY,axisZ]
        """
   
        [height,type,axisX,axisY,axisZ]=cavity

        if height>(self.height) or height<0.0:
            raise ValueError, "the height of the center of the cavity has to be between "+str(self.height)+ " and "+"0.0"+ " to ensure connectidness"

        elif max(axisX,axisY)<self.radius:
            raise ValueError, "the max dimension in the horizontal plane has to be bigger than the radius"


        self.listCavities.append(cavity)


def accordionFreeDimension(self, height, radius, thickness, listCavities):

    """
     The Cavities have to be given as [height,"type",axisX,axisY,axisZ]
     The shape is given has the difference between ShapePlus and ShapeMinus
    """

    create(self, height, radius, thickness)
    for cavity in self.listCavities:
        addCavity(self, cavity)

    shapePlus=primitives.Cylinder("+",self.radius,self.radius,self.height/2.0,0.0,0.0,primitives.Point(0.0,0.0,self.height/2.0))
    shapeMinus=primitives.Cylinder("+",self.radius-self.thickness,self.radius-self.thickness,self.height/2.0-self.thickness,0.0,0.0,primitives.Point(0.0,0.0,self.height/2.0))

    for cavity in self.listCavities:

        [height,type,axisX,axisY,axisZ]=cavity


        if type=="ellipsoid":

            shapePlus=primitives.Union(shapePlus,primitives.Ellipsoid("+",axisX,axisY,axisZ,0.0,0.0,primitives.Point(0.0,0.0,height)))
            shapeMinus=primitives.Union(shapeMinus,primitives.Ellipsoid("+",axisX-thickness,axisY-thickness,axisZ-thickness,0.0,0.0,primitives.Point(0.0,0.0,height)))

        elif type=="cylinder":

            shapePlus=primitives.Union(shapePlus,primitives.Cylinder("+",axisX,axisY,axisZ,0.0,0.0,primitives.Point(0.0,0.0,height)))
            shapeMinus=primitives.Union(shapeMinus,primitives.Cylinder("+",axisX-thickness,axisY-thickness,axisZ-thickness,0.0,0.0,primitives.Point(0.0,0.0,height)))

        elif type=="frisbee":
            shapePlus=primitives.Union(shapePlus,primitives.Frisbee("+",axisX,axisY,axisZ,0.0,0.0,primitives.Point(0.0,0.0,height)))
            shapeMinus=primitives.Union(shapeMinus,primitives.Frisbee("+",axisX-thickness,axisY-thickness,axisZ-thickness,0.0,0.0,primitives.Point(0.0,0.0,height)))

    shape=primitives.Difference(shapePlus,shapeMinus)

    return (shape,shapeMinus)


def accordionRecoveringGiven(heightTube,radiusTube, thickness, typeCavities,
                              listheightsJoiningPoints, listAxesX, listAxesY, Zrecovering):
    """
     There is a unique type of cavities. Only their height, axes among X and Y can variate.
     The heights are not the heights of the centers but the heights of the joining points
    """

    if not((len(listheightsJoiningPoints)+1)==len(listAxesX) and len(listAxesX)==len(listAxesY)):
        raise ValueError, "the lists have to have coordinated length"

    listheightsJoiningPoints.insert(0,0.5)
    listheightsJoiningPoints.append(heightTube)
    listheightsJoiningPoints.sort()

    if listheightsJoiningPoints[0]<0.5 or listheightsJoiningPoints[-1]>heightTube:
        raise ValueError, "the extremal joining points do not belong to the tube"


    listCavities=[]

    heightCenter=(listheightsJoiningPoints[0]+listheightsJoiningPoints[1])/2.0

    axisZ=(listheightsJoiningPoints[1]-listheightsJoiningPoints[0])/2.0

    listCavities.append([heightCenter,typeCavities,listAxesX[0],listAxesY[0],axisZ])

    if len(listheightsJoiningPoints)>1:

        for i in range(1,len(listheightsJoiningPoints)-1):

            heightCenter=(listheightsJoiningPoints[i]+listheightsJoiningPoints[i+1])/2.0

            axisZ=(1.0+float(Zrecovering))*(listheightsJoiningPoints[i+1]-listheightsJoiningPoints[i])/2.0

            listCavities.append([heightCenter,typeCavities,listAxesX[i],listAxesY[i],axisZ])

    return accordionFreeDimension(heightTube,radiusTube,thickness,listCavities)

def accordionUniform(heightTube,radiusTube, thickness, typeCavities, numberCavities, axisX, axisY, Zrecovering):

    """
     There is a unique type of cavities, a unique axisX and axisY. The Cavities are uniformly dispatched
    """

    if n==0:
        return accordionFreeDimension(heightTube,radiusTube,thickness,[])
    elif n==1:
        listheightsJoiningPoints=[]
    else:
        listheightsJoiningPoints=[0.5+float(i)*(heightTube-0.5)/float(n) for i in range(1,n)]

    listAxesX,listAxesY=[axisX for i in range(n)],[axisY for i in range(n)]


    return accordionRecoveringGiven(heightTube,radiusTube,thickness,typeCavities,listheightsJoiningPoints,listAxesX,listAxesY,Zrecovering)
