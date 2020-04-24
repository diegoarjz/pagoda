# Split

Splits the incoming geometries with planes created given a series of sizes along a scope's axis.

## Input Interfaces

| Name | Description                                        |
|------|----------------------------------------------------|
| in   | The input objects with the geometries to be split. |

## Output Interfaces

| Name      | Description                               |
|-----------|-------------------------------------------|
| split_$n$ | The output objects with the $n-th$ split. |

## Parameters

Note: the parameters for the Split operation are most likely to change as soon as lists are implemented in selscript.


| Name        | Type    | Allowed values | Default | Description                                                    |
|-------------|---------|----------------|---------|----------------------------------------------------------------|
| axis        | string  | "x", "y", "z"  | "x"     | The scope's axis along which the split planes will be created. |
| split_count | integer |                |         | The number of splits to create.                                |
| split_$n$   | float   |                |         | The size for the $n-th$ split.                                 |

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None

## Usage Example


```
split_in = InputInterface(interface: "in")
split = Operation(operation: "Split") {
    axis: "x",
    split_count: 3,
    split_1: 1,
    split_2: 3,
    split_3: 6
}
split_out_1 = OutputInterface(interface: "split_1")
split_out_2 = OutputInterface(interface: "split_2")
split_out_3 = OutputInterface(interface: "split_3")

```

