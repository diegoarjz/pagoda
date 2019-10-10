#include "selector.h"

#include "common/factory.h"
#include "common/logger.h"

#include <procedural_graph/input_interface_node.h>
#include <procedural_graph/operation_node.h>
#include <procedural_graph/output_interface_node.h>
#include <procedural_graph/parameter_node.h>
#include <procedural_objects/create_rect.h>
#include <procedural_objects/export_geometry.h>
#include <procedural_objects/extrude_geometry.h>
#include <procedural_objects/geometry_system.h>
#include <procedural_objects/hierarchical_system.h>
#include <procedural_objects/triangulate_geometry.h>

namespace selector
{
Selector::Selector()
{
	LOG_TRACE(Core, "Initializing Selector");

	m_proceduralObjectSystem = std::make_shared<ProceduralObjectSystem>();
	m_proceduralObjectSystem->RegisterProceduralComponentSystem(std::make_shared<GeometrySystem>());
	m_proceduralObjectSystem->RegisterProceduralComponentSystem(std::make_shared<HierarchicalSystem>());

	m_operationFactory = std::make_shared<OperationFactory>();

    // Register Nodes
	m_nodeFactory = std::make_shared<NodeFactory>();
	m_nodeFactory->Register("InputInterface", []() { return std::make_shared<InputInterfaceNode>(); });
	m_nodeFactory->Register("OutputInterface", []() { return std::make_shared<OutputInterfaceNode>(); });
	m_nodeFactory->Register("Parameter", []() { return std::make_shared<ParameterNode>(); });
	m_nodeFactory->Register("Operation",
	                        [this]() { return std::make_shared<OperationNode>(m_operationFactory); });

    // Register Operations
    m_operationFactory->Register("CreateRectGeometry", [this](){ return std::make_shared<CreateRectGeometry>(m_proceduralObjectSystem); });
    m_operationFactory->Register("ExportGeometry", [this](){ return std::make_shared<ExportGeometry>(m_proceduralObjectSystem); });
    m_operationFactory->Register("ExtrudeGeometry", [this](){ return std::make_shared<ExtrudeGeometry>(m_proceduralObjectSystem); });
    m_operationFactory->Register("TriangulateGeometry", [this](){ return std::make_shared<TriangulateGeometry>(m_proceduralObjectSystem); });
}

Selector::~Selector()
{
	LOG_TRACE(Core, "Shutting down Selector");
	Logger::Shutdown();
}

ProceduralObjectSystemPtr Selector::GetProceduralObjectSystem() { return m_proceduralObjectSystem; }

OperationFactoryPtr Selector::GetOperationFactory() { return m_operationFactory; }

NodeFactoryPtr Selector::GetNodeFactory() { return m_nodeFactory; }

}  // namespace selector
