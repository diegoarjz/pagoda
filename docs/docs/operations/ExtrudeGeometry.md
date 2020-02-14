# ExtrudeGeometry

Extrudes all faces of the incoming geometries by an amount.

## Input Interfaces

| Name | Description                                           |
|------|-------------------------------------------------------|
| in   | The input objects with the geometries to be extruded. |

## Output Interfaces

| Name | Description                                                                     |
|------|---------------------------------------------------------------------------------|
| out  | The output objects with the extruded geometries. |

## Parameters


| Name             | Type  | Allowed values | Default | Description                                   |
|------------------|-------|----------------|---------|-----------------------------------------------|
| extrusion_amount | float |                |         | The amount to extrude along each face normal. |

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None

## Usage Example

```
extrusion_in = InputInterface(interface: "in")
extrusion = Operation(operation: "ExtrudeGeometry") {extrusion_amount: 10}
extrusion_out = OutputInterface(interface: "out")
```
