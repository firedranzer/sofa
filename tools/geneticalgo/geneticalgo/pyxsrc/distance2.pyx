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


cdef double equation_droite(primitives2D.Vector2D vect, primitives2D.Point2D p):
    if primitives2D.norm2(vect) <= 0.0:
        raise ValueError, "wrong Vector"

    return (vect.second.x-vect.first.x)*(p.y-vect.first.y)-(vect.second.y - vect.first.y)*(p.x-vect.first.x)

cdef bint point_in_vector(primitives2D.Point2D p, primitives2D.Vector2D vect):

    if  primitives2D.norm(vect)==0.0:

        if p.x == vect.first.x and p.y == vect.first.y:
            return True
        else:
            return False

    cdef vect_temp= primitives2D.Vector2D(vect.first, p)

    cdef double produit_scal = primitives2D.scal(vect, vect_temp)
    cdef double det = primitives2D.Det(vect, vect_temp)

    if det == 0.0 and produit_scal >= 0.0 and primitives2D.norm(vect) >= primitives2D.norm(vect_temp):

        return True

    else:

        return False

cdef bint crossed_vector_line(primitives2D.Vector2D vect1, primitives2D.Vector2D vect2):

    if primitives2D.norm2(vect1) == 0.0:
        return point_in_vector(vect1.first, vect2)

    if equation_droite(vect1, vect2.first) * equation_droite(vect1, vect2.second) < 0.0:
        return True
    else:
        return False

cdef bint crossed_vectors(primitives2D.Vector2D vect1, primitives2D.Vector2D vect2):

    return crossed_vector_line(vect1, vect2) and crossed_vector_line(vect2, vect1)


cdef double distance_point_point(primitives2D.Point2D p1, primitives2D.Point2D p2):

    return sqrt((p2.x-p1.x) * (p2.x-p1.x) + (p2.y-p1.y) * (p2.y-p1.y))



cdef double distance_point_vector(primitives2D.Point2D p, primitives2D.Vector2D vect):

    if  primitives2D.norm(vect)==0.0:

        return distance_point_point(p, vect.first)

    cdef vect_temp= primitives2D.Vector2D(vect.first, p)

    cdef double produit_scal = primitives2D.scal(vect, vect_temp)

    cdef point_temp = primitives2D.Point2D

    if produit_scal < 0.0 or produit_scal > primitives2D.norm(vect):

        return min(distance_point_point(vect.first, p), distance_point_point(vect.second, p))

    else:

        point_temp = primitives2D.Point2D(vect.first.x + (produit_scal/primitives2D.norm2(vect))*vect.firstCoord(),\
                                          vect.first.y + (produit_scal/primitives2D.norm2(vect))*vect.secondCoord())

        return distance_point_point(point_temp, p)




cdef double distance_vector_vector(primitives2D.Vector2D vect1, primitives2D.Vector2D vect2):

    if crossed_vectors(vect1, vect2):

        return 0.0

    else:

        return min(distance_point_vector(vect1.first, vect2), distance_point_vector(vect1.second, vect2),\
                   distance_point_vector(vect2.first, vect1), distance_point_vector(vect2.second, vect1))



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


cdef double distance_vect_right_square(primitives2D.Vector2D vect, primitives2D.Point2D bottom_square, primitives2D.Point2D top_square):

    cdef primitives2D.Point2D p1 = bottom_square
    cdef primitives2D.Point2D p2 = primitives2D.Point2D(top_square.x, bottom_square.y)
    cdef primitives2D.Point2D p3 = top_square
    cdef primitives2D.Point2D p4 = primitives2D.Point2D(bottom_square.x, top_square.y)


    cdef primitives2D.Vector2D v1 = primitives2D.Vector2D(p1, p2)
    cdef primitives2D.Vector2D v2 = primitives2D.Vector2D(p2, p3)
    cdef primitives2D.Vector2D v3 = primitives2D.Vector2D(p3, p4)
    cdef primitives2D.Vector2D v4 = primitives2D.Vector2D(p4, p1)

    cdef list liste_temp = [distance_vector_vector(vect, v1), distance_vector_vector(vect, v2), distance_vector_vector(vect, v3),\
                            distance_vector_vector(vect, v4)]

    cdef double d =  min(liste_temp)

    if d == 0.0:
        return d

    elif  bottom_square.x <= vect.first.x <= top_square.x and bottom_square.y <= vect.first.y <= top_square.y:
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

cdef double plateau(double Xmin, double Xmax, double Ymin, double Ymax, double thickness, primitives2D.Point2D p):

    if thickness <= 0.00:
        raise ValueError, "thickness <= 0.00"

    if Xmin < p.x < Xmax and Ymin < p.y < Ymax:
        return 1.0

    elif Xmin - thickness < p.x < Xmax + thickness and Ymin - thickness < p.y < Ymax + thickness:
        return min((p.x - (Xmin - thickness))/thickness, ((Xmax + thickness) - p.x)/thickness,\
                (p.y - (Ymin - thickness))/thickness, ((Ymax + thickness) - p.y)/thickness)
    else:
        return 0.0

cdef class polygon_boosted_evaluation(primitives2D.Shape2D):

    def __init__(self, list polygon, int nX = 10, int nY = 10, double thickness= 0.01):

        if not (nX > 0 and nY > 0):
            raise ValueError, "I nedd a positive number of boxes"

        primitives2D.Shape2D.__init__(self)
        self.type = "polygon"

        listX = [point.x for point in polygon]
        listY = [point.y for point in polygon]

        cdef double minX = min(listX)
        cdef double minY = min(listY)

        cdef primitives2D.Point2D externalPoint = primitives2D.Point2D(minX - 1.0, minY - 1.0)

        cdef double maxX = max(listX)
        cdef double maxY = max(listY)

        cdef double dx = (2.0 * thickness + maxX - minX)/nX
        cdef double dy = (2.0 * thickness + maxY - minY)/nY

        if dx == 0.0 or dy == 0.0:
            raise ValueError, "flat polygon"

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
        cdef primitives2D.Point2D bottom_left
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



                list_temp = [10000.0, 100000.0,100000.0,1000000.0]

                for vect in list_vect:

                    if distance_vect_right_square(vect, list_boxes[i][j].box_bottom, list_boxes[i][j].box_top) > 0.0:

                        if distance_vector_vector(v0, vect) < list_temp[0]:
                            list_temp[0] = distance_vector_vector(v0, vect)

                        if distance_vector_vector(v1, vect) < list_temp[1]:
                            list_temp[1] = distance_vector_vector(v1, vect)

                        if distance_vector_vector(v2, vect) < list_temp[2]:
                            list_temp[2] = distance_vector_vector(v2, vect)

                        if distance_vector_vector(v3, vect) < list_temp[3]:
                            list_temp[3] = distance_vector_vector(v3, vect)


                for vect in list_vect:

                    if distance_vect_right_square(vect, list_boxes[i][j].box_bottom, list_boxes[i][j].box_top) <= 0.0:
                        list_boxes[i][j].list_vects_sign.append(vect)
                        list_boxes[i][j].list_vects_distance.append(vect)

                    elif distance_vector_vector(v0, vect) < list_temp[0] + dx or distance_vector_vector(v1, vect) < list_temp[1] + dy or\
                    distance_vector_vector(v2, vect) < list_temp[2] +dx or distance_vector_vector(v3, vect) < list_temp[3] + dy:
                        list_boxes[i][j].list_vects_distance.append(vect)


                bottom_left = primitives2D.Point2D(minX + i * dx, minY + j * dy)

                for vect in list_boxes[i][j].list_vects_sign:

                    if point_in_vector(bottom_left, vect):

                        list_boxes[i][j].bottom_left_corner_in = True


                if list_boxes[i][j].bottom_left_corner_in != True:

                    int_temp = 0

                    for vect in list_vect:

                        if crossed_vectors(primitives2D.Vector2D(externalPoint, bottom_left), vect):
                            int_temp+=1

                    if int_temp%2 == 0:

                        list_boxes[i][j].bottom_left_corner_in = False
                    else:
                        list_boxes[i][j].bottom_left_corner_in = True

                list_boxes[i][j].list_vects_sign = list_vect

        self.minX = minX
        self.minY = minY
        self.maxX = maxX
        self.maxY = maxY
        self.dx = dx
        self.dy = dy
        self.thickness = thickness
        self.list_boxes = list_boxes



    cdef double eval(self, primitives2D.Point2D p):

        if not (self.minX - self.thickness < p.x < self.maxX + self.thickness and self.minY - self.thickness < p.y < self.maxY + self.thickness):
            return 1.0

        cdef double f = plateau(self.minX, self.maxX, self.minY, self.maxY, self.thickness, p)

        cdef int i = <int>((p.x - self.minX - self.thickness)/self.dx)
        cdef int j = <int>((p.y - self.minY - self.thickness)/self.dy)


        cdef box box_p = self.list_boxes[i][j]

        cdef list list_temp = []

        for vect in box_p.list_vects_distance:

            list_temp.append(distance_point_vector(p, vect))

        cdef double distance = min(list_temp)

        cdef int i_temp = 0
        cdef primitives2D.Vector2D vect_temp = primitives2D.Vector2D(box_p.box_bottom, p)

        for vect in box_p.list_vects_sign:

            if crossed_vectors(vect, vect_temp):

                i_temp+=1

        if (box_p.bottom_left_corner_in and i_temp > 0 and i_temp%2 == 0) or (not box_p.bottom_left_corner_in and i_temp%2 == 1):

            return -1.0 * distance

        else:
            return 1.0#distance * f + (1-f) * distance_point_right_square(p, box_p.box_bottom, box_p.box_top)


