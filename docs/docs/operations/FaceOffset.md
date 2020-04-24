# FaceOffset

Creates new geometries by offsetting each face.

For each face in the input geometry, this operation creates a geometry (the
inner geometry) whose edges are parallel to the input face but offset inwards
by a given amount. It also creates a geometry for each edge of the original
face. This geometry takes the space between the original edge, the
corresponding edge in the inner geometry and the bisecting lines between the
adjacent edges.

In other words, this operation creates an inner geometry and outer geometries such that the outer geometries create a border of a given amount.

## Input Interfaces

| Name | Description                              |
|------|------------------------------------------|
| in   | The input procedural objects to process. |

## Output Interfaces

| Name  | Description                                                                                |
|-------|--------------------------------------------------------------------------------------------|
| inner | Procedural objects created from the interior geometries are output through this interface. |
| outer | Procedural objects created from the border geometries are output through this interface.   |

## Parameters

| Name   | Type  | Allowed values | Default | Description        |
|--------|-------|----------------|---------|--------------------|
| amount | float |                |         | The border amount. |

## Exposed Parameters

None

## Parameters Set in Procedural Objects

| Name       | Description                                               |
|------------|-----------------------------------------------------------|
| offset_tag | A String value that takes the value of "inner" or "outer" |

## Usage Example

```
face_offset_in = InputInterface(interface: "in")
face_offset = Operation(operation: "FaceOffset") { amount: 1 }

face_offset_inner = OutputInterface(interface: "inner")
face_offset_outer = OutputInterface(interface: "outer")

face_offset_in -> face_offset;
face_offset -> face_offset_inner;
face_offset -> face_offset_outer;
```
