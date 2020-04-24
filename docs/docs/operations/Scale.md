# Scale

Scales the input geometries.

## Input Interfaces

| Name | Description                                         |
|------|-----------------------------------------------------|
| in   | The input objects with the geometries to be scaled. |

## Output Interfaces

| Name | Description                                    |
|------|------------------------------------------------|
| out  | The output objects with the scaled geometries. |

## Parameters


| Name          | Type   | Allowed values                                 | Default        | Description                                              |
|---------------|--------|------------------------------------------------|----------------|----------------------------------------------------------|
| x             | float  |                                                |                | The amount to scale along the x axis (world or scope).   |
| y             | float  |                                                |                | The amount to scale along t the y axis (world or scope). |
| z             | float  |                                                |                | The amount to scale along t the z axis (world or scope). |
| pivotal_point | string | "scope_center", "scope_origin", "world_origin" | "scope_center" | The point that the geometry will be scaled towards/away. |

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None

## Usage Example

```
scale_scope_center_in = InputInterface(interface: "in")
scale_scope_center = Operation(operation: "Scale") {
    x: 0.1, y: 0.1, z: 0.1, pivotal_point: "scope_center"
}
scale_scope_center_out = OutputInterface(interface: "out")
scale_scope_center_in -> scale_scope_center -> scale_scope_center_out;
```
