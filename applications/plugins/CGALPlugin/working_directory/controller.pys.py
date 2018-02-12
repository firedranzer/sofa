import Sofa
import os
import customfield

def createScene(rootNode):

                rootNode.createObject('RequiredPlugin', pluginName='SoftRobots')
                rootNode.createObject('VisualStyle', displayFlags='showVisualModels hideBehaviorModels showCollisionModels hideBoundingCollisionModels hideForceFields showInteractionForceFields hideWireframe')

                rootNode.createObject('FreeMotionAnimationLoop')
                rootNode.createObject('GenericConstraintSolver', maxIterations='100', printLog='1', tolerance = '0.0000001')


                shape = rootNode.createChild("Shape")
                shape.createObject("EulerImplicitSolver", name="cg_odesolver",  printLog="0")
                shape.createObject("CGLinearSolver", template="GraphScattered", name="linear solver",  iterations="25",  tolerance="1e-09", threshold="1e-09")
                shape.createObject("SphericalField", name="sphere")
                shape.createObject("CustomField", name="field1", function="customfield.evalField")



                #Mesh component

                mesh=shape.createObject("MeshGenerationFromImplicitShape", name="mesh", facet_size="0.5", cell_size="0.8", scalarfield="@field1", drawTetras="true")

                print mesh.findData("outputTetras")

                #accordion"
                accordion = rootNode.createChild('accordion')
                accordion.createObject('EulerImplicit', name='odesolver')
                accordion.createObject('ShewchukPCGLinearSolver', iterations='15', name='linearsolver', tolerance='1e-5', preconditioners='preconditioner', use_precond='true', update_step='1')
                accordion.createObject('TetrahedronSetTopologyContainer', position='@../Shape/mesh.outputPoints', tetrahedra='@../Shape/mesh.outputTetras', name='container')
                accordion.createObject('TetrahedronSetTopologyModifier')
                accordion.createObject('TetrahedronSetTopologyAlgorithms', template='Vec3d')
                accordion.createObject('TetrahedronSetGeometryAlgorithms', template='Vec3d')
                
                accordion.createObject('MechanicalObject', name='tetras', src="@container", template='Vec3d', showIndices='false', showIndicesScale='4e-5', rx='0', dz='0')
                accordion.createObject('UniformMass', totalmass='0.5')
                accordion.createObject('TetrahedronFEMForceField', template='Vec3d', name='FEM', method='large', poissonRatio='0.3',  youngModulus='18000')
		  
                accordion.createObject('BoxROI', name='boxROI', box='-10 -10 0  10 10 0.5', drawBoxes='true', position="@tetras.rest_position", tetrahedra="@container.tetrahedra")
                accordion.createObject('RestShapeSpringsForceField', points='@boxROI.indices', stiffness='1e12')
                
                accordion.createObject('SparseLDLSolver', name='preconditioner')
                accordion.createObject('LinearSolverConstraintCorrection', solverName='preconditioner')
                #accordion.createObject('UncoupledConstraintCorrection')


                #accordion/cavity
                cavity = accordion.createChild('cavity')
                cavity.createObject('MeshSTLLoader', name='loader', filename='cylinder.stl')
                cavity.createObject('Mesh', src='@loader', name='topo')
                cavity.createObject('MechanicalObject', name='cavity')
                cavity.createObject('SurfacePressureConstraint', triangles='@topo.triangles', value='30', valueType="1")
                cavity.createObject('BarycentricMapping', name='mapping',  mapForces='false', mapMasses='false')
                
                
                #accordion/accordionVisu
                accordionVisu = accordion.createChild('visu')
                accordionVisu.createObject('TriangleSetTopologyContainer', name='container')
                accordionVisu.createObject('TriangleSetTopologyModifier')
                accordionVisu.createObject('TriangleSetTopologyAlgorithms', template='Vec3d')
                accordionVisu.createObject('TriangleSetGeometryAlgorithms', template='Vec3d')
                accordionVisu.createObject('Tetra2TriangleTopologicalMapping', name='Mapping', input="@../container", output="@container")
                
                accordionVisu.createObject('OglModel', template='ExtVec3f', color='0.3 0.2 0.2 0.6')
                accordionVisu.createObject('IdentityMapping')


                return rootNode
