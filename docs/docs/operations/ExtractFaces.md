# ExtractFaces

Creates geometries from the faces of incoming geometries.

## Input Interfaces

| Name | Description                                                            |
|------|------------------------------------------------------------------------|
| in   | The input objects with the geometries whose faces are to be extracted. |

## Output Interfaces

| Name | Description            |
|------|------------------------|
| out  | The output geometries. |

## Parameters

None

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None

## Usage Example

```
extract_faces_in = InputInterface(interface: "in")
extract_faces = Operation(operation: "ExtractFaces")
extract_faces_out = OutputInterface(interface: "out")
extract_faces_in -> extract_faces -> extract_faces_out;
```


