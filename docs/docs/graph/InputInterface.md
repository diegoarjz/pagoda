# InputInterface Node

The InputInterface node is used to define which input interface on the downstream [Operation Node](/graph/Operation) the procedural objects should be pushed into.

## Example Usage
```
extract_faces_in = InputInterface(interface: "in")
extract_faces = Operation(operation: "ExtractFaces")

extract_faces_in -> extract_faces;
```
