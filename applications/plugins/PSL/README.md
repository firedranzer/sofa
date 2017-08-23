Python Scene Language for Sofa
===========
The Python Scene Language (PSL) is an attempt to mixe the advantages of *scn* and *pyscn* in an unified
framework. The language has been specifically designed to make sofa scenes in an elegant and powerful
way.

PSL features:
- scene description (as we have in .scn)
- scene programmability (with embeded Python)
- scene templates (dynamic element that can be reuse and instantiated)
- scene libraries (to store scene templates for reuse and sharing)
- explicit aliasing (to simplify scene writing).
- scene loading & saving.
- multiple alternative syntax (xml, hjson, python-pickled)
- ... more to come ...

Compared to classical *.scn*, PSL offer scene dynamicity,
Comparet to *.pyscn*, PSL offer savability, templates and a more compact declarative syntax

#### First examples
The PSL language itself is defined with an abstract syntax semantics. Doing so allow us to very quickly implement concrete syntaxes
depending on the developpers preferences. We currently have implemented an XML base concrete syntax (that is 99% compatible with
existing .scn file) and an H-JSON one (that look a lot like QML's one). But the important aspect to keep in mind is that whetever
the syntax you like...this is mostly "cosmetic" as the underlying computational model is shared between the different syntaxes.

Let's start with a simple scene example in XML that is only containing classical Sofa components.
```xml
<Node name="root">
        <Node name="child1">
                <MechanicalObject name="mstate"/>
                <OglModel filename="anObj.obj"/>
        </Node>
</Node>
```

With PSL you can add dynamicity in your scene using the *Python* component as in:
```xml
<Node name="myNameisRoot">
        <Node name="child1">
                <MechanicalObject name="mstate"/>
                <OglModel filename="anObj.obj"/>
        </Node>
        <Python>
                Sofa.msg_info(myNameIsRoot, "Hello world")
                for i in range(0,10):
                        myNameIsRoot.addChild("three")
        </Python>
</Node>
```

This *Python* component is very important and several aspect have to be noticed:
- it is a component so it can be saved.
- the code in the Python component have direct access to the scene graph object with the right name scoping.

Now if like me you are not fan of XML syntax... you can implement exactely the same scene using
our H-JSON syntax (very close to the QML syntax).

This would look like:
```css
Node : {
        name : "root"
        Node : {
                name : "child1"
                MechanicalObject: { name : "mstate" }
                OglModel : { filename : "anObj.obj" }
        }

        Python : ''''
                  Sofa.msg_info(myNameIsRoot, "PSL offer scene direct scene element access to python code with scoping !!!")
                  for i in range(0,10):
                        myNameIsRoot.addChild("three")
                  '''
}
```

We hope this example gave you some envy to learn more about PSL and its other cool features.


#### Installation & requirement.
The language is under heavy developement so don't trust the code, the examples or the documentation.
It I want it to be finished join the developement effort.

The language is defined as a sofa Plugin named PSL which is currently it is only available in the PSL development branch.

It makes use of the H-JSON parser available at: http://hjson.org/

Hjson installation :
```shell
git clone https://github.com/hjson/hjson-py.git
cd hjson-py
sudo python setup.py install
```

#### Introduction.
For the simplicity of the following we will employ the H-JSON syntax as it provides both readbility,
compactness and clarity.


As we said before the drawback of *.scn* files is that everything is static.
This is why more and more people are using python
to describe scene as it allows to write:
```python
root = Sofa.createNode("root")
child1 = root.createNode("child1")
child1.createObject("MechanicalObject", name="mstate")
child1.createObject("OglModel", name="anObj.obj")
for i in range(0,10):
        child1.createNode("child_"+str(i))
```

The equivalent scene with PSL would be
```css
Node : {
        name : "root"
        Node : {
                name : "child1"
                MechanicalObject: { name : "mstate" }
                OglModel : { filename : "anObj.obj" }
                Python : '''
                         for i in range(0, 10):
                                child1.createChild("child_"+str(i))
                         '''
        }
}
```

At first sight the PSL version look a bit more complex. But it solve a deep problem of the python version.
It can  preserve the scene structure when it is loaded & saved.
This is because in *.pyscn* the python code is executed (consumed) at loading time and thus is not
part of the scene once loaded. The consequence is that saving the scene is in fact storing the *result* of
the execution of the script and thus we are totally loosing the advantages of python as this would give the
following scene:
```python
root = Sofa.createNode("root")
child1 = root.createNode("child1")
child1.createObject("MechanicalObject", name="mstate")
child1.createObject("OglModel", name="anObj.obj")
child1.createNode("child_0")
child1.createNode("child_1")
child1.createNode("child_2")
child1.createNode("child_3")
child1.createNode("child_4")
child1.createNode("child_5")
child1.createNode("child_6")
child1.createNode("child_7")
child1.createNode("child_8")
child1.createNode("child_9")
```

With PSL, this is not a problem because the dynamic fragment are stored *un-executed* in the scene graph.
They can thus be easily modifie, re-run and saved. Storing the python fragment in the scene graph also
permit to implement powerful feature as *templates*.

#### Templates
In PSL a Template is a component that stores a sub-graph in its textual, or parsed, form. A template
can be instantiated multiple time in the scene graph.
```css
Node : {
    name : "root"
    Template : {
        name : "MyTemplate"
        properties : {
            aName : "undefined"
            numpoints : 3
        }
        Node : {
            name : p"aName"
            MechanicalObject: {
                position : p"srange(0, numpoints* 3)"
            }
            UniformMass : {}
            Node : {
                name : "visual"
                OglModel : { filename : "myOBJ.obj"}
            }
        }
    }

    /// The template can then be instantiated using its name as in:
    MyTemplate : {
        aName : "defined1"
        numpoints : 10
    }

    MyTemplate : {
        aName : "defined2"
        numpoints : 100
    }

    /// Or using Python
    Python : '''
             for i in range(0,10):
                instantiate(root, "MyTemplate", {name:"defined"+str(i), numpoints : i})
             '''
}
```

#### Import
To allow template re-usability it is possible to store them in file or directories that can be imported with the Import directive.
In a file named mylibrary.pyjson" define  a template
```hjson
        Template : { name : "MotorA" ... }
        Template : { name : "MotorB" ... }
        Template : { name : "MotorC" .... }
```

Then in your scene file you load and use the template in the following way:
```hjson
Node : {
        Import : mylibrary

        mylibrary.MotorA : {}
        mylibrary.MotorB : {}
        ...
}
```

##### Aliasing
In Sofa the aliasing system is implicit and the alias are defined in the sofa code source. This is really trouble some as users need to *discover* that in a scene "Mesh" is in fact an alias to a "MeshTopology" object. Without proper tools the solution is often to search in the source code which was an alias.

In PSL we are preserving the use of Alias but we make them explicit. So each scene can defined its own set of alias and anyone reading the scene knows what are the alias and what are the real underlying objects.
```hjson
        Import : mylibrary

        Alias : TSPhereModel-CollisionSphere
        Alias : mylibrary.MotorA-MotorA
        Alias : mylibrary.MotorB-MotorB

        /// Now we can use either
        TSPhereModel : {}

        /// or
        CollisionSphere : {}
```
