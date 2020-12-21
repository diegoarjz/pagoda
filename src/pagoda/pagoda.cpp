#include "pagoda.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/factory.h>
#include <pagoda/common/fs/file_util.h>
#include <memory>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/io/reader.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>
#include <pagoda/graph/parameter_node.h>

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
#include <pagoda/geometry/operations/scope_texture_projection.h>
#include <pagoda/geometry/operations/split.h>
#include <pagoda/geometry/operations/translate.h>
#include <pagoda/geometry/operations/triangulate_geometry.h>

#include <pagoda/material/material_system.h>
#include <pagoda/material/operations/set_material.h>
#include <pagoda/material/operations/set_material_attribute.h>
#include <pagoda/material/operations/set_shader.h>
#include <pagoda/material/operations/set_texture.h>

#include <pagoda/geometry/geometry_system.h>
#include <pagoda/geometry/scope_axis_direction_predicate.h>
#include <pagoda/objects/hierarchical_system.h>
#include <pagoda/objects/procedural_object_predicate_registry.h>

namespace pagoda
{
using namespace objects;
using namespace math;
using namespace graph;
using namespace graph::io;

using namespace geometry;
using namespace geometry::operations;

using namespace material;
using namespace material::operations;

class Pagoda::Impl
{
	public:
	Impl(Pagoda *pagoda) : m_pagoda(pagoda), m_initialized(false) {}

	~Impl()
	{
		LOG_TRACE(Core, "Shutting down Pagoda");
		common::debug::Logger::Shutdown();
	}

	ProceduralObjectSystemPtr GetProceduralObjectSystem()
	{
		Init();
		return m_proceduralObjectSystem;
	}

	OperationFactoryPtr GetOperationFactory()
	{
		Init();
		return m_operationFactory;
	}

	NodeFactoryPtr GetNodeFactory()
	{
		Init();
		return m_nodeFactory;
	}

	GraphPtr CreateGraph()
	{
		Init();
		return std::make_shared<Graph>(GetNodeFactory());
	}

	GraphPtr CreateGraphFromFile(const std::string &filePath)
	{
		Init();
		LOG_TRACE(Core, "Creating Graph From File: " << filePath.c_str());
		GraphReader reader(GetNodeFactory());
		GraphPtr graph = reader.Read(common::fs::LoadFileToString(filePath));
		return graph;
	}

	GraphPtr CreateGraphFromString(const std::string &graphStr)
	{
		Init();
		LOG_TRACE(Core, "Creating Graph From String");
		GraphReader reader(GetNodeFactory());
		GraphPtr graph = reader.Read(graphStr);
		return graph;
	}

	void Init()
	{
		if (m_initialized) {
			return;
		}
		m_initialized = true;

		LOG_TRACE(Core, "Initializing Pagoda");

		m_proceduralObjectSystem = std::make_shared<ProceduralObjectSystem>();
		m_operationFactory = std::make_shared<OperationFactory>();
		m_predicateRegistry = std::make_shared<ProceduralObjectPredicateRegistry>();
		m_proceduralObjectSystem->SetPredicateRegistry(m_predicateRegistry);

		ProceduralObjectSystem::Registration(m_pagoda);
		HierarchicalSystem::Registration(m_pagoda);
		GeometrySystem::Registration(m_pagoda);
		MaterialSystem::Registration(m_pagoda);

		// Register Nodes
		{
			m_nodeFactory = std::make_shared<NodeFactory>();
			m_nodeFactory->Register("InputInterface", []() { return std::make_shared<InputInterfaceNode>(); });
			m_nodeFactory->Register("OutputInterface", []() { return std::make_shared<OutputInterfaceNode>(); });
			m_nodeFactory->Register("Parameter", []() { return std::make_shared<ParameterNode>(); });
			m_nodeFactory->Register("Operation", [this]() { return std::make_shared<OperationNode>(m_operationFactory); });
		}

		// Register Predicates
		{
			m_predicateRegistry->Register("front", std::make_shared<ScopeAxisDirectionPredicate>(
			                                         m_proceduralObjectSystem, 'z', boost::qvm::vec<float, 3>{1, 0, 0}));
			m_predicateRegistry->Register("back", std::make_shared<ScopeAxisDirectionPredicate>(
			                                        m_proceduralObjectSystem, 'z', boost::qvm::vec<float, 3>{-1, 0, 0}));
			m_predicateRegistry->Register("up", std::make_shared<ScopeAxisDirectionPredicate>(
			                                      m_proceduralObjectSystem, 'z', boost::qvm::vec<float, 3>{0, 0, 1}));
			m_predicateRegistry->Register("down", std::make_shared<ScopeAxisDirectionPredicate>(
			                                        m_proceduralObjectSystem, 'z', boost::qvm::vec<float, 3>{0, 0, -1}));

			m_predicateRegistry->Register("left", std::make_shared<ScopeAxisDirectionPredicate>(
			                                        m_proceduralObjectSystem, 'z', boost::qvm::vec<float, 3>{0, 1, 0}));
			m_predicateRegistry->Register("right", std::make_shared<ScopeAxisDirectionPredicate>(
			                                         m_proceduralObjectSystem, 'z', boost::qvm::vec<float, 3>{0, -1, 0}));
			m_predicateRegistry->Register("side", std::make_shared<ScopeAxisDirectionPredicate>(
			                                        m_proceduralObjectSystem, 'y', boost::qvm::vec<float, 3>{0, 0, 1}));
		}
	}

	private:
	ProceduralObjectSystemPtr m_proceduralObjectSystem;
	OperationFactoryPtr m_operationFactory;
	NodeFactoryPtr m_nodeFactory;
	ProceduralObjectPredicateRegistryPtr m_predicateRegistry;
	Pagoda *m_pagoda;
	bool m_initialized;
};

Pagoda::Pagoda() : m_implementation(std::make_unique<Pagoda::Impl>(this)) {}
Pagoda::~Pagoda() {}

ProceduralObjectSystemPtr Pagoda::GetProceduralObjectSystem() { return m_implementation->GetProceduralObjectSystem(); }

OperationFactoryPtr Pagoda::GetOperationFactory() { return m_implementation->GetOperationFactory(); }

NodeFactoryPtr Pagoda::GetNodeFactory() { return m_implementation->GetNodeFactory(); }

GraphPtr Pagoda::CreateGraph() { return m_implementation->CreateGraph(); }

GraphPtr Pagoda::CreateGraphFromFile(const std::string &filePath)
{
	return m_implementation->CreateGraphFromFile(filePath);
}

graph::GraphPtr Pagoda::CreateGraphFromString(const std::string &graphStr)
{
	return m_implementation->CreateGraphFromString(graphStr);
}
}  // namespace pagoda
