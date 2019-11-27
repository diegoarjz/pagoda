# ExportGeometry

Exports geometries to obj files.

## Input Interfaces

| Name | Description                       |
|------|-----------------------------------|
| in   | The input objects to be exported. |

## Output Interfaces

None

## Parameters

None

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None

## Exposed Parameters

| Name     | Description                                                |
|----------|------------------------------------------------------------|
| op.count | The number of geometries exported so far by the operation. |

## Usage Example

```
export_geometry_in = InputInterface(interface: "in", offset: 0)
export_geometry = Operation(operation: "ExportGeometry") {
    path: $< "geometry" + op.count + ".obj"; >$
}
export_geometry_in -> export_geometry;
extract_faces_in -> extract_faces -> extract_faces_out;
```

