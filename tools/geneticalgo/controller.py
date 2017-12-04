

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
            self.node.getChild('accordion').getObject('ConstantForceField').findData('totalForce').value=-"-0.40 0.40 0 "
            positions = self.node.getChild('accordion').getChild('markedPositions').getObject('markedPoints').findData('position').value
            self.positionInit =(positions[0], positions[1])

        elif self.compteur==90/3:
            positions = self.node.getChild('accordion').getChild('markedPositions').getObject('markedPoints').findData('position').value
	    print str(positions)
            self.HorizontalGap = positions[0] - self.positionInit[0]
	    self.node.getChild('accordion').getObject('ConstantForceField').findData('totalForce').value="0 0.0 0 "

	elif self.compteur==2*90/3:
	    self.node.getChild('accordion').getObject('ConstantForceField').findData('totalForce').value="0 -0.20 0 "
            positions = self.node.getChild('accordion').getChild('markedPositions').getObject('markedPoints').findData('position').value
            print str(positions)
            self.Zmax = positions[1][2] - positions[0][2]

	elif self.compteur==90:
	    
            positions = self.node.getChild('accordion').getChild('markedPositions').getObject('markedPoints').findData('position').value
            print str(positions)
            self.VerticalGap = positions[1] - positionsInit[1]

            self.node.findData('animate').value=0
            dict = {"HorizontalGap":self.HorizontalGap, "VerticalGap":self.VerticalGap}
            emitJSONFragmentFromData(dict)
