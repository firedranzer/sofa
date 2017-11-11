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
from shutil import copy2

## Récupère le chemin absolu du réportoire courant et l'ajoute aux chemin où python cherche
## les modules
sys.path.append(os.getcwd())

## Ajoute le répertoire précédant ainsi on a accès à sofalauncher comme un module python
## classique.
sys.path.append( os.path.dirname(os.getcwd()) )

import geneticalgo.algorithm
import  scenes.accordion.main


workdir="/tmp/sg"
params = {}
if len(sys.argv) not in [2,3]:
    print("USAGE: ./shapegenerator.py <workdir> <sceneparams>")
else:
    if len(sys.argv) => 2 :
        workdir = sys.argv[1]
    if len(sys.argv) == 3 :
        params = eval(sys.argv())

print("Saving results in "+workdir)
print("Custom scene params: "+str(params))

if not os.path.exists(workdir):
    os.mkdir(workdir)

copy2(os.getcwd() + "/fragmentCodePartOne", workdir + "/fragmentCodePartOne")
print("copied: " + os.getcwd() + "/fragmentCodePartOne to: " + workdir + "/fragmentCodePartOne")
copy2(os.getcwd() + "/fragmentCodePartTwo", workdir + "/fragmentCodePartTwo")
print("copied: " + os.getcwd() + "/fragmentCodePartTwo to: " + workdir + "/fragmentCodePartTwo")
copy2(os.getcwd() + "/webGLJSCode", workdir + "/webGLJSCode")
print("copied: " + os.getcwd() + "/webGLJSCode to: " + workdir + "/webGLJSCode")

params["nbInd"] = 5
params["crossTx"] = 1
params["nbIndMutated"] = 1
params["nbMutationsPerInd"] = 5

algo =geneticalgo.algorithm.GeneticAlgorithm(params)

algo.start(5,
           scenes.accordion.main.generateFunc,
           scenes.accordion.main.mutationFunc,
           scenes.accordion.main.crossFunc,
           scenes.accordion.main.evaluationFunc,
           scenes.accordion.main.selectionFunc,
           workdir
           )


url = "file://"+os.path.abspath(workdir)+"/index.html"
print(url)
webbrowser.open_new(url)
