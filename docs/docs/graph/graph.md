# Procedural Graph

Procedural graphs specify how [procedural objects](/objects/objects) flow
through the graph's nodes and, therefore, define the rules for procedural
generation in selector.

Graphs are created by reading from a graph file in selector's own graph format.

## Nodes

| Name                                  | Description                                                                                        |
|---------------------------------------|----------------------------------------------------------------------------------------------------|
| [Operation](Operation.md)             | Defines an [operation](/operations/operations)                                                     |
| [InputInterface](InputInterface.md)   | Routes incoming [objects](/objects/objects) to an input interface of a operation node's operation. |
| [OutputInterface](OutputInterface.md) | Fetches the [objects](/objects/objects) of an output interface of a operation node's operation.    |
| [Parameter](Parameter.md)             | Allows to create parameters and expressions that are set in operation nodes.                       |
| [Router](Router.md)                   | Filters and routes incoming procedural objects to different downstream nodes.                      |
