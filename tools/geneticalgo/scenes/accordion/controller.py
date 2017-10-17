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
            values=self.node.getChild('accordion').getObject('tetras').findData('position').value
            Z=[]
            for point in values:
                Z.append(point[2])
            self.Z0 = max(Z)-min(Z)
            self.V0 = self.node.getChild('accordion').getChild('cavity').getObject('pressure').findData('cavityVolume').value

        if self.compteur==self.number_of_dt:
            values=self.node.getChild('accordion').getObject('tetras').findData('position').value
            Z=[]
            for point in values:
                Z.append(point[2])
            self.Zmax=max(Z)-min(Z)
            self.Vmax = self.node.getChild('accordion').getChild('cavity').getObject('pressure').findData('cavityVolume').value
            self.node.findData('animate').value=0
            print "Z0,Zmax, V0, Vmax="+str(self.Z0)+", "+str(self.Zmax)+" , "+str(self.V0)+" , "+str(self.Vmax)
            return self.Z0, self.Zmax

