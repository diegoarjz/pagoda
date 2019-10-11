#ifndef SELECTOR_SELECTOR_H_
#define SELECTOR_SELECTOR_H_

#include "procedural_graph/node_factory.h"
#include "procedural_objects/operation_factory.h"
#include "procedural_objects/procedural_object_system.h"

namespace selector
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;

/**
 * Entry point class for the selector library.
 *
 * Selector is fully initialized on construction of this object and
 * shutdown on destruction.
 *
 * There can be several instances of \c Selector.
 */
class Selector
{
public:
    /**
     * Default constructor. Initializes selector.
     */
	Selector();
    /**
     * Destroys selector, shutting down everything.
     */
	~Selector();

    /**
     * Returns the \c ProceduralObjectSystem for this \c Selector instance.
     *
     * Different \c ProceduralComponentSystem can be registered in the \c ProceduralObjectSystem.
     */
	ProceduralObjectSystemPtr GetProceduralObjectSystem();

    /**
     * Returns the \c OperationFactory used to create \c ProceduralOperation in this instance.
     *
     * Different \c OperationFactory can be registered here.
     */
	OperationFactoryPtr GetOperationFactory();

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

}  // namespace selector

#endif
