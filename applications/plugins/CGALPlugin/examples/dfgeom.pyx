from libc.math cimport cos, sin, sqrt
from cpython.pycapsule cimport *
# distutils: language = c++
# distutils: sources = /home/dmarchal/projects/2016/DEFROST/04-code/sofa-src/sofa/master/forkdefrost/sofa/applications/plugins/CGALPlugin/examples/vector.cpp
cdef extern from "/home/dmarchal/projects/2016/DEFROST/04-code/sofa-src/sofa/master/forkdefrost/sofa/applications/plugins/CGALPlugin/examples/vector.h" namespace "":
    cdef cppclass Vec3d:
            Vec3d()
            Vec3d(double x, double y, double z)
            double x,y,z
            void set(double x, double y, double z)
            Vec3d operator+(Vec3d)
            Vec3d operator-(Vec3d)
            double length()

cdef class Shape:
    def __init__(self):
        None

    cdef double evalField(Shape tgt, Vec3d p):
        return 0.0

cdef class Sphere(Shape):
        cdef Vec3d center
        cdef double radius
        def __init__(self, c, radius):
            self.center = Vec3d(c[0], c[1], c[2])
            self.radius = radius

        def setCenter(self, a):
            self.center.x = self.center.x + a

        cdef double evalField(self, Vec3d p):
            return  (self.center + p).length() - self.radius

cdef class Union(Shape):
        cdef Shape a
        cdef Shape b
        def __init__(self, a, b):
            self.a = a
            self.b = b

        cdef double evalField(self, Vec3d p):
                return  min(self.a.evalField(p), self.b.evalField(p))

cdef class Difference(Shape):
        cdef Shape a
        cdef Shape b
        def __init__(self, a, b):
            self.a = a
            self.b = b

        cdef double evalField(self, Vec3d p):
                return  max(-self.a.evalField(p), self.b.evalField(p))


cdef class Intersection(Shape):
        cdef Shape a
        cdef Shape b
        def __init__(self, a, b):
            self.a = a
            self.b = b

        cdef double evalField(self, Vec3d p):
                return  max(self.a.evalField(p), self.b.evalField(p))


cdef double evalFieldC(Shape tgt, Vec3d p):
    #with nogil:
    return tgt.evalField(p)

cpdef double evalField(Shape tgt, x, y ,z):
    return evalFieldC(tgt, Vec3d(x,y,z))

cdef installCythonHookC(obj, Shape shape):
    return PyCapsule_New(<void*>12345678, "evalFunction", NULL)

cpdef installCythonHook(obj, shape):
    print("INSTALLING A LOW LEVEL CYTHON HOOK TO: ")
    return installCythonHook(obj, shape)
