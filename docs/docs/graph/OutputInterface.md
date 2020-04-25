# OutputInterface Node

The OutputInterface node is used to extract procedural objects from a specific
ouput interface on the upstream [Operation Node](/graph/Operation).

## Usage Example

```
create_box = Operation(operation: "CreateBoxGeometry") {
    xSize: 10, ySize: 10, zSize: 5
}
create_box_out = OutputInterface(interface: "out")

create_box -> create_box_out;
```
