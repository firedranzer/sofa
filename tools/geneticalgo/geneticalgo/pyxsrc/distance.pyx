#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - damien.marchal@inria.fr
##     - thomas.morzadec@inria.fr
##
####################################################################################################

# distutils: language=c++
# cython: profile=True

import numpy
import math
import copy
import primitives2D

from libc.math cimport sin, cos, sqrt, floor



cdef bint crossed_vectors(primitives2D.Vector2D vect1, primitives2D.Vector2D vect2):

    cdef primitives2D.Vector2D vect_temp1 =  primitives2D.Vector2D(vect1.first, vect2.first)
    cdef primitives2D.Vector2D vect_temp2 =  primitives2D.Vector2D(vect1.first, vect2.second)

    if primitives2D.Det(vect1, vect_temp1) * primitives2D.Det(vect1, vect_temp2) < 0.0:
        return True
    else:
        return False

cdef double distance_point_point(primitives2D.Point2D p1, primitives2D.Point2D p2):

    return sqrt((p2.x-p1.x) * (p2.x-p1.x) + (p2.y-p1.y) * (p2.y-p1.y))

cdef double distance_point_vector(primitives2D.Point2D p, primitives2D.Vector2D vect):

    if  primitives2D.norm(vect)==0.0:

        return distance_point_point(p, vect.first)

    cdef vect_temp= primitives2D.Vector2D(vect.first, p)

    cdef double produit_vect = primitives2D.scal(vect, vect_temp)

    if produit_vect < 0.0 or produit_vect > primitives2D.norm(vect) * primitives2D.norm(vect_temp):

        return min(distance_point_point(vect.first, p), distance_point_point(vect.second, p))

    else:

        return sqrt(distance_point_point(vect.first, p) - produit_vect/(primitives2D.norm(vect) * primitives2D.norm(vect)))


cdef double distance_vector_vector(primitives2D.Vector2D vect1, primitives2D.Vector2D vect2):

    if crossed_vectors(vect1, vect2):

        return 0.0

    else:

        return min(distance_point_point(vect1.first, vect2.first), distance_point_point(vect1.first, vect2.second),\
                   distance_point_point(vect1.second, vect2.first), distance_point_point(vect2.second, vect2.second))



cdef double distance_point_right_square(primitives2D.Point2D p, primitives2D.Point2D bottom_square, primitives2D.Point2D top_square):

    cdef primitives2D.Point2D p1 = bottom_square
    cdef primitives2D.Point2D p2 = primitives2D.Point2D(top_square.x, bottom_square.y)
    cdef primitives2D.Point2D p3 = top_square
    cdef primitives2D.Point2D p4 = primitives2D.Point2D(bottom_square.x, top_square.y)


    cdef primitives2D.Vector2D v1 = primitives2D.Vector2D(p1, p2)
    cdef primitives2D.Vector2D v2 = primitives2D.Vector2D(p2, p3)
    cdef primitives2D.Vector2D v3 = primitives2D.Vector2D(p3, p4)
    cdef primitives2D.Vector2D v4 = primitives2D.Vector2D(p4, p1)

    cdef list liste_temp = [distance_point_vector(p, v1), distance_point_vector(p, v2), distance_point_vector(p, v3),\
                            distance_point_vector(p, v4)]

    cdef double d =  min(liste_temp)

    if d == 0.0:
        return d

    elif  bottom_square.x <= p.x <= top_square.x and bottom_square.y <= p.y <= top_square.y:
        return -d
    else:
        return d


cdef class box:

    def __init__(self, primitives2D.Point2D box_bottom, primitives2D.Point2D box_top):

        self.box_bottom = box_bottom
        self.box_top = box_top
        self.bottom_left_corner_in = None
        self.list_vects_sign = []
        self.list_vects_distance = []

cdef class polygon_boosted_evaluation:

    def __init__(self, list polygon, int nX, int nY, double thickness):

        if not (nX > 0 and nY > 0):
            raise ValueError, "I nedd a positive number of boxes"

        listX = [point.x for point in polygon]
        listY = [point.y for point in polygon]

        cdef double minX = min(listX)
        cdef double minY = min(listY)

        cdef primitives2D.Point2D externalPoint = primitives2D.Point2D(minX - 1.0, minY - 1.0)

        cdef double maxX = max(listX)
        cdef double maxY = max(listY)

        cdef double dx = (2.0 * thickness + maxX - minX)/nX
        cdef double dy = (2.0 * thickness + maxY - minY)/nY

        cdef list list_vect = []

        if len(polygon)<3:
            raise ValueError, "I need at least a triangle!"

        for i in range(len(polygon) - 1):
            list_vect.append(primitives2D.Vector2D(polygon[i], polygon[i+1]))

        cdef list list_boxes=[]

        cdef primitives2D.Vector2D v0
        cdef primitives2D.Vector2D v1
        cdef primitives2D.Vector2D v2
        cdef primitives2D.Vector2D v3
        cdef list list_temp
        cdef int int_temp


        for i in range(nX):
            list_boxes.append([])
            for j in range(nY):
                list_boxes[i].append(box(primitives2D.Point2D(minX + i * dx, minY + j * dy),\
                                          primitives2D.Point2D(minX + i * dx + dx, minY + j * dy + dy)))

                v0 = primitives2D.Vector2D(primitives2D.Point2D(minX + i * dx, minY + j * dy),\
                primitives2D.Point2D(minX + i * dx + dx, minY + j * dy))

                v1 = primitives2D.Vector2D(primitives2D.Point2D(minX + i * dx + dx, minY + j * dy),\
                primitives2D.Point2D(minX + i * dx + dx, minY + j * dy +dy))

                v2 = primitives2D.Vector2D(primitives2D.Point2D(minX + i * dx + dx, minY + j * dy + dy),\
                primitives2D.Point2D(minX + i * dx, minY + j * dy + dy))

                v3 = primitives2D.Vector2D(primitives2D.Point2D(minX + i * dx, minY + j * dy + dy),\
                primitives2D.Point2D(minX + i * dx, minY + j * dy))



                list_temp = [distance_vector_vector(v0, list_vect[0]), distance_vector_vector(v1, list_vect[0]),\
                                     distance_vector_vector(v2, list_vect[0]), distance_vector_vector(v3, list_vect[0])]

                for vect in list_vect:

                    if distance_vector_vector(v0, vect) == 0.0 or distance_vector_vector(v1, vect) == 0.0\
                    or distance_vector_vector(v2, vect) == 0.0 or distance_vector_vector(v3, vect) == 0.0:

                        list_boxes[i][j].list_vects_sign.append(vect)

                    else:

                        if distance_vector_vector(v0, vect) < list_temp[0]:
                            list_temp[0] = distance_vector_vector(v0, vect)

                        if distance_vector_vector(v1, vect) < list_temp[1]:
                            list_temp[1] = distance_vector_vector(v1, vect)

                        if distance_vector_vector(v2, vect) < list_temp[2]:
                            list_temp[2] = distance_vector_vector(v2, vect)

                        if distance_vector_vector(v3, vect) < list_temp[3]:
                            list_temp[3] = distance_vector_vector(v3, vect)

                list_boxes[i][j].list_vects_distance = list_boxes[i][j].list_vects_sign + list_temp

                int_temp = 0

                for vect in list_vect:

                    if crossed_vectors(primitives2D.Vector2D(externalPoint, primitives2D.Point2D(minX + i * dx, minY + j * dy)), vect):
                        int_temp+=1

                if int_temp%2 == 0:

                    list_boxes[i][j].bottom_left_corner_in = False



        self.minX = minX
        self.minY = minY
        self.maxX = maxX
        self.maxY = maxY
        self.list_boxes = list_boxes



cdef plateau(double Xmin, double Xmax, double Ymin, double Ymax, double thickness, primitives2D.Point2D p):

    if thickness <=0.00:
        raise ValueError, "thickness <=0.00"

    if Xmin < p.x < Xmax and Ymin < p.y < Ymax:
        return 1.0

    elif Xmin - thickness < p.x < Xmax + thickness and Ymin - thickness < p.y < Ymax + thickness:
        return ((p.x - (Xmin - thickness))/thickness, ((Xmax + thickness) - p.x)/thickness,\
                (p.y - (Ymin - thickness))/thickness, ((Ymax + thickness) - p.y)/thickness)
    else:
        return 0.0

