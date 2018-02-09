
#!/usr/bin/env python
# -*- coding: utf-8 -*-
##################################################################################################### Copyright 2017 INRIA
### This file is part of the ShapeGenerator project.
### Contributors:
##     - thomas.morzadec@inria.fr
##     - damien.marchal@univ-lille1.fr
###################################################################################################
import sys
sys.path.append("/home/morzadec/Documents/sofa/src/tools/geneticalgo/geneticalgo")

import primitives
import SOFIA

listOfDrawnPoints = [[0.00, 0.1, [0.0, 0.03],[0.0, 0.03]],\
                        [0.0, 0.075, [0.00, 0.05],[0.0, 0.05]],\
                        [0.0, 0.05, [0.01, 0.05], [0.01, 0.05]],\
                        [0.00, 0.025, [0.01, 0.05], [0.01, 0.05]],\
                        [0.00, 0.00, [0.00, 0.02], [0.00, 0.02]],\
                        [0.00, -0.025, [0.00, 0.02], [0.00, 0.02]],\
                        [0.00, -0.05, [0.02, 0.05], [0.02, 0.05]],\
                        [0.00, -0.075, [0.00, 0.05], [0.00, 0.05]],\
                        [0.00, -0.1, [0.00, 0.05], [0.00, 0.05]]]
sofia = SOFIA.generateSOFIAManually(listOfDrawnPoints, 0.05)

shape = sofia.getShape()


def evalField(x,y,z):
   return shape.eval(primitives.Point(x,y,z))
