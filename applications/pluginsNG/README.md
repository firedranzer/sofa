Simple experiment on how to modularize Sofa in an expressive & efficient way. 

From the user point of view we should  be able to write stuff like that:  
```cpp
<Node name="root">
    <From plugin="Sofa.Loader.Mesh" import="*"/>
    <From plugin="Sofa.Loader.Mesh" import="ObjLoader" as="MyObjLoader"/>
    
    <Sofa.Loader.Mesh.ObjLoader/>
    <ObjLoader/>
    <MyObjLoader/>

</Node>
```
To load specific component (in the Sofa.Load.Mesh factory namespace) and also control the aliasing to avoid name clash. 


From the CMakeLists.txt point of view we should  be able to write stuff like that:  
```
find_package(Sofa.Loader) 
or 
find_package(Sofa.Loader.Mesh)
```
To control what I link against and thus my dependency grah. 


From the C++ implementation woud like to have:
```cpp 
namespace sofa
{
    namespace loader
    {
        namespace mesh
        {
            namespace _objloader_
            {
                class ObjLoader
                {
                
                };
            }
            using _objloader_::ObjLoader ; 
        }
    }
}
...
```


From the C++ client code (the one that use a class from Sofa) I woud like to do
```
#include <sofa/loader/mesh/ObjLoader.h> /// Or <Sofa/Loader/Mesh/ObjLoader>
using sofa::loader::mesh::ObjLoader ; 
...
```

From the filesystem organization point of view I would like to have:
 - extlibs
 - sofa
   - core
   - baselibrary
    - loader  (so in sofa::baselibrary::loader and accessed <RequiredPlugin name="Sofa.BaseLibrary.Loader")
    - xx
    - yy 
   - flexible (with of course sofa::flexible::* namespace and <RequiredPlugin name="Sofa.Flexible">)
   - compliant
   - ...
 - applications
   - runSofa
   - ...etc...
 - ...
 
 What I don't like with this is that 
