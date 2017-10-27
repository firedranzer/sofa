#compiler-settings
#commentStartToken = //
#end compiler-settings

### SCENE FILE AUTOMATICALLY GENERATED.
###
### $GENERATION
###

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
            values=self.node.getChild('accordion').getObject('tetras').findData('position').value
            Z=[]
            for point in values:
                Z.append(point[2])
            self.Z0 = max(Z)-min(Z)
            self.V0 = self.node.getChild('accordion').getChild('cavity').getObject('pressure').findData('cavityVolume').value

        if self.compteur==$nbIterations:
            values=self.node.getChild('accordion').getObject('tetras').findData('position').value
            Z=[]
            for point in values:
                Z.append(point[2])
            self.Zmax=max(Z)-min(Z)
            self.Vmax = self.node.getChild('accordion').getChild('cavity').getObject('pressure').findData('cavityVolume').value
            dict = {"Z0":self.Z0, "Zmax":self.Zmax, "V0":self.V0, "Vmax":self.Vmax}
            emitJSONFragmentFromData(dict)
            self.node.findData('animate').value=0

