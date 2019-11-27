# ExtractScope

Creates a box from the scope of an input object.

## Input Interfaces

| Name | Description                                            |
|------|--------------------------------------------------------|
| in   | The input objects from which scopes will be extracted. |

## Output Interfaces

| Name | Description                              |
|------|------------------------------------------|
| out  | The output object with the box geometry. |

## Parameters

None

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None

## Usage Example

```
extract_scope_in = InputInterface(interface: "in", offset: 0)
extract_scope = Operation(operation: "ExtractScope")
extract_scope_out = OutputInterface(interface: "out", offset: 0)

extract_scope_in -> extract_scope -> extract_scope_out;
```

