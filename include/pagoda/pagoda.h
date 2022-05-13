#ifndef PAGODA_PAGODA_H_
#define PAGODA_PAGODA_H_

#include "pagoda/api.h"

#include <memory>
#include <string>

namespace pagoda {

namespace graph {
using GraphPtr = std::shared_ptr<class Graph>;
using NodeFactoryPtr = std::shared_ptr<class NodeFactory>;
} // namespace graph

namespace objects {
using ProceduralObjectSystemPtr = std::shared_ptr<class ProceduralObjectSystem>;
using OperationFactoryPtr = std::shared_ptr<class OperationFactory>;
} // namespace objects

/**
 * Entry point class for the pagoda library.
 *
 * Pagoda is fully initialized on construction of this object and shutdown on
 * destruction. The core Pagoda systems and their operations are registered on
 * initialization. There can be several instances of \c Pagoda and each will
 * have its own instances of the core systems.
 */
class PAGODA_API Pagoda {
public:
  /**
   * Default constructor. Initializes pagoda.
   */
  Pagoda();
  /**
   * Destroys pagoda, shutting down everything.
   */
  ~Pagoda();

  /**
   * Returns the \c ProceduralObjectSystem for this \c Pagoda instance.
   *
   * Different \c ProceduralComponentSystem can be registered in the \c
   * ProceduralObjectSystem.
   */
  objects::ProceduralObjectSystemPtr GetProceduralObjectSystem();

  /**
   * Returns the \c OperationFactory used to create \c ProceduralOperation in
   * this instance.
   *
   * Different \c OperationFactory can be registered here.
   */
  objects::OperationFactoryPtr GetOperationFactory();

  /**
   * Returns the \c NodeFactory used to create \c Node in this instance.
   *
   * Different types of \c NodeFactory can be registered here.
   */
  graph::NodeFactoryPtr GetNodeFactory();

  /**
   * Creates an empty \c Graph.
   */
  graph::GraphPtr CreateGraph();

  /**
   * Creates a \c Graph from the file given in \p filePath.
   */
  graph::GraphPtr CreateGraphFromFile(const std::string &filePath);

  /**
   * Creates a \c Graph from the contents of \c graphStr.
   */
  graph::GraphPtr CreateGraphFromString(const std::string &graphStr);

private:
  class Impl;
  std::unique_ptr<Impl> m_implementation;
};

} // namespace pagoda

#endif
