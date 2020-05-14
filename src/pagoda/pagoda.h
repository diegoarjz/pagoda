#ifndef PAGODA_PAGODA_H_
#define PAGODA_PAGODA_H_

#include "objects/operation_factory.h"
#include "objects/procedural_object_system.h"
#include "procedural_graph/node_factory.h"

namespace pagoda
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;

/**
 * Entry point class for the pagoda library.
 *
 * Pagoda is fully initialized on construction of this object and
 * shutdown on destruction.
 *
 * There can be several instances of \c Pagoda.
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
	NodeFactoryPtr GetNodeFactory();

	/**
	 * Creates an empty \c Graph.
	 */
	GraphPtr CreateGraph();

	/**
	 * Creates a \c Graph from the file given in \p filePath.
	 */
	GraphPtr CreateGraphFromFile(const std::string &filePath);

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};

}  // namespace pagoda

#endif
