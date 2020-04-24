# RepeatSplit

Repeatedly splits the incoming geometries with planes created given a size along a scope's axis.

## Input Interfaces

| Name | Description                                        |
|------|----------------------------------------------------|
| in   | The input objects with the geometries to be split. |

## Output Interfaces

| Name | Description                       |
|------|-----------------------------------|
| out  | The output objects split objects. |

## Parameters

| Name   | Type   | Allowed values  | Default | Description                                                            |
|--------|--------|-----------------|---------|------------------------------------------------------------------------|
| axis   | string | "x", "y", "z"   | "x"     | The scope's axis along which the split planes will be created.         |
| size   | float  |                 |         | The distance between splits.                                           |
| adjust | string | "true", "false" | "false" | Wether to round *size* such that there is an integer number of splits. |

## Exposed Parameters

None

## Parameters Set in Procedural Objects

| Name  | Description                     |
|-------|---------------------------------|
| index | The split index, starting in 0. |

## Usage Example

```
repeat_split_in = InputInterface(interface: "in")
repeat_split = Operation(operation: "RepeatSplit") { size: 5, adjust: "false", axis: "x" }
repeat_split_out = OutputInterface(interface: "out")
repeat_split_in -> repeat_split -> repeat_split_out;
```
