#include "extract_scope.h"

#include "geometry_component.h"
#include "geometry_operations/create_box.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "parameter/parameter.h"
#include "procedural_component.h"
#include "procedural_object_system.h"

namespace selector
{
const InterfaceName ExtractScope::inputGeometry("in", 0);
const InterfaceName ExtractScope::outputGeometry("out", 0);
const char* ExtractScope::name = "ExtractScope";

ExtractScope::ExtractScope(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(inputGeometry);
	CreateOutputInterface(outputGeometry);
}

ExtractScope::~ExtractScope() {}

void ExtractScope::Execute()
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
}  // namespace selector

