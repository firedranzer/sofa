import Sofa
import pprint
PSLExport = []

def Node(parent, **kwargs):
    """Create a node and attach it to the provided graph"""
    name = "undefined"
    if kwargs == None:
            kwargs = {}
    elif "name" in kwargs:
        name = kwargs["name"]
        del kwargs["name"]

    child = parent.createChild(name)
    for k in kwargs:
        data = child.getData(k)
        if data != None:
            data.value = kwargs[k]
    return child

class psltemplate(object):
   def __init__(self, f):
       self.function = f
       if not "PSLExport" in f.func_globals:
           f.func_globals["PSLExport"] = []
       f.func_globals["PSLExport"].append(self.function.func_name)

   def __call__(self, node, **kwargs):
       if not "name" in kwargs:
           kwargs["name"] = "undefined"
       templatenode = node.createChild(kwargs["name"])
       self.function(templatenode, **kwargs)
       calledAs = self.function.func_name+"("
       for k in kwargs:
           calledAs += str(k)+"="+repr(kwargs[k])
       calledAs += ")"
       #templatenode.addData("psl_instanceof", "PSL", "", "s", "PythonDecoratorTemplate")
       #templatenode.addData("psl_templatename", "PSL", "", "s", calledAs)
       return templatenode

for (name,desc) in Sofa.getAvailableComponents():
    code = """def %s(owner, **kwargs):
        \"\"\"%s\"\"\"
        if kwargs == None:
                kwargs = {}
        owner.createObject(\"%s\", **kwargs)
""" % (name,desc,name)
    exec(code)

