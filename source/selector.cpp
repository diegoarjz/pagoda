#include "selector.h"

#include "common/factory.h"
#include "common/file_util.h"
#include "common/logger.h"

#include <procedural_graph/graph.h>
#include <procedural_graph/input_interface_node.h>
#include <procedural_graph/operation_node.h>
#include <procedural_graph/output_interface_node.h>
#include <procedural_graph/parameter_node.h>
#include <procedural_graph/reader.h>

#include <procedural_objects/clip_geometry.h>
#include <procedural_objects/create_box.h>
#include <procedural_objects/create_rect.h>
#include <procedural_objects/export_geometry.h>
#include <procedural_objects/extract_scope.h>
#include <procedural_objects/extrude_geometry.h>
#include <procedural_objects/geometry_system.h>
#include <procedural_objects/hierarchical_system.h>
#include <procedural_objects/repeat_split.h>
#include <procedural_objects/triangulate_geometry.h>

namespace selector
{
class Selector::Impl
{
public:
	Impl()
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
		m_nodeFactory->Register("Operation", [this]() { return std::make_shared<OperationNode>(m_operationFactory); });

		// Register Operations
		m_operationFactory->Register(
		    "CreateRectGeometry", [this]() { return std::make_shared<CreateRectGeometry>(m_proceduralObjectSystem); });
		m_operationFactory->Register(
		    "CreateBoxGeometry", [this]() { return std::make_shared<CreateBoxGeometry>(m_proceduralObjectSystem); });
		m_operationFactory->Register("ExportGeometry",
		                             [this]() { return std::make_shared<ExportGeometry>(m_proceduralObjectSystem); });
		m_operationFactory->Register("ExtractScope",
		                             [this]() { return std::make_shared<ExtractScope>(m_proceduralObjectSystem); });
		m_operationFactory->Register("ExtrudeGeometry",
		                             [this]() { return std::make_shared<ExtrudeGeometry>(m_proceduralObjectSystem); });
		m_operationFactory->Register("TriangulateGeometry", [this]() {
			return std::make_shared<TriangulateGeometry>(m_proceduralObjectSystem);
		});
		m_operationFactory->Register("ClipGeometry",
		                             [this]() { return std::make_shared<ClipGeometry>(m_proceduralObjectSystem); });
		m_operationFactory->Register("RepeatSplit",
		                             [this]() { return std::make_shared<RepeatSplit>(m_proceduralObjectSystem); });
	}

	~Impl()
	{
		LOG_TRACE(Core, "Shutting down Selector");
		Logger::Shutdown();
	}

	ProceduralObjectSystemPtr GetProceduralObjectSystem() { return m_proceduralObjectSystem; }

	OperationFactoryPtr GetOperationFactory() { return m_operationFactory; }

	NodeFactoryPtr GetNodeFactory() { return m_nodeFactory; }

	GraphPtr CreateGraph() { return std::make_shared<Graph>(GetNodeFactory()); }

	GraphPtr CreateGraphFromFile(const std::string &filePath)
	{
		LOG_TRACE(Core, "Creating Graph From File: %s", filePath.c_str());
		GraphReader reader(GetNodeFactory());
		GraphPtr graph = reader.Read(FileUtil::LoadFileToString(filePath));
		LOG_TRACE(Core, "graph: 0x%x", graph.get());
		return graph;
	}

private:
	ProceduralObjectSystemPtr m_proceduralObjectSystem;
	OperationFactoryPtr m_operationFactory;
	NodeFactoryPtr m_nodeFactory;
};

Selector::Selector() : m_implementation(std::make_unique<Selector::Impl>()) {}
Selector::~Selector() {}

ProceduralObjectSystemPtr Selector::GetProceduralObjectSystem()
{
	return m_implementation->GetProceduralObjectSystem();
}

OperationFactoryPtr Selector::GetOperationFactory() { return m_implementation->GetOperationFactory(); }

NodeFactoryPtr Selector::GetNodeFactory() { return m_implementation->GetNodeFactory(); }

GraphPtr Selector::CreateGraph() { return m_implementation->CreateGraph(); }

GraphPtr Selector::CreateGraphFromFile(const std::string &filePath)
{
	return m_implementation->CreateGraphFromFile(filePath);
}
}  // namespace selector
