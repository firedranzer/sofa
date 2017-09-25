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








#    def resetGraph(self, node):
#        outputVector = [245, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.]
#        self.node.getObject('serial').findData('sentData').value = outputVector

#    def onKeyPressed(self,c):
#        inputvalue = self.node.getObject('aCable').findData('value')
#        print 'key pressed: '+str(inputvalue)


#    def onEndAnimationStep(self,dt):

#        displacement=[ 0., 0., 0., 0., 0., 0., 0., 0., 0.]
#        pressure=[ 0., 0., 0., 0., 0., 0., 0., 0., 0.]
#        outputVector = [245, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

#        for cableId in range(7,10):
#          cable = self.node.getChild('complianceSection').getChild('cables').getObject('c'+str(cableId))
#          displacement[cableId-1] = cable.findData('cableLength').value
#          outputVector[cableId] = int(displacement[cableId-1])

#        for sectionId in range(1,4):
#          for cavityId in range(1,4):
#            cavity = self.node.getChild('complianceSection').getChild('MappedCavity'+str(sectionId)+str(cavityId)).getObject('SurfacePressureConstraint')
#            pressure[(sectionId-1)*3+(cavityId-1)] = cavity.findData('value').value[0][0]
#            print 'pressure: '+ str( pressure[(sectionId-1)*3+(cavityId-1)])


#            outputVector[10+(sectionId-1)*3+(cavityId-1)] = int(pressure[(sectionId-1)*3+(cavityId-1)])

#        self.node.getObject('serial').findData('sentData').value = outputVector
#        print(outputVector)


#    #def onBeginAnimationStep(self,dt):

#        #############################################################
#        ####Translate a bit the goal for a more friendly interaction
#        #############################################################
#        #meca1 = self.node.getChild('goal').getChild('goalPoints').getObject('goalMORigide')
#        #meca2 = self.node.getChild('goal').getChild('goalPoints').getObject('goalMORigideTranslated')

#        #position = meca1.findData('position').value[0]
#        #meca2.findData('position').value = str(position[0])+" "+str(position[1])+" "+str(position[2]-10.)




