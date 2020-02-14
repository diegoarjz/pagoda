# CreateRectGeometry

Creates a rectangular geometry in one of the x, y or z planes, centered at the origin.

## Input Interfaces

None


## Output Interfaces

| Name | Description                                      |
|------|--------------------------------------------------|
| out  | The output object with the rectangular geometry. |

## Parameters

| Name   | Type   | Allowed values | Default | Description                                 |
|--------|--------|----------------|---------|---------------------------------------------|
| width  | float  |                |         | The rectangle's width.                      |
| height | float  |                |         | The rectangle's height.                     |
| plane  | string | "x", "y", "z"  | "z"     | The plane in which to create the rectangle. |

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None


## Usage Example

```
create_rect = Operation(operation: "CreateRectGeometry") {
    width: 10, height: 10, plane: "x"
}
create_rect_out = OutputInterface(interface: "out")

create_rect -> create_rect_out;
```
