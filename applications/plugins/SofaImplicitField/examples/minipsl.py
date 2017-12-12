import pyximport; pyximport.install()
import dfgeom
from dfgeom import *
import minipsl
import hjson

class MyObjectHook(object):
    def __call__(self, s):
        return s

def toDict(s):
    q = {}
    for k,v in s:
        q[k] = v
    return q

def processShapeAst(parentNode, s):
    print("PARSING: "+str(s))
    k,v = s
    if k == "ImplicitShape":
        node = parentNode.createChild("ImplicitShape")
        for d in v:
            n,vv = d
            if n == "name":
                node.name = "ImplicitShape(" + vv+ ")"
            else:
                return processShapeAst(node, d)
    elif k == "Difference":
            node = parentNode.createChild("Difference")
            l=[]
            for d in v:
                l.append(processShapeAst(node, d))
            print("RES: "+str(l))
            return Difference(l[0], l[1])
    elif k == "Union":
            node = parentNode.createChild("Union")
            l=[]
            for d in v:
                l.append(processShapeAst(node, d))
            return Union(l[0],l[1])
    elif k == "Sphere":
            node = parentNode.createChild("Sphere")
            sphere = Sphere(**toDict(v))
            return sphere
    return None

def parse(root, s):
    r = hjson.loads(s,  object_pairs_hook=MyObjectHook())
    print("PARSE: "+str(r[0]))
    return processShapeAst(root, r[0])
