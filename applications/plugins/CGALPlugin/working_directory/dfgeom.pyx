from libc.math cimport cos, sin, sqrt, fmod
from cpython.pycapsule cimport *
# distutils: language = c++
# distutils: sources = /home/morzadec/Documents/sofa/src/applications/plugins/CGALPlugin/examples/vector.cpp
cdef extern from "/home/morzadec/Documents/sofa/src/applications/plugins/CGALPlugin/examples/vector.h" namespace "":
    cdef cppclass Vec3d:
            Vec3d()
            Vec3d(double x, double y, double z)
            double x,y,z
            void set(const double x, const double y, const double z)
            Vec3d operator+(const Vec3d)
            Vec3d operator-(const Vec3d)
            Vec3d mul(const double)const
            Vec3d div(const double)const
            double length() const
            Vec3d mod(const Vec3d)
            Vec3d vmin(const Vec3d) const
            Vec3d vmax(const Vec3d) const
            Vec3d vmin(const double) const
            Vec3d vmax(const double) const
            Vec3d abs() const

class ImplicitShape(object):
    def __init__(self, s):
        self.shapes = s

cdef class Shape:
    def __init__(self):
        None

    cdef double evalFieldC(Shape tgt, Vec3d p):
        return 0.0

    def evalField(self, p):
        return self.evalFieldC(Vec3d(p[0],p[1], p[2]))


cdef class Cylinder(Shape):
    cdef Vec3d c

    def __init__(self, c):
        self.c = Vec3d(c[0], c[1], c[2])

    cdef double evalFieldC(self, Vec3d p):
        cdef Vec3d d = Vec3d(p.x-self.c.x, p.y-self.c.y, 0)
        return d.length()-self.c.z

    def evalField(self, p):
        return self.evalFieldC(Vec3d(p[0],p[1], p[2]))


cdef class Box(Shape):
    cdef Vec3d b

    def __init__(self, c):
        self.b = Vec3d(c[0], c[1], c[2])

    cdef double evalFieldC(self, Vec3d p):
        cdef Vec3d d = p.abs() - self.b
        return d.vmax(0.0).length() + min( max(d.x,max(d.y,d.z)),0.0)

    def evalField(self, p):
        return self.evalFieldC(Vec3d(p[0],p[1], p[2]))

cdef class PythonShape(Shape):
    cdef double evalFieldC(self, Vec3d p):
        return  self.evalField((p.x, p.y, p.z))

cdef class Tiling(Shape):
    cdef Vec3d c
    cdef Shape target

    def __init__(self, target, c):
        self.c = Vec3d(c[0], c[1], c[2])
        self.target = target

    cdef double evalFieldC(self, Vec3d p):
        cdef Vec3d q = p.mod(self.c) - self.c.mul(0.5)
        return  self.target.evalFieldC(q)

cdef class Scale(Shape):
    cdef double factor
    cdef Shape target

    def __init__(self, target, factor):
        self.factor = factor
        self.target = target

    cdef double evalFieldC(self, Vec3d p):
        return self.target.evalFieldC(p.div(self.factor))*self.factor;

cdef class Sphere(Shape):
        cdef Vec3d center
        cdef double radius
        def __init__(self, *args, **kwargs):
            self.center = Vec3d(0,0,0)
            self.radius = 1.0

            if len(args) >= 1:
                self.center = Vec3d(args[0][0], args[0][1], args[0][2])

            if len(args) >= 2:
                self.radius = args[1]


            for k in kwargs:
                vv = eval(str(kwargs[k]))

                if k=="center":
                    if isinstance(vv, list):
                        self.center = Vec3d(vv[0], vv[1], vv[2])
                    else:
                        print("PROBLEM")
                elif k=="radius":
                    self.radius = vv

        def setCenter(self, a):
            self.center.x = self.center.x + a

        cdef double evalFieldC(self, Vec3d p):
            return  (self.center + p).length() - self.radius


cdef class Union(Shape):
        cdef Shape a
        cdef Shape b
        def __init__(self, a, b):
            self.a = a
            self.b = b

        cdef double evalFieldC(self, Vec3d p):
                return  min(self.a.evalFieldC(p), self.b.evalFieldC(p))


cdef class Difference(Shape):
        cdef Shape a
        cdef Shape b
        def __init__(self, a, b):
            self.a = a
            self.b = b

        cdef double evalFieldC(self, Vec3d p):
                return  max(self.a.evalFieldC(p), -self.b.evalFieldC(p))


cdef class Intersection(Shape):
        cdef Shape a
        cdef Shape b
        def __init__(self, a, b):
            self.a = a
            self.b = b

        cdef double evalFieldC(self, Vec3d p):
              return  max(self.a.evalFieldC(p), self.b.evalFieldC(p))

cdef double rawEvalFieldC(Shape tgt, double x, double y, double z):
    #with nogil:
    return tgt.evalFieldC(Vec3d(x,y,z))

cpdef double evalField(Shape tgt, x, y ,z):
    return rawEvalFieldC(tgt,x,y,z)


cdef getCythonRawFunctionC(Shape shape):
    return ( PyCapsule_New(<void*>rawEvalFieldC, "evalFunction", NULL), shape )

cpdef getCythonRawFunction(shape):
    print("INSTALLING A LOW LEVEL CYTHON HOOK TO: ")
    return getCythonRawFunctionC(shape)

