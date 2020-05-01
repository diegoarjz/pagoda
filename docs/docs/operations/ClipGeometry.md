# ClipGeometry

Clips the incoming geometries agains an arbitrary plane.

## Input Interfaces

| Name | Description                                          |
|------|------------------------------------------------------|
| in   | The input objects with the geometries to be clipped. |

## Output Interfaces

| Name  | Description                                                                    |
|-------|--------------------------------------------------------------------------------|
| front | The output objects with the clipped geometries that are in front of the plane. |
| back  | The output objects with the clipped geometries that are behind of the plane.   |

## Parameters


| Name  | Type  | Allowed values | Default | Description                                                                                           |
|-------|-------|----------------|---------|-------------------------------------------------------------------------------------------------------|
| plane | Plane |                |         | The plane with which to clip the geometries. Currently must be specified from a pgscript expression. |

## Exposed Parameters

None

## Parameters Set in Procedural Objects

None

## Usage Example

```
clip_geometry_in = InputInterface(interface: "in")
clip_geometry = Operation(operation: "ClipGeometry") {
    plane: $< Plane(Vector3(0, 0, 4), Vector3(1, 2, 3)); >$
}
front_geometry = OutputInterface(interface: "front")
back_geometry = OutputInterface(interface: "back")

```

