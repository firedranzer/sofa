from distutils.core import setup
from Cython.Build import cythonize

setup( name = "geneticalgo",
       packages = ["geneticalgo", "geneticalgo.pyxsrc"],

    ext_modules = cythonize("pyxsrc/*.pyx"),
)
