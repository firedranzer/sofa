#!/usr/bin/python
# -*- coding: utf-8 -*-
#/******************************************************************************
#*       SOFA, Simulation Open-Framework Architecture, development version     *
#*                (c) 2006-2015 INRIA, USTL, UJF, CNRS, MGH                    *
#*                                                                             *
#* This library is free software; you can redistribute it and/or modify it     *
#* under the terms of the GNU Lesser General Public License as published by    *
#* the Free Software Foundation; either version 2.1 of the License, or (at     *
#* your option) any later version.                                             *
#*                                                                             *
#* This library is distributed in the hope that it will be useful, but WITHOUT *
#* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
#* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
#* for more details.                                                           *
#*                                                                             *
#* You should have received a copy of the GNU Lesser General Public License    *
#* along with this library; if not, write to the Free Software Foundation,     *
#* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
#*******************************************************************************
#*                              SOFA :: Framework                              *
#*                                                                             *
#* Contact information: contact@sofa-framework.org                             *
#******************************************************************************/
#*******************************************************************************
#* Contributors:                                                               *
#*    - damien.marchal@univ-lille1.fr Copyright (C) CNRS                       *
#*                                                                             *
#******************************************************************************/

import Sofa
import SofaPython
import difflib
import os

# TODO(dmarchal 2017-06-17) Get rid of these ugly globals.
templates = {}
aliases = {}
sofaAliases = {}
sofaComponents = []
SofaStackFrame = []
datafieldQuirks = []
sofaRoot = None
imports = {}

def refreshComponentListFromFactory():
    global sofaComponents, sofaAliases
    sofaComponents = []
    sofaAliases = {}
    for (name, desc) in Sofa.getAvailableComponents():
            sofaComponents.append(name)
            for alias in Sofa.getAliasesFor(name):
                sofaAliases[alias] = name

def srange(b, e):
        s=""
        for i in range(b,e):
                s+=str(i)+" "
        return s

def flattenStackFrame(sf):
        """Return the stack frame content into a single "flat" dictionnary.
           The most recent entries are overriden the oldest.
           """
        res = {}
        for frame in sf:
                for k in frame:
                        res[k] = frame[k]
        return res

def getFromStack(name, stack):
        """Search in the stack for a given name. The search is proceeding from
           the most recent entries to the oldest. If 'name' cannot be found
           in the stack None is returned. """
        for frame in reversed(stack):
                for k in frame:
                        if k == name:
                                return frame[k]
        return None

def populateFrame(cname, frame, stack):

        """Initialize a frame from the current attributes of the 'self' object
           This is needed to expose the data as first class object.
        """
        fself = getFromStack("self", stack)
        if fself == None:
                return

def processPython(parent, key, kv, stack, frame):
        """Process a python fragment of code with context provided by the content of the stack."""
        context = flattenStackFrame(stack)
        local = {}
        exec(kv, context, local)

        ## Transfer the local entries to the previously defined context
        ## This allow import in one line and use the imported in the following ones.
        for k in local:
                stack[-1][k] = local[k]

def evalPython(key, kv, stack, frame):
        """Process a python fragment of code with context provided by the content of the stack."""
        r = flattenStackFrame(stack)
        retval = eval(kv, r)
        return retval

def getField(object, name):
    d = object.getLink(name)
    if d != None:
        return d

    d = object.getData(name)
    if d != None:
        return d

    return None

pslprefix = "PSL::engine"

def processString(object, name, value, stack, frame):
    ## Python Hook to build an eval function.
    if len(value) > 2 and value[0] == 'p' and value[1] == '"':
            d = object.addData("psl_"+name, "PSL", "This hold a python expression.", "s", str(value))
            object.findData("psl_"+name).setPersistant(True)
            return evalPython(None, value[2:-1], stack, frame)

    return value

def processParameter(parent, name, value, stack, frame):
        try:
            if isinstance(value, list):
                    matches = difflib.get_close_matches(name, sofaComponents+templates.keys()+sofaAliases.keys(), n=4)
                    c=parent.createChild("[XX"+name+"XX]")
                    Sofa.msg_error(c, pslprefix+" unknow parameter or component [" + name + "] suggestions -> "+str(matches))
            elif not name in datafieldQuirks:
                    ## Python Hook to build an eval function.
                    if len(value) > 2 and value[0] == 'p' and value[1] == '"':
                            value = evalPython(None, value[2:-1], stack, frame)

                    try:
                            field = getField(frame["self"], name)
                            if field != None:
                                field.setValueString(str(value))
                                field.setPersistant(True)
                            else:
                                Sofa.msg_error(parent, pslprefix+" unable to get the field '" +name+"'")

                    except Exception,e:
                            Sofa.msg_error(parent, pslprefix+" exception while parsing field '" +name+"' because "+str(e))

                    if name == "name":
                            frame[value] = frame["self"]
                            #frame["name"] = value

        except Exception, e:
            SofaPython.sendMessageFromException(e)
            Sofa.msg_error(parent, pslprefix+" unable to parse parameter '"+str(name)+ "=" + str(value)+"'")

def createObject(parentNode, name, stack , frame, kv):
        if name in sofaComponents:
                obj = parentNode.createObject(name, **kv)
                for k in kv:
                    if getField(obj, k) == None:
                        Sofa.msg_info(obj, pslprefix+" attribute '"+str(k)+"' is a parsing hook. Let's add Data field to fix it. To remove this warning stop using parsing hook.")
                        d = obj.addData(k, "PSL", "", "s", str(kv[k]))
                        obj.findData(k).setPersistant(True)
                return obj

        kv["name"] = name
        failureObject = parentNode.createObject("Undefined", **kv)

        Sofa.msg_error(failureObject, pslprefix+" unable to create the object '"+str(name)+"'")
        return failureObject

def processObjectDict(obj, dic, stack, frame):
        for key,value in dic:
                if key == "Python":
                        processPython(obj, key, value, stack, frame)
                else:
                        processParameter(obj, key, value, stack ,frame)

def processObject(parent, key, kv, stack, frame):
    try:
        global sofaComponents
        populateFrame(key, frame, stack)
        frame = {}
        kwargs = {}
        if not isinstance(kv, list):
                kv = [("name" , kv)]

        for k,v in kv:
                if len(v) != 0 and v[0] == 'p' and v[1] == '"':
                        v = evalPython(None, v[2:-1], stack, frame)

                #if k == "name":
                #        frame["name"] = v

                kwargs[k] = str(v)

        stack.append(frame)
        frame["self"] = obj = createObject(parent, key, stack, frame, kwargs)

        ### Force all the data field into a non-persistant state.
        for datafield in obj.getListOfDataFields():
            datafield.setPersistant(False)

        for link in obj.getListOfLinks():
            link.setPersistant(False)

        ### Then revert only the ones that have been touched
        for dataname in kwargs:
            try:
                if dataname in datafieldQuirks:
                    continue

                field = getField(obj, dataname)
                if field != None:
                    field.setPersistant(True)

            except Exception,e:
                Sofa.msg_warning(obj, pslprefix+" this does not seems to be a valid field '"+str(dataname)+"'")

        if not "name" in kwargs:
            obj.findData("name").unset()

        stack.pop(-1)

        if key == "RequiredPlugin" :
                refreshComponentListFromFactory()

        return obj
    except Exception, e:
        c=parent.createChild("[XX"+key+"XX]")
        Sofa.msg_error(c, pslprefix+" unable to create an object because: "+str(e.message))

# TODO add a warning to indicate that a template is loaded twice.
def importTemplates(content):
        templates = {}
        for key, value in content:
                if key == "Template":
                        name = "undefined"
                        properties = {}
                        rvalue = []
                        for k,v in value:
                                if k == "name":
                                        name = str(v)
                                elif k == "properties":
                                        properties = v
                                else:
                                        rvalue.append((k, v))
                        templates[name] = {"properties":properties, "content" : rvalue}
                else:
                        Sofa.msg_warning(pslprefix, " an imported file contains something that is not a Template.")

        return templates

# TODO gérer les imports circulaires...
def processImport(parent, key, kv, stack, frame):
        global imports, templates
        if not (isinstance(kv, str) or isinstance(kv, unicode)):
                Sofa.msg_error(parent, pslprefix+" expecting a single 'string' entry....in procesImport " + str(type(kv)))
                return
        filename = kv+".psl"
        if not os.path.exists(filename):
                dircontent = os.listdir(os.getcwd())
                matches = difflib.get_close_matches(filename, dircontent, n=4)
                Sofa.msg_error(parent, pslprefix+" the file '" + filename + "' does not exists. Do you mean: "+str(matches))
                return
        Sofa.msg_info(parent, "importing "+ os.getcwd() + "/"+filename)

        f = open(filename).read()
        loadedcontent = hjson.loads(f, object_pairs_hook=MyObjectHook())
        imports[filename] = importTemplates(loadedcontent)

        for tname in imports[filename].keys():
                templates[kv+"."+tname] = imports[filename][tname]

def processTemplate(parent, key, kv, stack, frame):
        global templates
        name = "undefined"
        properties = {}
        pattern = []
        for key,value in kv:
                if key == "name":
                        name = value
                elif key == "properties":
                        properties = value
                else:
                        pattern.append( (key, value) )
        o = parent.createObject("Template", name=str(name))
        o.listening = True
        o.setTemplate(kv)
        o.trackData(o.findData("psl_source"))
        o.addData("psl_instanceof", "PSL", "", "s", "Template")
        frame[str(name)] = o
        templates[str(name)] = o
        return o

def processAlias(parent, key, kv, stack, frame):
        global aliases
        oldName, newName = kv.split('-')
        aliases[newName]=oldName

def reinstanciateTemplateOnSourceChange(templateSource):
    global templates

    key = templateSource.name
    frame = {}
    frame["parent"]=templateSource
    frame["self"]=templateSource
    nframe = {}
    instanceProperties = eval(templateInstance.psl_source)

    for c in templateInstance.getChildren():
            templateInstance.removeChild(c)

    c = templateInstance.getObjects()
    for o in c:
            templateInstance.removeObject(o)

    ## Is there a template with this name, if this is the case
    ## Retrieve the associated templates .
    if isinstance(templates[key], Sofa.Template):
            n = templates[key].getTemplate()
            for k,v in n:
                    if k == 'name':
                            None
                    elif k == 'properties':
                            for kk,vv in v:
                                    if not kk in frame:
                                            nframe[kk] = vv
                    else:
                            source = v
    else:
            source = templates[key]["content"]
            for k,v in templates[key]["properties"]:
                    if not k in frame:
                            nframe[k] = str(v)

    for k,v in instanceProperties:
            nframe[k] = templateInstance.findData(str(k)).getValue(0)

    stack = [globals(), frame]
    n = processNode(templateInstance, "Node", source, stack, nframe, doCreate=False)

def gatherInstances(templatename, node):
    r = []
    for child in node.getChildren():
        instanceof = child.getData("psl_instanceof")
        if instanceof != None and str(instanceof) == templatename:
            r.append(child)
        else:
            r = r + gatherInstances(templatename, child)
    return r

def reinstanciateAllTemplates(template):
    Sofa.msg_info(template, "Re-instanciating instance of: "+template.name)
    root = template.getContext().getRoot()
    g = gatherInstances(template.name, root)

    for node in g:
        frame = {}
        frame["parent"]=node.getParents()
        frame["self"]=node
        instanceProperties = eval(node.psl_properties)

        for c in node.getChildren():
            node.removeChild(c)

        for o in node.getObjects():
            node.removeObject(o)

        src = eval(template.psl_source)
        res = []
        for k,v in src:
            if k == "properties":
                for kk, vv in v:
                    frame[kk] = vv
            elif k == "name":
                None
            elif k == "Node":
                res = v
        src=res
        for k,v in instanceProperties:
            frame[k] = v

        print("PROCESSING CODE: "+str(src)+ " WITH FRAME: "+str(frame))
        n = processNode(node, "Node", src, [frame], frame, doCreate=False)


def reinstanciateTemplate(templateInstance):
        global templates

        key = templateInstance.name
        frame = {}
        frame["parent"]=templateInstance
        frame["self"]=templateInstance
        nframe = {}
        instanceProperties = eval(templateInstance.psl_source)

        for c in templateInstance.getChildren():
                templateInstance.removeChild(c)

        c = templateInstance.getObjects()
        for o in c:
                templateInstance.removeObject(o)

        ## Is there a template with this name, if this is the case
        ## Retrieve the associated templates .
        if isinstance(templates[key], Sofa.Template):
                n = templates[key].getTemplate()
                for k,v in n:
                        if k == 'name':
                                None
                        elif k == 'properties':
                                for kk,vv in v:
                                        if not kk in frame:
                                                nframe[kk] = vv
                        else:
                                source = v
        else:
                source = templates[key]["content"]
                for k,v in templates[key]["properties"]:
                        if not k in frame:
                                nframe[k] = str(v)

        for k,v in instanceProperties:
                nframe[k] = templateInstance.findData(str(k)).getValue(0)

        stack = [globals(), frame]
        n = processNode(templateInstance, "Node", source, stack, nframe, doCreate=False)

def processProperties(self, key, kv, stack, frame):
    if not isinstance(kv, list):
        raise Exception("This shouldn't happen, exepecting only list")

    msg = ""
    for k,v in kv:
        ## Check if the property named by "k" already exists.
        if hasattr(self, k):
            msg += " - cannot add a property named '"+k+"' as it already exists"
            continue

        if isinstance(v, unicode):
            v=str(v)

        if isinstance(v, str):
            v=processString(self, k, v, stack, frame)

        if isinstance(v, int):
            self.addData(k, "Properties", "", "d", v)
        elif isinstance(v, str) or isinstance(v,unicode):
            self.addData(k, "Properties", "", "s", str(v))
        elif isinstance(v, float):
            self.addData(k, "Properties", "", "f", v)

        if hasattr(self, k):
            msg += " - adding: '"+str(k)+"' = "+str(v)
        else:
            msg += " - unable to create a property from the value '"+str(v)+"'"

    Sofa.msg_info(self, pslprefix+"Adding a user properties: \n"+msg)


def instanciateTemplate(parent, key, kv, stack, frame):
        global templates
        stack.append(frame)
        nframe={}
        source = None
        if isinstance(templates[key], Sofa.Template):
                n = templates[key].getTemplate()
                for k,v in n:
                        if k == 'name':
                            None
                        elif k == 'properties':
                                for kk,vv in v:
                                        if not kk in frame:
                                                nframe[kk] = vv
                        else:
                                source = v
        else:
                source = templates[key]["content"]
                for k,v in templates[key]["properties"]:
                        if not k in frame:
                                nframe[k] = str(v)

        for k,v in kv:
                nframe[k] = v

        n = processNode(parent, "Node", source, stack, nframe, doCreate=True)

        if isinstance(templates[key], Sofa.Template):
                for k,v in kv:
                        if not hasattr(n, k):
                                if isinstance(v, int):
                                        n.addData(k, key+".Properties", "Help", "d", v)
                                elif isinstance(v, str) or isinstance(v,unicode):
                                        n.addData(k, key+".Properties", "Help", "s", str(v))
                                elif isinstance(v, float):
                                        n.addData(k, key+".Properties", "Help", "f", v)
                                elif isinstance(v, unicode):
                                        n.addData(k, key+".Properties", "Help", "f", str(v))

                n.addData("psl_properties", "PSL", "Captured variables for template re-instantiation", "s", repr(kv))
                n.addData("psl_instanceof", "PSL", "Type of the object", "s", key)
        stack.pop(-1)

def processNode(parent, key, kv, stack, frame, doCreate=True):
        global templates, aliases
        stack.append(frame)
        populateFrame(key, frame, stack)

        if doCreate:
                if parent == None:
                    tself = Sofa.createNode("undefined")
                else:
                    tself = parent.createChild("undefined")

                frame["self"] = tself

                ### Force all the data field into a non-persistant state.
                for datafield in tself.getListOfDataFields():
                    datafield.setPersistant(False)

                for link in tself.getListOfLinks():
                    link.setPersistant(False)
        else:
                tself = frame["self"] = parent

        print("HERE... "+str(kv))

        if isinstance(kv, list):
                for key,value in kv:
                        print("PROCESS: "+key)

                        sofaAliasInitialName = None
                        if isinstance(key, unicode):
                                key = str(key)

                        if key in sofaAliases:
                                sofaAliasInitialName = key
                                key = sofaAliases[key]

                        if key in aliases:
                                key = aliases[key]

                        if key == "Import":
                                n = processImport(tself, key, value, stack, {})
                        elif key == "Node":
                                n = processNode(tself, key, value, stack, {})
                        elif key == "Python":
                                processPython(tself, key, value, stack, {})
                        elif key == "properties":
                                processProperties(tself, key, value, stack, {})
                        elif key == "Template":
                                tself.addObject( processTemplate(tself, key, value, stack, {}) )
                        elif key == "Using":
                                processAlias(tself, key,value, stack, frame)
                        elif key in sofaComponents:
                                o = processObject(tself, key, value, stack, {})
                                if o != None:
                                        if isinstance(sofaAliasInitialName, str):
                                            Sofa.msg_warning(o, pslprefix+"'"+key+" was created using the hard coded alias '"+str(sofaAliasInitialName)+"'"+".  \nUsing hard coded aliases is a confusing practice and we advise you to use scene specific alias with the Alias keyword.")
                        elif key in templates:
                                instanciateTemplate(tself, key,value, stack, frame)
                        else:
                                ## we are on a cache hit...so we refresh the list.
                                refreshComponentListFromFactory()

                                if key in sofaComponents:
                                        o = processObject(tself, key, value, stack, {})
                                        if o != None:
                                                tself.addObject(o)
                                processParameter(tself, key, value, stack, frame)
        else:
                raise Exception("This shouldn't happen, expecting only list")
        stack.pop(-1)
        return tself

def processRootNode(kv, stack, frame):
        global templates, aliases
        stack.append(frame)
        populateFrame("", frame, stack)

        if isinstance(kv, list):
                for key,value in kv:
                        if isinstance(key, unicode):
                                key = str(key)

                        if key == "Node":
                                n = processNode(None, key, value, stack, {})
                                return n
                        else:
                                Sofa.msg_error(tself, pslprefix+"Unable to find a root Node in this file")
                                return None

        Sofa.msg_error(tself, pslprefix+"Unable to find a root Node in this file")
        return None

## Root function that process an abstract tree.
def processTree(ast, directives, globalenv):
        refreshComponentListFromFactory()

        if directives["version"] == "1.0":
            r = processRootNode(ast, [], globalenv)
            return r

        ## Add here the future version of the language

