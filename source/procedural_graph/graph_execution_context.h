#ifndef SELECTOR_PROCEDURAL_GRAPH_GRAPH_EXECUTION_CONTEXT_H_
#define SELECTOR_PROCEDURAL_GRAPH_GRAPH_EXECUTION_CONTEXT_H_

#include <procedural_objects/geometry_system.h>
#include <procedural_objects/procedural_object.h>

namespace selector
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;

struct GraphExecutionContext
{
public:
	GraphExecutionContext(GraphPtr graph, ProceduralObjectSystemPtr object_system, GeometrySystemPtr geom_sys);

	GraphPtr m_graph;
	ProceduralObjectSystemWeakPtr m_proceduralObjectSystem;
	GeometrySystemWeakPtr m_geometrySystem;
};  // struct GraphExecutionContext
}  // namespace selector
#endif
