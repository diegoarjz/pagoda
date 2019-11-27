# Translate

Translates the input geometries.

## Input Interfaces

| Name | Description                                             |
|------|---------------------------------------------------------|
| in   | The input objects with the geometries to be translated. |

## Output Interfaces

| Name | Description                                         |
|------|-----------------------------------------------------|
| out  | The output objects with the translated geometries.. |

## Parameters


| Name  | Type   | Allowed values  | Default | Description                                                                                                         |
|-------|--------|-----------------|---------|---------------------------------------------------------------------------------------------------------------------|
| x     | float  |                 |         | The amount to translate along the x axis (world or scope).                                                          |
| y     | float  |                 |         | The amount to translate along the y axis (world or scope).                                                          |
| z     | float  |                 |         | The amount to translate along the z axis (world or scope).                                                          |
| world | string | "true", "false" | "false" | If false, translates the geometry given the scope's frame of coordinates. If true, translates given the world axis. |

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None

## Usage Example

```
translate_in = InputInterface(interface: "in", offset:0)
translate = Operation(operation: "Translate") {
    x: 1, y: 2, z: 3, world: "true"
}
translate_out = OutputInterface(interface: "out", offset: 0)
translate_in -> translate -> translate_out;
```


