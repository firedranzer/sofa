= Examples

== CustomField.scn :
   Demonstrate the use of the CustomField component in which a python function is providing the
   field value and gradient at given point in space.

== cython/CustomField.scn :
   Python is a really slow langage. When an application requires to evaluate the field value at
   at multiple point the performances will decrease. To accelerate the python calculus it is
   possible to have cython to generate machine code.
