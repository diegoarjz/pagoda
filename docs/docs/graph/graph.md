# Procedural Graph

Procedural graphs specify how [procedural objects](/objects/objects) flow through the graph's nodes and, therefore, define the rules for procedural generation in selector.

Graphs are created by reading from a graph file in selector's own graph format.

## Nodes

| Name                                      | Description                                                                                        |
|-------------------------------------------|----------------------------------------------------------------------------------------------------|
| [Operation](/graph/Operation)             | Defines an [operation](/operations/operations)                                                     |
| [InputInterface](/graph/InputInterface)   | Routes incoming [objects](/objects/objects) to an input interface of a operation node's operation. |
| [OutputInterface](/graph/OutputInterface) | Fetches the [objects](/objects/objects) of an output interface of a operation node's operation.    |
| [Parameter](/graph/Parameter)             | Allows to create parameters and expressions that are set in operation nodes.                       |
