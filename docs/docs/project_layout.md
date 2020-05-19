# Project Layout

**Pagoda** tries to follow the [Pitchfork Layout Model](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs) (PFL) as closely as possible.

The authors succinctly describe PFL as follows:

> PFL a convention for laying out source, build, and resource files in a
> filesystem to aide in uniformity, tooling, understandability, and
> compartmentalization.

It's a very thought out layout that tries to achieve a more standard way of
structuring a C++ source code. To this end, and in the interest of keeping
things as standard as possible with other open source projects, this page
describes the project layout for **pagoda**.

## Top Level Layout

From the layout described in PCL, **pagoda** uses the following:

```text
pagoda/
  build/
  include/
    pagoda/
  src/
    pagoda/
    bin/
  tests/
  external/
  docs/
```

We will start by describing the simplest directories first.

### Build Directory

The purpose of the **build** is to contain ephemeral build results and should
not be checked into the version control system. According to PFL multiple
subdirectories are allowed, each reflecting a different build configuration
(e.g., **Release** or **Debug**).

### Tests Directory

The purpose of the **tests** directory is to contain all source code files used
to compile (**non-unit**) tests. More specifically, this directory contains
regression tests for both **pagoda** graph files and **pgscript** scripts.

To allow **pagoda** to be embedded in other projects, the compilation of the
tests in this directory should be automatically disabled.

Unit tests are placed alongside the respective **unit** they are testing. For
example, if we have a class in `pagoda/src/module/a_class.cpp` its test should
be placed in `pagoda/src/module/a_class.test.cpp`. The advantage being that,
while writing **pagoda**'s source code, it should be easier to navigate between
source and test code and to understand which compilation units have unit tests
or not.

### External Directory

The purpose of the **external** directory is to contain all embeddable projects
that are to be built as a part of **pagoda**. Each subdirectory should be
respective to a different project.

Currently, the only embeddable dependency in **pagoda** is the [google
test](https://github.com/google/googletest). Because it is set as a git
sub-module, its subdirectory within the **exteral** directory is
auto-populated.

### Include Directory

The purpose of the **include** directory is to hold **pagoda**'s public headers
which are meant to be used by clients of the **pagoda** library.

Consumers of **pagoda**'s library should only be pointed to this directory as a
source of include files, preventing users from being able to `#include` files
that are inside the **source** directory.

The file structure within the **include** directory follows the exact same
structure as the **src** directory.

### Src Directory

The purpose the **src** directory is to contain all source code files to build
**pagoda** executable binary and library.

The PFL doesn't shed a lot of light into how the split between library code and
executable code should be achieved. Furthermore, because **pagoda**'s main
codebase lies in its library and it is still not clear how many executables
this project will have, the current choice of splitting is to have a
`src/pagoda/` and `include/pagoda/` directories for the library and a
`src/bin/` directory for the executables. At this stage, it is not prescribed
how the source code for each binary shall be structure within this directory.

C++ namespaces should be reflected in the file structure of both the **src**
and **include** directories. For example the class `pagoda::geometry::Geometry`
should have its c++ file in `pagoda/src/pagoda/geometry/Geometry.cpp` and its
header file in `pagoda/include/pagoda/geometry/Geometry.h` (assuming it's a
public header).

## Namespace Layout

**Pagoda**'s library has several components that should be properly organised
to allow a better and faster development. Because the namespace structure
dictates the file layout, it is more convenient to describe the namespaces and
their structure.

```text
pagoda::
  common::
    debug::
    exception::
    fs::
    instrument::
  math::
  objects::
    core::
    operations::
  graph::
    io::
  geometry::
    core::
    io::
    algorithms::
    operations::
  pgscript::
    value::
    parser::
    intermediate::
    interpreter::
  dynamic::
    binding::
```

### common namespace

The **common** namespace contains code that, as the name implies, is common to
all of **pagoda**. Within it, there are namespaces with code to help in
debugging (e.g., assertions, logging), the base of the exception system,
generic file system handling and code for instrumenting **pagoda** itself
(e.g., profilers and statistics).

### math namespace

The **math** namespace contains mathematical functions and methods that are not
provided by other libraries (currently, **pagoda** is only using **boost**
libraries). Whenever possible, it should be better reusing maths code that is
provided by specific libraries. **Pagoda** doesn't want to reinvent the
mathematical wheel.

### object namespace

The **object** namespace is meat to contain the code related to the procedural
objects system. As described in its [documentation](objects/objects.md), this
is an entity composed system.

Within this namespace, code inside the **core** namespace provides code for the
foundations of this system (e.g., `ProceduralObjectSystem`,
`ProceduralObject`).

### graph namespace

The **graph** namespace contains code that allows defining the procedural
graphs, its rules and their posterior execution. The execution code for the
core node types, along with the extension points, should also reside within
this name space.

Code for loading and saving graphs (to disk or elsewhere) should be contained
in the **pagoda::graph::io** namespace.

### geometry namespace

The **geometry** namespace contains code for geometry and topology
representations in its **core** namespace. In the **io** namespace provides
methods for reading and writing geometries to file or elsewhere.

### pgscript namespace

### dynamic namepsace
