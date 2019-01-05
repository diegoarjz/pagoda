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
const InterfaceName TriangulateGeometry::sInputGeometry(0, 0);
const InterfaceName TriangulateGeometry::sOutputGeometry(0, 0);

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
		GeometrySizes sizes = triangulate.ResultSize(inGeometry);
		auto outGeometry = std::make_shared<Geometry>(sizes.m_numVertices, sizes.m_numEdges, sizes.m_numFaces);

		triangulate.Execute(inGeometry, outGeometry);
		outGeometryComponent->SetGeometry(outGeometry);

		// Hierarchy
		std::shared_ptr<HierarchicalComponent> inHierarchicalComponent =
		    inObject->GetComponent<HierarchicalComponent>();
		std::shared_ptr<HierarchicalComponent> outHierarchicalComponent =
		    outObject->GetComponent<HierarchicalComponent>();

		auto hierarchicalSystem = std::dynamic_pointer_cast<HierarchicalSystem>(
		    exection_context->procedural_object_system->GetComponentSystem(ComponentType::Hierarchical));
		hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
	}
}

std::shared_ptr<ParameterBase> TriangulateGeometry::GetParameter(const std::string& name) { return nullptr; }

}  // namespace selector
