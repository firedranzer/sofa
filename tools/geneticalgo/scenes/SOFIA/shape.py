
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
                        [0.00, 0.035, [0.0, 0.05], [0.0, 0.05]],\
                        [0.00, 0.025, [0.00, 0.02], [0.00, 0.02]],\
                        [0.00, -0.025, [0.00, 0.02], [0.00, 0.02]],\
                        [0.00, -0.05, [0.02, 0.05], [0.02, 0.05]],\
                        [0.00, -0.075, [0.00, 0.05], [0.00, 0.05]],\
                        [0.00, -0.1, [0.00, 0.05], [0.00, 0.05]]]
sofia = SOFIA.generateSOFIAManually(listOfDrawnPoints, 0.05)

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

