# Geometry Core Library

The Geometry Core library contains all code related to every type of geometry that will be supported.

# Goals and purposes

To provide a fast, reliable and customisable geometry representations for procedural modelling.

## Polygonal Meshes

A polygonal mesh is characterised by having polygons as faces. Polygonal meshes are separated into vertices and their topologies.

Operations supported:
 
 * Inserting vertices.
 * Creating faces from vertices.
 * Iterating over the vertices.
 * Iterating over the faces.
 * Iterating over the edges.
 * Iterating over the edges around a face.
 * Iterating over the vertices around a face.

### Split Point Geometry Topology

The split point topology is the main topology for polygonal meshes. It consists of _Points_, _SplitPoint_, _Edges_ and _Faces_ entities.

Points relate one-to-one with the geometry vertices. A Point can have multiple SplitPoints where each SplitPoint is contained in a different Face. Since all SplitPoints refer to the same Point, they all share the same position in space. Faces are represented by a counter-clockwise chain of Edges. Finally, Edges are an ordered tuple that link two SplitPoints.

This way, it is possible to have many a vertex with many faces and per-face vertex data. However, this topology does not store face-to-face incidences as, for example, the half-edge data structure.


#### Incidence relations

| Entity     | Relation                                              |
|:-----------|:------------------------------------------------------|
| Point      | References $n$ Edges.                                 |
| SplitPoint | References exactly one Point, one Face and an outgoing and incoming Edge. |
| Edge       | References one source and one destination SplitPoint. |
| Face       | References one SplitPoint.                            |

The Edges referenced by each Point are Edges that leave such Point. The Edge referenced by SplitPoints are the Edges leaving that SplitPoint.

#### Operations

In the following, $p$ represents a Point, $s$ a SplitPoint, $e$ an Edge and $f$ a Face.

Operations on Point:

* $edge(p)$:  Returns the edge leaving $p$.
* $split\_point(p, e)$: Returns the SplitPoint contained $p$ and is the source SplitPoint of $e$.
* $face(p, e)$: Returns the Face incident in $p$ that has as its edge $e$.

Operations on SplitPoint:

* $point(s)$: The Point for this $s$.
* $out\_edge(s)$: The Edge that has $s$ as its source SplitPoint.
* $in\_edge(s)$: The Edge that has $s$ as its destination SplitPoint.
* $face(s)$: The Face that contains $s$.

Operations on Edge:

* $source(e)$: The SplitPoint at the source of $e$.
* $destination(e)$: The SplitPoint at the destination of $e$.
* $face(e)$: The face $e$ belongs to.

Operations on Face:

* $split\_point(f)$: The first SplitPoint contained in $f$.
* $edge(f)$: The edge whose source is the first SplitPoint in $f$.

#### Navigating the topology

Navigating Edges:

* *$next\_edge(e)$*: The next Edge in the chain. Equivalent to $edge(destination(e))$.
* *$prev\_edge(e)$*: The previous Edge in the chain. Equivalent to $in_edge(split\_point(e))$.

Navigating Points:

* *$next\_point(p, e)$*: The next Point along its Edge $e$. Equivalent to $point(destination(e)), e \in edges(p)$.
* *$prev\_point(p, e)$*: The previous Point along its Edge $e$. Equivalent to $point(prev\_split\_point(split\_point(e))), e \in edges(p)$.

Navigating SplitPoints:

* $next\_split\_point(s)$*: The next SplitPoint along its Edge. Equivalent to $split\_point(next\_edge(s))$.
* $prev\_split\_point(s)$*: The previous SplitPoint along its Edge. Equivalent to $source(in\_edge(s))$.

#### Creating the topology

The topology should always be in a consistent and valid state. Therefore it is not possible to directly create or modify its basic elements but, instead, the topology provides operations that manipulate constructs in tested and valid ways.

Creating Faces:
These operations create faces with three points. All the respective SplitPoints and Edges are also created. All the Points used for creating the face are returned in counter-clockwise order. It is not the responsibility of the topology to check whether the points are coplanar or not.

* $create\_face()$: creates a Face with three new Points.
* $create\_face(p_0)$: creates a Face with the given Point. Two new Points are created.
* $create\_face(p_0, p_1)$: creates a Face with the given Points. A new Point is also created.
* $create\_face(p_0, p_1, p_2)$: creates a Face with the given Points.

Adding Points to faces:

* $add\_point\_to\_face(f, e)$: Adds a Point to a Face such that it splits $e$ into two Edges. Creates a new Point. Edge $e$ must belong to the Face $f$.
* $add\_point\_to\_face(f, e, p)$: Adds a Point to the Face such that it splits $e$ into two Edges. Uses the Point $p$ and Edge $e$ must belong to the Face $f$.

#### Modifying the topology

Modifying edges:

* $split\_edge(e)$: Splits the Edge $e$, creating two new Edges and their respective Point and SplitPoint. Returns the two Edges.
* $split\_edge(e, p)$: Splits the Edge $e$, creating two new Edge such that they connect to Point $p$. Returns the two Edges.
* $collapse\_edge(e)$: Collapses the Edge $e$ into a single SplitPoint, creating a new Point.
* $collapse\_edge(e, p)$: Collapse the Edge $e$ into the Point $p$. A new SplitPoint is created for the given Point.

Modifying faces:

* $split\_face(f, e_0, e_1)$: Splits a face into two new faces, connecting the destination SplitPoint of $e_0$ is connected to the source SplitPoint of $e_1$. Two new edges are created connecting these two SplitPoints. These edges belong to different Faces.

#### Deleting parts of the topology

* $delete\_face(f)$: Deletes the Face $f$. This cascades to Edges, SplitPoints and Points (if they become empty).
* $delete\_point(p)$: Deletes the Point $p$. This cascades to Edges, SplitPoints and other Points.
* $delete\_edge(e)$: Deletes the Edge $e$. This cascades to Faces, SplitPoints and Points (if they become empty).
* $delete\_split\_point(s)$: Deletes the SplitPoint $s$. This cascades to Edges, Faces and Points (if they become empty).

#### Querying the topology

Topology queries are more complex than the operations described above as they usually don’t rely only the defined incidence relations. They should be used sparingly as they are more resource intensive.

* $edges(p_0, p_1)$: Returns the Edges that connect Points p_0 and p_1.
* $faces(p)$: Returns the Faces around Point $p$.
* $faces(p_0, p_1)$: Returns the faces that have an Edge connecting Points $p_0$ and $p_1$ in any direction.
* $shared\_edges(f_0, f_1)$: Returns the edges that both Faces $f_0$ and $f_1$ share.
* $is\_valid()$: Checks whether the topology and all its elements are valid. This should only be used for debugging purposes.