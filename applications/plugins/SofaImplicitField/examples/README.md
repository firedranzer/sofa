= Examples

== CustomField_python.scn :
   Demonstrate the use of the CustomField component which allow to implement the field
   using python. The view of the geometry is done using the 
   PointCloudImplicitFieldVisualization component.
   
== CustomField_cython.scn :
   Python is a really slow langage. A good alternative is use "cython". This is what
   is done in this example where the machine code is generated "on the fly" from
   the cython file resutling in a large speed up on the distance field evaluation. 
   This example requires cython to be installed and is not expected to work on windows. 
   The visualization of the field is done using the PointCloudImplicitFieldVisualization 
   component.

== CustomField_rawcython.scn :
   This one is really experimental as the code compiled on the fly is now executed out of
   the python GIL and thus the CustomField using it is not bottlenecked by python which 
   is really usefull when the field is evaluated by multiple thread. 
   
== CustomFieldAllInOne.scn :
   In this example we are rendering the same field using three components:
   - the PointCloudImplicitFieldVisualisation, 
   - the SurfaceMeshGenerationFromScalarField (implementing a marching cube)
   - the ImplicitFieldShaderVisualization which convert the shape describing the geometry into 
     a glsl shader that is injected into a raymarching code. The ray marching shader is interesting
     as it blend the image with the other OpenGL content. 