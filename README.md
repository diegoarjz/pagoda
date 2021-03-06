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
$ cd pagoda
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make && make test && make install
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
create_rect = Operation(operation: "CreateRectGeometry") { width: 10, height: 10 }
create_rect_out = OutputInterface(interface: "out")
create_rect -> create_rect_out;

export_in = InputInterface(interface: "in")
export = Operation(operation: "ExportGeometry") { path: "rect.geom" }
export_in -> export;

create_rect_out -> export_in;
```

Creating a node is similar to constructing an object in object oriented
programming. You call the constructor of the node's type (e.g., Operation or
OutputInterface) with some parameters inside the round brackets (the
construction arguments). Because all nodes can contain parameters (influencing
the node's execution) you specify these inside the curly brackets (the
execution parameters).

Nodes can be assigned to names which are then used to create links between
nodes.

So, for example, in the first line, you create an _Operation_ node whose
operation is a _CreateRectGeometry_. When the node is executed the execution
parameters will be passed to the operation, which will create a 10x10 square.

On the second line, an _OutputInterface_ node is created. Since the
_OutputInterface_ nodes don't require execution parameters the curly brackets
were omitted. The third line links both nodes.

Similarly, lines four to six create an Operation node to export the geometry
and its input interface. The final line links the output of the create_rect
operation and the export geometry.

To execute this node you can run the following on the command line:

```
pagoda create_rect.pgd --execute
```

You will then have a rect.obj file with the square.

