# Operation Node

Defines an operation to be applied to incoming procedural objects.

An <i>Operation</i> node has a single construction parameter (<i>operation</i>) which is used to define the operation to execute.

As any other node, it can also have execution parameters which are passed to the operation.

## Example Usage

```
create_box = Operation(operation: "CreateBoxGeometry") {
    xSize: 10,
    ySize: 10,
    zSize: 5
}
```
