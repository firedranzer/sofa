### Import a python implementation of the geometry.
### The advantage of this representation is that it is super fast.
import math	

### The function that is called by sofa to get the field value.
def evalField(x,y,z):
    cx = x - 0.5
    cy = y - 0.5
    cz = z - 0.5
    return math.sqrt( (cx*cx + cy*cy + cz*cz) ) - 0.5

