Simple experiment on how to modularize Sofa in an expressive & efficient way. 

The factory can be extended to 
```cpp
<From plugin='Sofa.Loader.Mesh' import='*'>
<From plugin='Sofa.Loader.Mesh' import='ObjLoader'>
<From plugin='Sofa.Loader.Mesh' import='ObjLoader' as='MyLoader'>



<MyLoader/>

<From plugin='Sofa.Loader' import='*'>



```
