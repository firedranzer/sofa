from distutils.core import setup
from Cython.Build import cythonize

setup(
    ext_modules = cythonize("primitives.pyx")#,
#                             language="c++")
)

#setup(
#    ext_modules = cythonize("primitives.pxd")
#)

setup(
    ext_modules = cythonize("accordion.pyx")#,
#                             language="c++")
)
