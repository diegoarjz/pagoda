# Parameter Node

The Parameter node is used to define parameters that can be used in other
nodes.

During the execution of a Parameter node, each of its parameters is injected
into each of the downstream nodes, overwritting its value if the node already
has a parameter with the same name.

## Usage Example

```
parameter = Parameter() { amount: 10 }

create_rect = Operation(operation: "CreateRectGeometry") {
    width: $< amount; >$, height: $< amount / 2; >$
}
extrusion = Operation(operation: "ExtrudeGeometry") {extrusion_amount: $< 2 * amount; >$}

parameter -> create_rect;
parameter -> extrusion;
```
