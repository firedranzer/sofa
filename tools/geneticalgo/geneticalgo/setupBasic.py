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


from setuptools import setup, find_packages, Extension
from Cython.Distutils import build_ext


ext_modules=[
    Extension("primitives2D",    # location of the resulting .so
             ["primitives2D.pyx"],) ]


setup(name='primitives2D',
      packages=find_packages(),
      cmdclass = {'build_ext': build_ext},
      ext_modules = ext_modules,
     )



ext_modules=[
    Extension("primitives",    # location of the resulting .so
             ["primitives.pyx"],) ]


setup(name='primitives',
      packages=find_packages(),
      cmdclass = {'build_ext': build_ext},
      ext_modules = ext_modules,
     )




#setup(
#    ext_modules = cythonize("primitives.pyx")
#)

#setup(
#    ext_modules = cythonize("primitives2D.pyx")
#)
