####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - thomas.morzadec@inria.fr
##
####################################################################################################
from distutils.core import setup
from Cython.Build import cythonize

setup(
    ext_modules = cythonize("primitives.pyx")
)
