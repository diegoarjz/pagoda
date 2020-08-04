# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Texture coordinates in geometries.
- Texture coordinates projection based on geometry scope.
- Support for loading PNG images as RGB8.
- Material system and components.
- Methods to create and get nodes by name.
- Traversal classes for graphs:
    - *Linear* traverses the nodes without any particular order.
    - *Forward* traverses the nodes, in breadth first order, starting with the
      input nodes.
- Query nodes in a graph:
    - *InputNode* finds nodes that don't have upstream nodes.
    - *OutputNode* finds nodes that don't have downstream nodes.
- Traversal operations for geometry, providing a cleaner interface for
  operating on geometries.

### Changed

- Restrutctured the code to use the Pitchfork Layout model.
- System registration (e.g., Geometry and Material) is now performed by each
  system. The core systems are automatically registered.

## [0.1.0] - 2020-04-28

### Added

- Procedural graph description file format.
- Operation Node.
- Input Interface Node.
- Output Interface Node.
- Parameter Node.
- Router Node.
- Predicates for procedural objects facing a given direction.
- Create Rect Geometry operation.
- Create Sphere Geometry operation.
- Create Box Geometry operation.
- Export Geometry operation.
- Extract Scope operation.
- Extrude Geometry operation.
- Face Offset operation.
- Triangulate Geometry operation.
- Clip Geometry operation.
- Repeat Split operation.
- Extract Faces operation.
- Translate operation.
- Split operation.
- Scale operation.
- Rotate operation.
- Pgscript scripting language.
- Geometry implementation with Split Point topology.
