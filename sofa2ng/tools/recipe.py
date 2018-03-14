#!/usr/bin/env python
# -*- coding: utf-8 -*-
import json
from Cheetah.Template import Template
import mc
from mc import actionFromTemplate, reorder

oldnsprefix1 = """
namespace sofa
{

namespace helper
{
"""
oldnsprefix2 = """
namespace sofa
{
namespace helper
{
"""
oldnsprefix3 = """
namespace sofa {
namespace helper {
"""

newnsprefix = """
namespace sofa
{
namespace type
{
"""

package_name = "Sofa.Type"
srcpath = "../../../SofaKernel/framework/sofa/helper"
srcincpath = "sofa/helper"
oldheader = "SOFA_HELPER"
tasks = [["pre", "move", "../../../SofaKernel/framework/sofa/helper/types/fixed_array.cpp", "../../../SofaKernel/framework/sofa/helper/fixed_array.cpp"]]
tasks = []
tasks += [ ["spm", "package", package_name, "init"] ]
tasks += [["spm", "package", package_name, "property", "dependencies", "add-to", ["Sofa.Config", "Sofa.Messaging"]]] 
tasks += [["mkdir", "Sofa.Type/src/sofa/type"]]
tasks += [["mkdir", "Sofa.Type/deprecated_layout/sofa/helper/"]]
tasks += [["mkdir", "Sofa.Type/deprecated_layout/sofa/helper/types"]]
tasks += [["mkconfig", package_name, "src/sofa/type/config.h"]]

tasks += actionFromTemplate(package_name, "typeinfos", ".", srcincpath, oldheader=oldheader, types=[".h", ".cpp"])  

tasks += actionFromTemplate(package_name, "MemoryManager", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward =  {"sofa" : {"helper" : {"using" : " sofa::type::MemoryManager ;"}}} )  
tasks += actionFromTemplate(package_name, "OptionsGroup", srcpath, srcincpath, oldheader=oldheader, forward = {} )  
tasks += actionFromTemplate(package_name, "vector", srcpath, srcincpath,oldheader=oldheader, forward = {"sofa" : {"helper" : {"using" : " sofa::type::vector ;"}}})  
tasks += actionFromTemplate(package_name, "integer_id", srcpath, srcincpath,oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::integer_id ;"}}} )
tasks += actionFromTemplate(package_name, "deque", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {})  
tasks += actionFromTemplate(package_name, "accessor", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using namespace" : " sofa::type ;"}}})  
tasks += actionFromTemplate(package_name, "stable_vector", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::stable_vector ;"}}})  
tasks += actionFromTemplate(package_name, "set", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::set ;"}}})  
tasks += actionFromTemplate(package_name, "pair", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::pair ;"}}})  
tasks += actionFromTemplate(package_name, "list", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::list ;"}}})  
tasks += actionFromTemplate(package_name, "map", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::map ;"}}})   
tasks += actionFromTemplate(package_name, "hash", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::hash ;"}}})   
tasks += actionFromTemplate(package_name, "deque", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::deque ;"}}})   
tasks += actionFromTemplate(package_name, "map_ptr_stable_compare", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using namespace" : " sofa::type ;"}}})  
tasks += actionFromTemplate(package_name, "fixed_array", srcpath, srcincpath, oldheader=oldheader, types=[".h",".cpp"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::fixed_array ;"}}})   
tasks += actionFromTemplate(package_name, "cast", srcpath, srcincpath, oldheader=oldheader, types=[".h"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::cast ;"}}})   
tasks += actionFromTemplate(package_name, "SVector", srcpath, srcincpath, oldheader=oldheader, types=[".h", ".cpp"], forward = {"sofa" : {"helper" : {"using" : " sofa::type::SVector ;"}}})   
tasks += actionFromTemplate(package_name, "RGBAColor", srcpath+"/types", srcincpath+"/types", oldheader="SOFA_HELPER_TYPES", 
                                                       types=[".h", ".cpp"], forward = {"sofa" : {
                                                                    "helper" : { "types" : {"using" : " sofa::type::types::RGBAColor ;"}},
                                                                    "defaulttype" : {"using" : " sofa::type::types::RGBAColor ;"}}
                                                                     })   
tasks += actionFromTemplate(package_name, "Material", srcpath+"/types", srcincpath+"/types", oldheader="SOFA_HELPER_TYPES", 
                                                       types=[".h", ".cpp"], forward = {"sofa" : {"helper" : { "types" : {"using" : " sofa::type::types::Material ;"}}}})   

tasks += [["rename", package_name, "#include <sofa/helper/helper.h>", "#include <sofa/type/config.h>"]]
tasks += [["rename", package_name, oldnsprefix1, newnsprefix]]
tasks += [["rename", package_name, oldnsprefix2, newnsprefix]]
tasks += [["rename", package_name, oldnsprefix3, newnsprefix]]

tasks += [ ["post", "spm", "package", package_name, "generate-cmakelists"] ]

### Refactor from other projects... 
tasks += [["post", "rename", package_name, "#include <sofa/helper/logging/Messaging.h>", "#include <sofa/messaging/Messaging.h>"]]
tasks += [["post", "rename", package_name, "#include <sofa/helper/BackTrace.h>", """
#include <sofa/messaging/BackTrace.h>
using sofa::messaging::BackTrace ; 

#include <sofa/type/typeinfos.h>
"""]]
tasks += [["post", "rename", package_name, "helper::vector", "sofa::type::vector"]]
tasks += [["post", "rename", package_name, "sofa::helper::types::RGBAColor", "sofa::type::types::RGBAColor"]]
tasks += [["post", "rename", package_name, "sofa::helper::defaulttype::RGBAColor", "sofa::type::types::RGBAColor"]]
tasks += [["post", "rename", package_name, " helper::types::RGBAColor", " sofa::type::RGBAColor"]]
tasks += [["post", "rename", package_name, " helper::defaulttype::RGBAColor", " sofa::type::types::RGBAColor"]]
tasks += [["post", "rename", package_name, " defaulttype::RGBAColor", " sofa::type::types::RGBAColor"]]


tasks += [["post", "rename", package_name, " helper::CPUMemoryManager<T>", " sofa::type::CPUMemoryManager<T>"]]
tasks += [["post", "fixheader", package_name+"/src/sofa/type/vector.h", "#include <sofa/defaulttype/DataTypeInfo.h>", ""]]
tasks += [["post", "fixheader", package_name+"/src/sofa/type/vector.cpp", "#include <sofa/helper/vector_device.h>", ""]]
tasks += [["post", "fixheader", package_name+"/src/sofa/type/vector.cpp", "#include <sofa/helper/Factory.h>\n", ""]]
tasks += [["post", "fixheader",  package_name+"/src/sofa/type/fixed_array.cpp", '#include "../fixed_array.h"', "#include <sofa/type/fixed_array.h>"]]
tasks += [["post", "fixheader",  package_name+"/src/sofa/type/fixed_array.h", '#include <sofa/helper/system/config.h>\n', ""]]

tasks += [["post", "fixheader",  package_name+"/src/sofa/type/Material.h", 'SOFA_HELPER_API', "SOFA_TYPE_API"]]
tasks += [["post", "fixheader",  package_name+"/src/sofa/type/Material.cpp", 'SOFA_HELPER_API', "SOFA_TYPE_API"]]

tasks += [["post", "fixheader",  package_name+"/src/sofa/type/Material.h", '#include <sofa/core/core.h>', "#include <sofa/type/config.h>"]]
tasks += [["post", "fixheader",  package_name+"/src/sofa/type/Material.h", '#include <sofa/defaulttype/RGBAColor.h>', "#include <sofa/type/RGBAColor.h>"]]
tasks += [["post", "fixheader",  package_name+"/src/sofa/type/Material.h", '#include <sofa/core/objectmodel/DataFileName.h>\n', ""]]

tasks += [["post", "fixheader",  package_name+"/src/sofa/type/RGBAColor.h", 'SOFA_HELPER_API', "SOFA_TYPE_API"]]
tasks += [["post", "fixheader",  package_name+"/src/sofa/type/RGBAColor.cpp", 'SOFA_HELPER_API', "SOFA_TYPE_API"]]
tasks += [["post", "rename", package_name, "SOFA_HELPER_API", "SOFA_TYPE_API"]]

mc.cook({"commands" : reorder(tasks)})
   


