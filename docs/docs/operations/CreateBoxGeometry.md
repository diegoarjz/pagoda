# CreateBoxGeometry

Creates a box geometry.

## Input Interfaces

None

## Output Interfaces

| Name | Description                              |
|------|------------------------------------------|
| out  | The output object with the box geometry. |

## Parameters

| Name  | Type  | Allowed values | Default | Description                            |
|-------|-------|----------------|---------|----------------------------------------|
| xSize | float |                |         | The rectangle's size along the x axis. |
| ySize | float |                |         | The rectangle's size along the y axis. |
| zSize | float |                |         | The rectangle's size along the z axis. |

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None


## Usage Example

```
create_box = Operation(operation: "CreateBoxGeometry") {
    xSize: 10, ySize: 10, zSize: 5
}
create_box_out = OutputInterface(interface: "out", offset: 0)
```
