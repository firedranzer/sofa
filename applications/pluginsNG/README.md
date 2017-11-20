Simple experiment on how to modularize Sofa in an expressive & efficient way. 

The factory can be extended to 
```cpp
<Node name="root">
    <From plugin="Sofa.Loader.Mesh" import="*"/>
    <From plugin="Sofa.Loader.Mesh" import="ObjLoader" as="MyObjLoader"/>
    
    <Sofa.Loader.Mesh.ObjLoader/>
    <ObjLoader/>
    <MyObjLoader/>

</Node>
```
