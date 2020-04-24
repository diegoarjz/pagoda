# Operations

Procedural operations create, or otherwise manipulate [Procedural
Objects](/objects/objects) and their Procedural Components.

Operations receive input objects and output created objects through named input
and output interfaces respectively. Operations should not modify the input
objects but should, instead always create new objects to be output. The reason
is that the same object can be pushed to input interfaces of different
operations.

## Geometry Creation

| Operation                                                | Description                                                    |
|----------------------------------------------------------|----------------------------------------------------------------|
| [CreateRectGeometry](/operations/CreateRectGeometry)     | Creates a rectangular geometry in either the x, y or z planes. |
| [CreateBoxGeometry](/operations/CreateBoxGeometry)       | Creates a box aligned to the world's axis.                     |
| [CreateSphereGeometry](/operations/CreateSphereGeometry) | Creates a sphere.                                              |
| [ExtractScope](/operations/ExtractScope)                 | Create a box for a given scope.                                |

## Geometry Transform

| Operation                          | Description            |
|------------------------------------|------------------------|
| [Translate](/operations/Translate) | Translates a geometry. |
| [Rotate](/operations/Rotate)       | Rotates a geometry.    |
| [Scale](/operations/Scale)         | Scales a geometry.     |

## Geometry Modification

| Operation                                              | Description                                                                     |
|--------------------------------------------------------|---------------------------------------------------------------------------------|
| [ClipGeometry](/operations/ClipGeometry)               | Clips a geometry with a plane.                                                  |
| [ExtrudeGeometry](/operations/ExtrudeGeometry)         | Extrudes all faces of a geometry by a given amount.                             |
| [Split](/operations/Split)                             | Splits a geometry with a series of planes aligned to one of its scope's axis.   |
| [RepeatSplit](/operations/RepeatSplit)                 | Repeatedly splits a geometry along a scope's axis into equally spaced sections. |
| [TriangulateGeometry](/operations/TriangulateGeometry) | Triangulates a geometry.                                                        |
| [ExtractFaces](/operations/ExtractFaces)               | Creates a new geometry with each face in a previous geometry.                   |
| [FaceOffset](/operations/FaceOffset)                   | Creates new geometries by offsetting each face.                                 |

## Export

| Operation                                    | Description                   |
|----------------------------------------------|-------------------------------|
| [ExportGeometry](/operations/ExportGeometry) | Exports geometries to a file. |


