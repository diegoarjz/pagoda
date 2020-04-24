# Welcome to Selector

The goal of Selector is to be a procedural modelling framework aimed at supporting researchers and academics in this field. Therefore, it must be flexible enough to allow the implementation of multiple algorithms and methodologies.

Currently, Selector implements a graph-based approach since it is considered to be one of the most flexible methods to specify the rules in procedural modelling. The idea is to, eventually, reach a design that allows its users to re-use code from any of the available layers to implement other methodologies.

## Project layout

    selector/
        source/                         # Selector's library source code
            common/                     # Code common to all layers
            dynamic_value/              # Interface between selector and selscript
            geometry_core/              # Core geometry and topology representation
            geometry_operations/        # Operations on geometries
            math_lib/                   # Mathematics Library
            procedural_graph/           # Procedural graph, nodes and reader
            procedural_objects/         # Procedural objects, components and operations
            selscript/                  # Grammar, parser and interpreter for selscript
        tests/                          # Unit tests and regression tests for selector and selscript
        tools/                          # Executables

## Procedural Objects

[Procedural Objects](objects/objects.md) flow through the graph edges and are manipulated by [Procedural Operations](operations/operations.md).

## Procedural Operations

[Procedural Operations](operations/operations.md) take [Procedural Objects](objects/objects.md) as input, manipulating or transforming them and output new [Procedural Objects](objects/objects.md) through input and output interfaces respectively.

## Procedural Graph

The [Procedural Graph](graph/graph.md) is what defines the rules that generate the geometry and other elements that make up the procedural scene. It defines the flow of [objects](objects/objects.md) through the graph and defines how parameters are set for the [operations](operations/operations.md).


