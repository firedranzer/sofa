#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - thomas.morzadec@inria.fr
##     - damien.marchal@univ-lille1.fr
##
####################################################################################################
import sys, os

## Récupère le chemin absolu du réportoire courant et l'ajoute aux chemin où python cherche
## les modules
sys.path.append(os.getcwd())

## Ajoute le répertoire précédant ainsi on a accès à sofalauncher comme un module python
## classique.
sys.path.append( os.path.dirname(os.getcwd()) )

from geneticalgo import algorithm
from  scenes.accordion import main

algo = algorithm.GeneticAlgorithm(4, 2, 2, 2)
algo.start(4,
           main.generateFunc,
           main.mutationFunc,
           main.crossFunc,
           main.evaluationFunc,
           main.selectionFunc,
           "tempdir"
           )
