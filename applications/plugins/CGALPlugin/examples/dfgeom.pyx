from libc.math cimport cos, sin, sqrt
from cpython.pycapsule cimport *
# distutils: language = c++
# distutils: sources = /home/morzadec/Documents/sofa/src/applications/plugins/CGALPlugin/examples/vector.cpp
cdef extern from "/home/morzadec/Documents/sofa/src/applications/plugins/CGALPlugin/examples/vector.h" namespace "":
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

    cdef double evalFieldC(Shape tgt, Vec3d p):
        return 0.0

    def evalField(self, p):
        return self.evalFieldC(Vec3d(p[0],p[1], p[2]))


cdef class PythonShape(Shape):
    cdef double evalFieldC(self, Vec3d p):
        return  self.evalField((p.x, p.y, p.z))

cdef class Sphere(Shape):
        cdef Vec3d center
        cdef double radius
        def __init__(self, c, radius):
            self.center = Vec3d(c[0], c[1], c[2])
            self.radius = radius

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
                return  max(-self.a.evalFieldC(p), self.b.evalFieldC(p))


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
