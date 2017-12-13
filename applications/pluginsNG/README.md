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

From the CMakeLists.txt point of view we should  be able to write stuff like that in the CMakeLists.txt:  
```
find_package(Sofa.Loader) 
or 
find_package(Sofa.Loader.Mesh)
```

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

From the C++ client code I woud like to have
```
#include <sofa/loader/mesh/ObjLoader.h> /// Or <Sofa/Loader/Mesh/ObjLoader>
using sofa::loader::mesh::ObjLoader ; 
...
```

