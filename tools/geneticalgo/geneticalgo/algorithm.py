#!/usr/bin/env python
# -*- coding: utf-8 -*-
####################################################################################################
## Copyright 2017 INRIA
##
## This file is part of the ShapeGenerator project.
##
## Contributors:
##     - damien.marchal@inria.fr
##     - thomas.morzadec@inria.fr
##
####################################################################################################
import primitives2D
import primitives
import random
import operator
import copy
import html
import os
import datetime
import sofalauncher.launcher
from html import HTML

import contextlib
import os

@contextlib.contextmanager
def inDirectory(path):
    """A context manager which changes the working directory to the given
    path, and then changes it back to its previous value on exit.
    """
    prev_cwd = os.getcwd()
    os.chdir(path)
    try:
        yield
    finally:
        os.chdir(prev_cwd)


## Indiviual
class Individual(object):
    def __init__(self):
        self.level = None
        self.id = None

def key_level(ind):
    return ind.level

def key_index(ind):
    return ind.id

class Population(object):
    def __init__(self):
        self.id = 0
        self.pop = []

    def key_pop(self):
        return self.id

    def __getitem__(self,i):
        return self.pop[i]

    def append(self, ind):
        self.pop.append(ind)

    def remove_pop(self, ind):
        self.pop.remove(ind)

    def deleteFrom(self, rank):
        del self.pop[rank:]

    def __len__(self):
        return len(self.pop)

    def __add__(self, right):
        p = Population()
        p.pop = self.pop + right.pop
        return p

    def sortAccordingToLevel(self):

        self.pop.sort(key=key_level)

    def sortAccordingToIndex(self):

        self.pop.sort(key=key_index)


class GeneticAlgorithm(object):
    def __init__(self, params):
        self.populations = []
        self.params = params

    def saveHTMLGeneration(self, gen, score, wdir):
        if wdir == None:
            return
        with HTML() as h:
            with h.body as b:
                b.h1("Population "+str(gen.id))
                b.p("Size:"+str(len(gen)))
                b.p("Score: "+str(score))
                for ind in gen.pop:
                    a = b.p("Individual "+str(ind.id)+". Score: "+str(ind.level)).a(ind.results["directory"], href=ind.results["directory"])

        f=open("generation_"+str(gen.id)+".html", "w")
        f.write(str(h))

    def saveHTMLIndex(self, num, wdir):
        if wdir == None:
            return
        with HTML() as h:
            with h.body as b:
                with b.center as c:
                    c.h1("ShapeGenerator run results")
                    c.hr()

                b.p("Started at: ")
                b.p("Terminated at: ")
                for i in range(num):
                    b.a("generation "+str(i), href="generation_"+str(i)+".html")
                    b.br()

        f=open("index.html", "w")
        f.write(str(h))


    def start(self,  numGen, generateFunc, mutationFunc, crossFunc, evalFunc, selectionFunc, wdir=None):
        if wdir != None:
            if not os.path.exists(wdir):
                os.makedirs(wdir)

        with inDirectory(wdir):

            ## Create the intial population.
            currgen =  generateFunc(0, self.params)

            ## Evaluate the score for each individual in the population.
            currscore = evalFunc(currgen)
            print("=================== GENERATION "+str(0)+"==================")
            print("size: "+str(len(currgen)))
            print("score: "+str(currscore))

            self.saveHTMLGeneration(currgen, currscore, wdir)
            for k in range(1, numGen):
                print("=================== GENERATION "+str(k)+"==================")
                ## Pour chaque génération.
                nextgen = crossFunc(currgen,self.params)
                nextgen = mutationFunc(nextgen, self.params)

                ### Gives score to each individual in the population
                evalFunc(nextgen)

                ### Replace the current generation with a new one.
                currgen = selectionFunc( (nextgen+currgen), self.params )
                currscore = 0
                for ind in currgen:
                    if currscore < ind.level:
                        currscore = ind.level

                print("size: "+str(len(currgen)))
                print("score: "+str(currscore))

                currgen.id = k
                self.saveHTMLGeneration(currgen, currscore, wdir)

            self.saveHTMLIndex(numGen, wdir)
            print("Experiment terminated.")
            print("Results can be found in "+wdir+"/index.html")

