# Rotate

Rotates the input geometries.

## Input Interfaces

| Name | Description                                          |
|------|------------------------------------------------------|
| in   | The input objects with the geometries to be rotated. |

## Output Interfaces

| Name | Description                                      |
|------|--------------------------------------------------|
| out  | The output objects with the rotated geometries.. |

## Parameters


| Name           | Type   | Allowed values                    | Default | Description                                                                                                   |
|----------------|--------|-----------------------------------|---------|---------------------------------------------------------------------------------------------------------------|
| x              | float  |                                   |         | The amount to rotate around the x axis (world or scope).                                                      |
| y              | float  |                                   |         | The amount to rotate around the y axis (world or scope).                                                      |
| z              | float  |                                   |         | The amount to rotate around the z axis (world or scope).                                                      |
| world          | string | "true", "false"                   | "false" | If false, rotates the geometry given the scope's frame of coordinates. If true, rotates given the world axis. |
| rotation_order | string | combinations of "x", "y", and "z" | "xyz"   | Order in which to apply the rotations.                                                                        |

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None

## Usage Example

```
rotate_in = InputInterface(interface: "in", offset:0)
rotate = Operation(operation: "Rotate") {
    x: 45, y: 0, z: 0, rotation_order: "x", world: "true"
}
rotate_out = OutputInterface(interface: "out", offset: 0)
rotate_in -> rotate -> rotate_out;
```
