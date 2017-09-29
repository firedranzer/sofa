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
import webbrowser

## Récupère le chemin absolu du réportoire courant et l'ajoute aux chemin où python cherche
## les modules
sys.path.append(os.getcwd())

## Ajoute le répertoire précédant ainsi on a accès à sofalauncher comme un module python
## classique.
sys.path.append( os.path.dirname(os.getcwd()) )

from geneticalgo import algorithm
from  scenes.accordion import main


workdir="/tmp/sg"
if len(sys.argv) !=2:
    print("USAGE: ./shapegenerator.py <workdir>")
    print("missing workdir, using: "+workdir)
else:
    workdir = sys.argv[1]
print("Saving resulst in "+workdir)
algo = algorithm.GeneticAlgorithm({"nbInd":4,
                                   "crossTx":2,
                                   "nbIndMutated":4,
                                   "nbMutationsPerInd":4})
algo.start(4,
           main.generateFunc,
           main.mutationFunc,
           main.crossFunc,
           main.evaluationFunc,
           main.selectionFunc,
           workdir
           )


url = "file://"+os.path.abspath(workdir)+"/index.html"
print(url)
webbrowser.open_new(url)
