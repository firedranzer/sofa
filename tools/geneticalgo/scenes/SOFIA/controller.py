

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
            self.node.getChild('accordion').getObject('ConstantForceField').findData('totalForce').value="0 0.40 0 "
            positions = self.node.getChild('accordion').getChild('markedPositions').getObject('markedPoints').findData('position').value[0]

            self.positionInit = (positions[0], positions[1])

        elif self.compteur==$nbIterations/3:
            positions = self.node.getChild('accordion').getChild('markedPositions').getObject('markedPoints').findData('position').value[0]
            print str(positions)
            self.VerticalGap1 = positions[1] - self.positionInit[1]
            self.node.getChild('accordion').getObject('ConstantForceField').findData('totalForce').value="0 0.0 0 "

        elif self.compteur==2*$nbIterations/3:
            self.node.getChild('accordion').getObject('ConstantForceField').findData('totalForce').value="0 -0.40 0 "

        elif self.compteur==$nbIterations:

            positions = self.node.getChild('accordion').getChild('markedPositions').getObject('markedPoints').findData('position').value[0]
            print str(positions)
            self.VerticalGap2 = positions[1] - self.positionInit[1]

            self.node.findData('animate').value=0
            dict = {"VerticalGap1":self.VerticalGap1, "VerticalGap2":self.VerticalGap2}
            emitJSONFragmentFromData(dict)
