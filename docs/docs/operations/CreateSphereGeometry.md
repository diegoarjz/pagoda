# CreateSphereGeometry

Creates a sphere geometry centered at the origin.

## Input Interfaces

None

## Ouput Interfaces

| Name | Description                                 |
|------|---------------------------------------------|
| out  | The output object with the sphere geometry. |

## Parameters

| Name   | Type  | Allowed values | Default | Description                        |
|--------|-------|----------------|---------|------------------------------------|
| radius | float |                | 1.0     | The sphere's radius.               |
| slices | int   |                | 10      | The number of meridians to create. |
| stacks | int   |                | 10      | The number of parallels to create. |

## Exposed Parameters

None

## Parameters set in Procedural Objects

None

## Usage Example

```
create_sphere = Operation(operation: "CreateSphereGeometry") {
    radius: 2, slices: 20, stacks: 20
}
create_sphere_out = OutputInterface(interface: "out")

create_sphere -> create_sphere_out;
```
