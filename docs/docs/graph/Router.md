# Router Node

Filters and routes incoming procedural objects based on a set of predicates,
routing them to the respective downstream nodes.

Parameter names must correspond to downstream nodes and its values are
predicate names. So, in the example below, procedural objects that match the
<i>up</i> predicate are routed to <i>top_extrusion_in</i> node and those that
match the <i>left</i> predicate are routed to the <i>left_extrusion_in</i>
node.

## Example Usage

```
extract_faces_in = InputInterface(interface: "in")
extract_faces = Operation(operation: "ExtractFaces")
extract_faces_out = OutputInterface(interface: "out")
extract_faces_in -> extract_faces -> extract_faces_out;

top_extrusion_in = ...
left_extrusion_in = ...
remaining_extrusion = ...

router = Router() {
    top_extrusion_in: up,
    left_extrusion_in: left
}

extract_faces_out -> router;
router -> top_extrusion_in;
router -> left_extrusion_in;
```

## Available Predicates

| Name  | Description                                                          |
|-------|----------------------------------------------------------------------|
| up    | Matches objects whose scope's _z_ axis is pointing towards (0,0,1).  |
| down  | Matches objects whose scope's _z_ axis is pointing towards (0,0,-1). |
| left  | Matches objects whose scope's _z_ axis is pointing towards (0,1,0).  |
| right | Matches objects whose scope's _z_ axis is pointing towards (0,-1,0). |
| front | Matches objects whose scope's _z_ axis is pointing towards (1,0,0).  |
| back  | Matches objects whose scope's _z_ axis is pointing towards (-1,0,0). |
