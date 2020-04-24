# TriangulateGeometry

Triangulates the faces of incoming geometries.

## Input Interfaces

| Name | Description                                               |
|------|-----------------------------------------------------------|
| in   | The input objects with the geometries to be triangulated. |

## Output Interfaces

| Name | Description                      |
|------|----------------------------------|
| out  | The output triangulated objects. |

## Parameters

None

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None

## Usage Example

```
triangulate_geometry_in = InputInterface(interface: "in")
triangulate_geometry = Operation(operation: "TriangulateGeometry")
triangulate_geometry_out = OutputInterface(interface: "out")
```

