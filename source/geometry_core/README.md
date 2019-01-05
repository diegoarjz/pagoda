# Geometry Core Library

The Geometry Core library contains all code related to every type of geometry that will be supported.

# Goals and purposes

To provide a fast, reliable and customisable geometry representations for procedural modelling.

*Note:* Geometry representations should provide separate custom data for vertices and faces

## Polygonal Meshes

A polygonal mesh is characterised by having polygons as faces. Because this is a geometry library for procedural
modelling, it is expected that a geometry's number of vertices, edges and faces are known before creation.

Operations supported:
 
 * Inserting vertices.
 * Creating faces from vertices.
 * Iterating over the vertices.
 * Iterating over the faces.
 * Iterating over the edges.
 * Iterating over the edges around a face.
 * Iterating over the vertices around a face.

### Immutable Polygonal Meshes

After finishing creating an immutable polygonal mesh (e.g., added all vertices and faces) a geometry becomes immutable.
The rationale is that, in procedural modelling, manipulating geometry is achieved by creating a new geometry with the
desired modifications.

# Topology Classes

_Work in progress_

How can I create an uniform interface to support all of these?
 * Half-edge?
 * Face Based representation
 * Lists of vertices...

# Storage Classes
 * Handle based?
    * Retrieve data (e.g. vertices or faces) from handles.
