#include "pagoda.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/factory.h>
#include <pagoda/common/fs/file_util.h>

#include <pagoda/procedural_graph/graph.h>
#include <pagoda/procedural_graph/input_interface_node.h>
#include <pagoda/procedural_graph/operation_node.h>
#include <pagoda/procedural_graph/output_interface_node.h>
#include <pagoda/procedural_graph/parameter_node.h>
#include <pagoda/procedural_graph/reader.h>
#include <pagoda/procedural_graph/router_node.h>

#include <pagoda/geometry/operations/clip_geometry.h>
#include <pagoda/geometry/operations/create_box.h>
#include <pagoda/geometry/operations/create_rect.h>
#include <pagoda/geometry/operations/create_sphere.h>
#include <pagoda/geometry/operations/export_geometry.h>
#include <pagoda/geometry/operations/extract_faces.h>
#include <pagoda/geometry/operations/extract_scope.h>
#include <pagoda/geometry/operations/extrude_geometry.h>
#include <pagoda/geometry/operations/face_offset.h>
#include <pagoda/geometry/operations/repeat_split.h>
#include <pagoda/geometry/operations/rotate.h>
#include <pagoda/geometry/operations/scale.h>
#include <pagoda/geometry/operations/split.h>
#include <pagoda/geometry/operations/translate.h>
#include <pagoda/geometry/operations/triangulate_geometry.h>

#include <pagoda/procedural_objects/geometry_system.h>
#include <pagoda/procedural_objects/hierarchical_system.h>
#include <pagoda/procedural_objects/procedural_object_predicate_registry.h>
#include <pagoda/procedural_objects/scope_axis_direction_predicate.h>

namespace pagoda
{
using namespace math;
using namespace geometry::operations;

class Pagoda::Impl
{
public:
	Impl()
	{
		LOG_TRACE(Core, "Initializing Pagoda");

		m_proceduralObjectSystem = std::make_shared<ProceduralObjectSystem>();
		m_proceduralObjectSystem->RegisterProceduralComponentSystem(std::make_shared<GeometrySystem>());
		m_proceduralObjectSystem->RegisterProceduralComponentSystem(std::make_shared<HierarchicalSystem>());

		m_operationFactory = std::make_shared<OperationFactory>();
		m_predicateRegistry = std::make_shared<ProceduralObjectPredicateRegistry>();

		// Register Nodes
		{
			m_nodeFactory = std::make_shared<NodeFactory>();
			m_nodeFactory->Register("InputInterface", []() { return std::make_shared<InputInterfaceNode>(); });
			m_nodeFactory->Register("OutputInterface", []() { return std::make_shared<OutputInterfaceNode>(); });
			m_nodeFactory->Register("Parameter", []() { return std::make_shared<ParameterNode>(); });
			m_nodeFactory->Register("Operation",
			                        [this]() { return std::make_shared<OperationNode>(m_operationFactory); });
			m_nodeFactory->Register("Router", [this]() { return std::make_shared<RouterNode>(m_predicateRegistry); });
		}

		// Register Operations
		{
			m_operationFactory->Register("CreateRectGeometry", [this]() {
				return std::make_shared<CreateRectGeometry>(m_proceduralObjectSystem);
			});
			m_operationFactory->Register("CreateSphereGeometry", [this]() {
				return std::make_shared<CreateSphereGeometry>(m_proceduralObjectSystem);
			});
			m_operationFactory->Register("CreateBoxGeometry", [this]() {
				return std::make_shared<CreateBoxGeometry>(m_proceduralObjectSystem);
			});
			m_operationFactory->Register(
			    "ExportGeometry", [this]() { return std::make_shared<ExportGeometry>(m_proceduralObjectSystem); });
			m_operationFactory->Register("ExtractScope",
			                             [this]() { return std::make_shared<ExtractScope>(m_proceduralObjectSystem); });
			m_operationFactory->Register(
			    "ExtrudeGeometry", [this]() { return std::make_shared<ExtrudeGeometry>(m_proceduralObjectSystem); });
			m_operationFactory->Register(
			    "FaceOffset", [this]() { return std::make_shared<FaceOffsetOperation>(m_proceduralObjectSystem); });
			m_operationFactory->Register("TriangulateGeometry", [this]() {
				return std::make_shared<TriangulateGeometry>(m_proceduralObjectSystem);
			});
			m_operationFactory->Register("ClipGeometry",
			                             [this]() { return std::make_shared<ClipGeometry>(m_proceduralObjectSystem); });
			m_operationFactory->Register("RepeatSplit",
			                             [this]() { return std::make_shared<RepeatSplit>(m_proceduralObjectSystem); });
			m_operationFactory->Register("ExtractFaces",
			                             [this]() { return std::make_shared<ExtractFaces>(m_proceduralObjectSystem); });
			m_operationFactory->Register("Translate",
			                             [this]() { return std::make_shared<Translate>(m_proceduralObjectSystem); });
			m_operationFactory->Register("Split",
			                             [this]() { return std::make_shared<Split>(m_proceduralObjectSystem); });
			m_operationFactory->Register("Scale",
			                             [this]() { return std::make_shared<Scale>(m_proceduralObjectSystem); });
			m_operationFactory->Register("Rotate",
			                             [this]() { return std::make_shared<Rotate>(m_proceduralObjectSystem); });
		}

		// Register Predicates
		{
			m_predicateRegistry->Register(
			    "front", std::make_shared<ScopeAxisDirectionPredicate>(m_proceduralObjectSystem, 'z', Vec3F{1, 0, 0}));
			m_predicateRegistry->Register(
			    "back", std::make_shared<ScopeAxisDirectionPredicate>(m_proceduralObjectSystem, 'z', Vec3F{-1, 0, 0}));

			m_predicateRegistry->Register(
			    "up", std::make_shared<ScopeAxisDirectionPredicate>(m_proceduralObjectSystem, 'z', Vec3F{0, 0, 1}));
			m_predicateRegistry->Register(
			    "down", std::make_shared<ScopeAxisDirectionPredicate>(m_proceduralObjectSystem, 'z', Vec3F{0, 0, -1}));

			m_predicateRegistry->Register(
			    "left", std::make_shared<ScopeAxisDirectionPredicate>(m_proceduralObjectSystem, 'z', Vec3F{0, 1, 0}));
			m_predicateRegistry->Register(
			    "right", std::make_shared<ScopeAxisDirectionPredicate>(m_proceduralObjectSystem, 'z', Vec3F{0, -1, 0}));
			m_predicateRegistry->Register(
			    "side", std::make_shared<ScopeAxisDirectionPredicate>(m_proceduralObjectSystem, 'y', Vec3F{0, 0, 1}));
		}
	}

	~Impl()
	{
		LOG_TRACE(Core, "Shutting down Pagoda");
		common::debug::Logger::Shutdown();
	}

	ProceduralObjectSystemPtr GetProceduralObjectSystem() { return m_proceduralObjectSystem; }

	OperationFactoryPtr GetOperationFactory() { return m_operationFactory; }

	NodeFactoryPtr GetNodeFactory() { return m_nodeFactory; }

	GraphPtr CreateGraph() { return std::make_shared<Graph>(GetNodeFactory()); }

	GraphPtr CreateGraphFromFile(const std::string &filePath)
	{
		LOG_TRACE(Core, "Creating Graph From File: " << filePath.c_str());
		GraphReader reader(GetNodeFactory());
		GraphPtr graph = reader.Read(common::fs::LoadFileToString(filePath));
		return graph;
	}

private:
	ProceduralObjectSystemPtr m_proceduralObjectSystem;
	OperationFactoryPtr m_operationFactory;
	NodeFactoryPtr m_nodeFactory;
	ProceduralObjectPredicateRegistryPtr m_predicateRegistry;
};

Pagoda::Pagoda() : m_implementation(std::make_unique<Pagoda::Impl>()) {}
Pagoda::~Pagoda() {}

ProceduralObjectSystemPtr Pagoda::GetProceduralObjectSystem() { return m_implementation->GetProceduralObjectSystem(); }

OperationFactoryPtr Pagoda::GetOperationFactory() { return m_implementation->GetOperationFactory(); }

NodeFactoryPtr Pagoda::GetNodeFactory() { return m_implementation->GetNodeFactory(); }

GraphPtr Pagoda::CreateGraph() { return m_implementation->CreateGraph(); }

GraphPtr Pagoda::CreateGraphFromFile(const std::string &filePath)
{
	return m_implementation->CreateGraphFromFile(filePath);
}
}  // namespace pagoda
