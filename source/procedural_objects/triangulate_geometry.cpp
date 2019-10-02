#include "triangulate_geometry.h"

#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "procedural_component.h"

#include <geometry_operations/triangulate.h>

#include <memory>

namespace selector
{
const InterfaceName TriangulateGeometry::sInputGeometry("in", 0);
const InterfaceName TriangulateGeometry::sOutputGeometry("out", 0);
const char* TriangulateGeometry::name = "TriangulateGeometry";

TriangulateGeometry::TriangulateGeometry()
{
	START_PROFILE;

	ProceduralObjectMask inGeometryMask;
	inGeometryMask.set(static_cast<uint32_t>(GeometryComponent::GetType()));
	inGeometryMask.set(static_cast<uint32_t>(HierarchicalComponent::GetType()));
	CreateInputInterface(sInputGeometry, inGeometryMask);

	ProceduralObjectMask outGeometryMask;
	outGeometryMask.set(static_cast<uint32_t>(GeometryComponent::GetType()));
	outGeometryMask.set(static_cast<uint32_t>(HierarchicalComponent::GetType()));
	CreateOutputInterface(sOutputGeometry, outGeometryMask);
}

TriangulateGeometry::~TriangulateGeometry() {}

void TriangulateGeometry::Execute()
{
	START_PROFILE;

	Triangulate<Geometry> triangulate;

	while (HasInput(sInputGeometry))
	{
		// Geometry
		ProceduralObjectPtr inObject = GetInputProceduralObject(sInputGeometry);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(sOutputGeometry);

		std::shared_ptr<GeometryComponent> inGeometryComponent = inObject->GetComponent<GeometryComponent>();
		std::shared_ptr<GeometryComponent> outGeometryComponent = outObject->GetComponent<GeometryComponent>();

		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto outGeometry = std::make_shared<Geometry>();

		triangulate.Execute(inGeometry, outGeometry);
		outGeometryComponent->SetGeometry(outGeometry);

		// Hierarchy
		std::shared_ptr<HierarchicalComponent> inHierarchicalComponent =
		    inObject->GetComponent<HierarchicalComponent>();
		std::shared_ptr<HierarchicalComponent> outHierarchicalComponent =
		    outObject->GetComponent<HierarchicalComponent>();

		auto hierarchicalSystem = std::dynamic_pointer_cast<HierarchicalSystem>(
		    execution_context->procedural_object_system->GetComponentSystem(ComponentType::Hierarchical));
		hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
	}
}

}  // namespace selector
