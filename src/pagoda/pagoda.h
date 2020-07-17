#ifndef PAGODA_PAGODA_H_
#define PAGODA_PAGODA_H_

#include "graph/node_factory.h"
#include "objects/operation_factory.h"
#include "objects/procedural_object_system.h"

namespace pagoda
{
namespace graph
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;
}  // namespace graph

/**
 * Entry point class for the pagoda library.
 *
 * Pagoda is fully initialized on construction of this object and shutdown on
 * destruction. The core Pagoda systems and their operations are registered on
 * initialization. There can be several instances of \c Pagoda and each will
 * have its own instances of the core systems.
 */
class Pagoda
{
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
	 * Different \c ProceduralComponentSystem can be registered in the \c ProceduralObjectSystem.
	 */
	objects::ProceduralObjectSystemPtr GetProceduralObjectSystem();

	/**
	 * Returns the \c OperationFactory used to create \c ProceduralOperation in this instance.
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
	graph::GraphPtr CreateGraphFromFile(const std::string& filePath);

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};

}  // namespace pagoda

#endif
