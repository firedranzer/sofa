#!/usr/bin/env python
# -*- coding: utf-8 -*-
import Sofa
import math


class controller(Sofa.PythonScriptController):
    def initGraph(self, node):
        self.node=node
        self.compteur=0
        self.number_of_dt=10
        self.node.findData('animate').value=1

    def onEndAnimationStep(self,dt):
        self.compteur+=1
        if self.compteur==1:
            values=self.node.getChild('accordion').getObject('tetras').findData('free_position').value
            Z=[]
            for point in values:
                Z.append(point[2])
            self.Z0=max(Z)-min(Z)

        if self.compteur==self.number_of_dt:
            values=self.node.getChild('accordion').getObject('tetras').findData('free_position').value
            Z=[]
            for point in values:
                Z.append(point[2])
            self.Zmax=max(Z)-min(Z)
            self.node.findData('animate').value=0
            print "Z0,Zmax="+str(self.Z0)+", "+str(self.Zmax)
            return self.Z0, self.Zmax

