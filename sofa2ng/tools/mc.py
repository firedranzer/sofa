#!/usr/bin/env python
# -*- coding: utf-8 -*-
import json, os
from Cheetah.Template import Template
import sr

def lprint(l):
    print(json.dumps(l, indent=4, separators=(',', ':')))
     
def nameMap(file, context):
    return (context["recipedir"]+"/"+file, context["dstdir"]+"/"+file)

def makeATestContextFrom(context):
    nc = context.copy()
    nc["package_name"] = context["package_name"]+"_tests"
    nc["package_dir"] = context["package_dir"]+"/tests"
    nc["dstdir"] =  nc["package_dir"]+"/src"
    nc["cmake_template"] = "TestCMakeLists.txt.tpl"
    return nc
                           
def moveCodeAndPatch(filenames, nameMap, forward=None, **kwargs):
    if isinstance(filenames, str):
        filenames = [filename]
        
    #dstpath = package_name+"/src/"+(package_name.lower().replace(".","/"))
    #dstincpath0 = (package_name.lower().replace(".","/"))
    
    #dstincpath = "src/"+(package_name.lower().replace(".","/"))
    
    #if oldheader == None:
    #    oldheader = package_name.upper()
    #oldheader += "_"+name.upper()
    
    #if newheader == None:
    #    newheader = (package_name.upper()+"_"+name.upper()).replace(".","_")

    #tasks = []
    #file2property = {".h" : "header_files", ".inl" : "header_files", ".cpp" : "source_files" }
    #for t in types:    
    #    EXT = t.upper().replace(".","_")
    #    tasks.append(["move", srcpath+"/"+name+t, dstpath+"/"+name+t])
    #    if t in file2property:
    #        tasks.append(["spm", "package", package_name, "property", file2property[t], "add-to", dstincpath+"/"+name+t])
    #    else:
    #        tasks.append(["spm", "package", package_name, "property", "extra_files", "add-to", dstincpath+"/"+name+t])
    #    
    #    tasks.append(["rename", package_name, "#include <"+srcincpath+"/"+name+".", "#include <"+dstincpath0+"/"+name+"."])
    #        
    #    tasks.append(["fixheader", dstpath+"/"+name+t, oldheader+EXT, newheader+EXT])
    #    if forward != None and t == ".h":
    #        tasks.append(["mkforward", package_name, srcincpath+"/"+name+t, dstincpath0+"/"+name+t, forward])
    #        tasks.append(["spm", "package", package_name, "property", "header_files", "add-to", "deprecated_layout/"+srcincpath+"/"+name+t])
      
    tasks = []  
    
    file2property = {".h" : "header_files", ".inl" : "header_files", ".cpp" : "source_files" }

    for filename in filenames:
       ext = os.path.splitext(filename)[1]
       srcfilename,dstfilename = nameMap(filename, kwargs)
       r = os.path.commonprefix([dstfilename+"/", kwargs["package_dir"]+"/"])
       dstrelativefilename = dstfilename[len(r):]
       tasks.append(["move", srcfilename, dstfilename])
       tasks.append(["spm", "add-to-property", file2property[ext], dstrelativefilename, kwargs])
         
    return tasks

def reorder(tasks):
    mkdir = []
    move = []
    create = []
    spm = []
    fix = []
    last = []
    pre = []
    for i in tasks: 
        if i[0] in ["mkdir"]:
            mkdir.append(i)
        elif i[0] in ["move"]:
            move.append(i)
        elif i[0] in ["mkconfig"]:
            create.append(i)
        elif i[0] in ["spm", "mkforward", "fixheaders"]:
            spm.append(i)
        elif i[0] in ["rename"]:
            fix.append(i)
        elif i[0] in ["post"]:
            last.append(i[1:])
        elif i[0] in ["post"]:
            pre.append(i[1:])
        else:
            last.append(i)
    return pre+mkdir+move+spm+create+fix+last

def cook(commands):
    sr.replayRefactoring(commands)


if __name__ == "__main__":
    import sys
    import runpy
    import os
    if len(sys.argv) != 4:
        print("Usage: mc recipe <srcpath> <dstpath>") 
    print("MasterChief is now cooking '"+sys.argv[1]+"' for you.")
    context = {"rootsrcdir" : sys.argv[2] , "rootdstdir": sys.argv[3] }

    if not os.path.exists(context["rootdstdir"]):
        os.makedirs(context["rootdstdir"])    
    
    l=runpy.run_path(sys.argv[1], context)
    sr.replayRefactoring(l["command"])

