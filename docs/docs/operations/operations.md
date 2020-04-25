# Operations

Procedural operations create, or otherwise manipulate [Procedural
Objects](/objects/objects) and their Procedural Components.

Operations receive input objects and output created objects through named input
and output interfaces respectively. Operations should not modify the input
objects but should, instead always create new objects to be output. The reason
is that the same object can be pushed to input interfaces of different
operations.

## Geometry Creation

| Operation                                       | Description                                                    |
|-------------------------------------------------|----------------------------------------------------------------|
| [CreateRectGeometry](CreateRectGeometry.md)     | Creates a rectangular geometry in either the x, y or z planes. |
| [CreateBoxGeometry](CreateBoxGeometry.md)       | Creates a box aligned to the world's axis.                     |
| [CreateSphereGeometry](CreateSphereGeometry.md) | Creates a sphere.                                              |
| [ExtractScope](ExtractScope.md)                 | Create a box for a given scope.                                |

## Geometry Transform

| Operation                 | Description            |
|---------------------------|------------------------|
| [Translate](Translate.md) | Translates a geometry. |
| [Rotate](Rotate.md)       | Rotates a geometry.    |
| [Scale](Scale.md)         | Scales a geometry.     |

## Geometry Modification

| Operation                                     | Description                                                                     |
|-----------------------------------------------|---------------------------------------------------------------------------------|
| [ClipGeometry](ClipGeometry.md)               | Clips a geometry with a plane.                                                  |
| [ExtrudeGeometry](ExtrudeGeometry.md)         | Extrudes all faces of a geometry by a given amount.                             |
| [Split](Split.md)                             | Splits a geometry with a series of planes aligned to one of its scope's axis.   |
| [RepeatSplit](RepeatSplit.md)                 | Repeatedly splits a geometry along a scope's axis into equally spaced sections. |
| [TriangulateGeometry](TriangulateGeometry.md) | Triangulates a geometry.                                                        |
| [ExtractFaces](ExtractFaces.md)               | Creates a new geometry with each face in a previous geometry.                   |
| [FaceOffset](FaceOffset.md)                   | Creates new geometries by offsetting each face.                                 |

## Export

| Operation                           | Description                   |
|-------------------------------------|-------------------------------|
| [ExportGeometry](ExportGeometry.md) | Exports geometries to a file. |


