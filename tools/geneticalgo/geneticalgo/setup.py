from distutils.core import setup
from Cython.Build import cythonize

setup( name = "geneticalgo",
       packages = ["geneticalgo", "geneticalgo.primitives"],

    ext_modules = cythonize("primitives/*.pyx"),
)
