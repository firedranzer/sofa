

#!/usr/bin/env python
# -*- coding: utf-8 -*-
import Sofa
import math
import json

def emitJSONFragmentFromData(dict):
    print("JSON : "+json.dumps(dict)+"\n")

class controller(Sofa.PythonScriptController):
    def initGraph(self, node):
        self.node=node
        self.compteur=0

    def onBeginAnimationStep(self,dt):
        print "onEndAnimationStep"
        self.compteur+=1

        if self.compteur==1:
            positions = self.node.getChild('accordion').getChild('markedPositions').getObject('markedPoints').findData('position').value
            self.Z0 = positions[1][2] - positions[0][2]
            self.V0 = self.node.getChild('accordion').getChild('cavity').getObject('pressure').findData('cavityVolume').value

        if self.compteur==$nbIterations:
            positions = self.node.getChild('accordion').getChild('markedPositions').getObject('markedPoints').findData('position').value
            print str(positions)
            self.Zmax = positions[1][2] - positions[0][2]
            self.Vmax = self.node.getChild('accordion').getChild('cavity').getObject('pressure').findData('cavityVolume').value
            dict = {"Z0":self.Z0, "Zmax":self.Zmax, "V0":self.V0, "Vmax":self.Vmax}
            emitJSONFragmentFromData(dict)
            self.node.findData('animate').value=0

