#include "graph_execution_context.h"

namespace selector
{
GraphExecutionContext::GraphExecutionContext(GraphPtr graph, ProceduralObjectSystemPtr object_system,
                                             GeometrySystemPtr geom_sys)
    : m_graph(graph), m_proceduralObjectSystem(object_system), m_geometrySystem(geom_sys)
{
}
}  // namespace selector
