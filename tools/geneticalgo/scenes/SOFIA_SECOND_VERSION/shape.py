
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

listOfDrawnPoints = [[0.00, 0.1, 0.03, 0.05],\
                     [0.0, 0.075, 0.07, 0.09],\
                     [0.00, -0.1, 0.07, 0.05],\
                     [0.00, -0.15, 0.05, 0.07]
                     ]

hole1 = [[0.00, 0.075, 0.0, 0.0],\
        [0.0, 0.05, 0.03, 0.03],\
        [0.00, 0.025, 0.03, 0.03],\
        [0.00, -0.025, 0.02, 0.02],\
        [0.00, -0.045, 0.01, 0.01],\
        [0.00, -0.055, 0.04, 0.04],\
        ]

hole2 = [[0.015, -0.065, 0.0, 0.0],\
        [0.015, -0.070, 0.01, 0.01],\
        [0.015, -0.085, 0.01, 0.01]
        ]

hole3 = [[-0.015, -0.065, 0.0, 0.0],\
                [-0.015, -0.070, 0.01, 0.01],\
                [-0.015, -0.085, 0.01, 0.01]
                ]

list_holes = [hole1, hole2, hole3]

sofia = SOFIA.generateSOFIAManually(listOfDrawnPoints, list_holes, 0.05)

shape = sofia.getShape()


def evalField(x,y,z):
   return shape.eval(primitives.Point(x,y,z))



if False:

    ################
    ###
    ###EXAMPLE
    ###
    ################

    import numpy as np
    import random
    import matplotlib.pyplot as plt
    (n,p)=(300,300)
    x , y = np.meshgrid(np.linspace(-0.15, 0.15,n),np.linspace(-0.15,0.12,p))
    z = np.zeros((n,p))
    for i in range(n):
        for j in range(p):
            z[i][j] = shape.eval(primitives.Point(x[i][j], y[i][j], 0.025))
    plt.figure()
    levels = [-0.1, 0.0]
    contour = plt.contour(x, y, z, levels,)
    plt.clabel(contour, colors = 'k', fmt = '%2.1f', fontsize=12)
    plt.show()

