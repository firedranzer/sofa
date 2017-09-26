#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - thomas.morzadec@inria.fr
##
####################################################################################################
import sys,os
## Récupère le chemin absolu du réportoire courant et l'ajoute aux chemin où python cherche
## les modules
sys.path.append(os.getcwd())
sys.path.append( os.path.dirname(os.getcwd())+"/sofalauncher" )
print("SEARCH: " +os.path.dirname(os.getcwd())+"/sofalauncher")
import sofalauncher
print("ZZZZZZZZZZZZZZZZdddddddddddddddddd")
#from sofalauncher import launcher

from geneticalgo import algorithm
import scenes.test.rien
from  scenes.accordion import main



print("ICICIDDDDDDDDDDDDDDDDC")
algo = algorithm.GeneticAlgorithm(2, 2, 2,2)
algo.start(4,
           main.generateFunc,
           main.mutationFunc,
           main.crossFunc,
           main.evaluationFunc,
           main.selectionFunc,
           "tempdir"
           )
