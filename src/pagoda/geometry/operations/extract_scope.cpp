#include "extract_scope.h"

#include <pagoda/procedural_objects/geometry_component.h>
#include <pagoda/procedural_objects/geometry_system.h>
#include <pagoda/procedural_objects/hierarchical_component.h>
#include <pagoda/procedural_objects/hierarchical_system.h>
#include <pagoda/procedural_objects/procedural_component.h>
#include <pagoda/procedural_objects/procedural_object_system.h>

#include <pagoda/geometry/algorithms/create_box.h>

namespace pagoda::geometry::operations
{
using namespace geometry::core;
using namespace geometry::algorithms;

const std::string ExtractScope::inputGeometry("in");
const std::string ExtractScope::outputGeometry("out");
const char* ExtractScope::name = "ExtractScope";

ExtractScope::ExtractScope(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(inputGeometry);
	CreateOutputInterface(outputGeometry);
}

ExtractScope::~ExtractScope() {}

void ExtractScope::DoWork()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	while (HasInput(inputGeometry))
	{
		// Geometry
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(outputGeometry);

		std::shared_ptr<GeometryComponent> inGeometryComponent =
		    geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		std::shared_ptr<GeometryComponent> outGeometryComponent =
		    geometrySystem->CreateComponentAs<GeometryComponent>(outObject);

		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto outGeometry = std::make_shared<Geometry>();

		CreateBox<Geometry> createBox(inGeometryComponent->GetScope());
		createBox.Execute(outGeometry);

		outGeometryComponent->SetGeometry(outGeometry);
		outGeometryComponent->SetScope(inGeometryComponent->GetScope());

		// Hierarchy
		std::shared_ptr<HierarchicalComponent> inHierarchicalComponent =
		    hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		std::shared_ptr<HierarchicalComponent> outHierarchicalComponent =
		    hierarchicalSystem->GetComponentAs<HierarchicalComponent>(outObject);

		hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
	}
}
}  // namespace pagoda::geometry::operations

