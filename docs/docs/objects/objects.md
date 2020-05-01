# Procedural Objects

To achieve more flexibility Pagoda implements the concept of *Procedural
Objects* and *Procedural Components* which is implemented with an [Entity
Component System](https://en.wikipedia.org/wiki/Entity_component_system)
architectural pattern. You can, therefore, think of *Procedural Objects* as
bags of *Procedural Components*. For each type of *Procedural Component* there
is a *Procedural Component System* that manages all of its respective
instances.

Operations always receive input [Procedural Objects](/objects/objects) through
their input interfaces and create new [Procedural Objects](/objects/objects)
with new *Procedural Components* which are pushed to output interfaces.

## Geometry Component

The *Geometry Component* is what holds the geometry and scope for a *Procedural
Object*. Most operations will manipulate this component somehow.

## Hierarchical Component

The *Hierarchical Component* is used to keep track of which *Procedural
Objects* were used to create other *Procedural Objects*. For example, if an
*Split* operation creates the **A** and **B** objects from an input **I**
object, then **A** and **B** are children of **I**.
