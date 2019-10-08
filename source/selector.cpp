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
Selector& Selector::GetInstance()
{
	static Selector instance;
	return instance;
}

Selector::Selector()
{
	LOG_TRACE(Core, "Initializing Selector");

	IsRegistered<ExtrudeGeometry>();
	IsRegistered<CreateRectGeometry>();
	IsRegistered<TriangulateGeometry>();
	IsRegistered<ExportGeometry>();

	IsRegistered<OperationNode>();
	IsRegistered<InputInterfaceNode>();
	IsRegistered<OutputInterfaceNode>();
	IsRegistered<ParameterNode>();

	m_proceduralObjectSystem = std::make_shared<ProceduralObjectSystem>();
	m_proceduralObjectSystem->RegisterProceduralComponentSystem(std::make_shared<GeometrySystem>());
	m_proceduralObjectSystem->RegisterProceduralComponentSystem(std::make_shared<HierarchicalSystem>());
}

Selector::~Selector()
{
	LOG_TRACE(Core, "Shutting down Selector");
	Logger::Shutdown();
}

ProceduralObjectSystemPtr Selector::GetProceduralObjectSystem() { return m_proceduralObjectSystem; }

}  // namespace selector
