#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import json
import shutil
from Cheetah.Template import Template


emptypackage = {
        "package_name" : "undefined",
        "package_type" : "undefined",
        "dependencies" : [],
        "header_files" : [],
        "source_files" : [],
        "extra_files" : []
}

def toLine(l):
    s=""
    for i in l:
        s+=" "+i
    return s
    
def generateCMakeLists(package_name, package_dir=None, cmake_template="BaseCMakeLists.txt.tpl", **kwargs):
    if package_dir == None:
        package_dir = package_name
    print("   - generating "+package_dir+"/CMakeLists.txt")
 
    theFile = open(package_dir+"/CMakeLists.txt", "w+")
    pck = loadPackage(package_name, package_dir=package_dir)
    pck["sorted_dependencies"] = toLine( pck["dependencies"] )
    
    templatebasepath = os.path.dirname(os.path.abspath(__file__))+"/templates"
    templatelocation = templatebasepath+"/"+cmake_template
    if os.path.exists(cmake_template):
        templatelocation = cmake_template 
    elif os.path.exists(templatelocation):
        pass
    else:
        print(" - failed to find a valid template in")
        print("   search for: "+cmake_template)
        print("   search for: "+templatelocation)
        return 
            
    if not os.path.exists(templatelocation):
        print(" - failed to find a valid template in "+templatelocation)
        return 
    
    t = Template( open(templatelocation).read(), searchList=[pck, {"cmake_template_src_path" : templatelocation}])
    theFile.write(str(t))


def loadSpm(spmfilename):
    return json.loads( open(spmfilename).read() ) 

def addPackageGroup(name):
    print("Adding a package group: "+name)

def savePackage(package_name, package, package_dir=None, **kwargs):
    if package_dir == None:
        package_dir = package_name
    spmdir = package_dir+"/"+".spm"
    spmfile = spmdir+"/config.json"  
    for k in package:
        if isinstance( package[k], list ):
            package[k].sort()    
    open(spmfile, "w").write( json.dumps( package ) )      
      

def loadPackage(package_name, package_dir=None, **kwargs):
    if package_dir == None:
        package_dir = package_name 
    spmdir = package_dir+"/"+".spm"
    spmfile = spmdir+"/config.json"      
    if os.path.exists(spmfile):
        spm = loadSpm(spmfile)
        spm["package_cname"] = spm["package_name"].replace(".","_").upper()
        return spm
    print("unable to load spm file from " + package_dir)
    return None

def addToProperty(property_name, property_value, package_name, **kwargs):
    print("   - adding to property '"+property_name+"' value: "+str(property_value))
    package = loadPackage(package_name, **kwargs)
        
    if property_name not in package:
        package[property_name] = []
    
    if isinstance(property_value, list):
        package[property_name] += property_value
    else:
        package[property_name].append(property_value)
    savePackage(package_name, package,**kwargs)   

def setProperty(package_name, property_name, property_value, **kwargs):
    print("   - setting property '"+property_name+"' to package '"+package_name+"'")
    package = loadPackage(package_name, **kwargs)
    package[property_name] = property_value
    savePackage(package_name, package, **kwargs)    
   

def addProperty(property_name, property_value, package_name, **kwargs):
    print("   - adding property '"+property_name+"' to package '"+package_name+"'")
    package = loadPackage(package_name, **kwargs)
    if property_name not in package:
        package[property_name] = property_value
    else:
        print("  failure: property '"+property_name+"' alreay exists in package "+package_name)
    savePackage(package_name, package, **kwargs)    

def initPackage(package_name, package_dir, package_type="library", **kwargs):
    finalpath = package_dir 
    print("   - init package in "+finalpath)
    print("    - name : '"+package_name+"'")
    print("    - path: '"+finalpath+"'") 
    parentpath=""
    pathdec = package_name.split('/')
    for p in pathdec[:-1]:
        parentpath+=p+"/"
        if os.path.exists(parentpath):
            print("    - path exist:"+parentpath) 
        else:
            groupname = parentpath.replace('/', '.')
            print("    - create group: "+groupname)
            #spmdir = parentpath+"/"+".spm"
            #spmfile = spmdir+"/config.json"            
            os.mkdir(parentpath)
            #os.mkdir(spmdir)
            #open(spmfile, "w").write( json.dumps( emptypackage ) )
            #setProperty(groupname, "package_name", groupname)
            #setProperty(groupname, "package_type", "group")
            
    if pathdec[-1]:       
            
                if not os.path.exists(finalpath):
                    print("    - create dir")
                    os.mkdir(finalpath)
                spmdir = finalpath+"/"+".spm"
                if not os.path.exists(spmdir):
                    print("    - create spm dir")
                    os.mkdir(spmdir)
                spmfile = spmdir+"/config.json"                      
                print("   - save package description in: "+spmfile)
                open(spmfile, "w").write( json.dumps(  emptypackage ) )
                setProperty(package_name, "package_name", package_name, package_dir=package_dir)
                setProperty(package_name, "package_type", package_type, package_dir=package_dir)
            
                generateCMakeLists(package_name, package_dir=package_dir)    


