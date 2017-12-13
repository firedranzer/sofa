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

From the developper point of view we should  be able to write stuff like that in the CMakeLists.txt:  
```
find_package(Sofa.Loader) 
or 
find_package(Sofa.Loader.Mesh)
```



