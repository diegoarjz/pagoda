<a href="https://www.buymeacoffee.com/diegoarjz" target="_blank"><img src="https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png" alt="Buy Me A Coffee" style="height: 41px !important;width: 174px !important;box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;-webkit-box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;" ></a>

| Branch | master| development |
|--------|-------|-------------|
| | ![Build](https://github.com/diegoarjz/pagoda/workflows/Build/badge.svg?branch=master) | ![Build](https://github.com/diegoarjz/pagoda/workflows/Build/badge.svg?branch=development) |

You can find Pagoda's documentation [here](https://diegoarjz.github.io/pagoda).

# What is Pagoda

![banner](https://user-images.githubusercontent.com/6498824/68999381-481b2f80-08b7-11ea-8259-05bbc2b0a454.png)

![graph](https://user-images.githubusercontent.com/6498824/68999389-5bc69600-08b7-11ea-93f0-c16b6dabc04a.png)

The goal of Pagoda is to be a procedural modelling framework aimed at
supporting researchers and academics in this field. Therefore, it must be
flexible enough to allow the implementation of multiple algorithms and
methodologies.

Currently, Pagoda implements a graph-based approach since it is considered to
be one of the most flexible methods to specify the rules in procedural
modelling. The idea is to, eventually, reach a design that allows its users to
re-use code from any of the available layers to implement other methodologies.

# Building Pagoda

Before building pagoda, you need to install the
[Boost](https://www.boost.org) libraries. Instructions on how to do so can be
found on its website.

Pagoda also needs [Google Test](https://github.com/google/googletest).
However, this dependency is built for you automatically.

Once you have installed [Boost](https://www.boost.org) execute the following
commands in the terminal:

```
$ git clone https://github.com/diegarjz/pagoda.git pagoda
$ conan install -if build pagoda
$ conan build -bf build pagoda
```

If all goes well you will have built and installed the pagoda library,
headers and the `pagoda` executable. The `pagoda` executable is what
allows you to execute a procedural graph in the specified in the pagoda
format.

# Executing your first procedural graph

The `pagoda` executable takes a procedural graph file and optionally executes it.

A procedural graph is a directed graph that details the execution of the
operations and the flow of procedural objects through the operations.

All sorts of concepts in a procedural graph are represented by nodes.
Operations nodes, interfaces between procedural operations, parameter nodes, etc. The reasoning
for this approach is that it allows a higher level of flexibility and
extensibility while defining procedural graphs. Links between nodes usually
represent the flow of procedural objects but different nodes might process the
objects in different ways. Some may create new geometry with procedural
operations. Other might filter the flow of objects through the nodes.

The most simple procedural graph might be one that just creates a rectangle.
This is illustrated in the following snippet:

```
create_rect = CreateRectGeometry { width: 10, height: 10 }
export = ExportGeometry { path: "rect.obj" }
create_rect>out -> in<export;
```

Creating an operation node is done by stating the operation name (e.g.
CreateRectGeometry) followed by its parameters in curly braces.  Nodes are
assigned to names which are then used to create links between them, defining
how objects will flow from one node to the next.

So, for example, in the first line, you create a _CreateRectGeometry_ operation
which, when executed, will create a 10x10 square.

Similarly the second line creates an _ExportGeometry_ operation which will
write geometries to the file specified by its `path` parameter.

Operations can have mulitple named input and output interfaces. You will
usually link an output interface of an upstream node to the input interface of
a downstream node as is done in the third line.

To execute this node you can run the following on the command line:

```
pagoda create_rect.pgd --execute
```

You will then have a rect.obj file with the square.

