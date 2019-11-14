#include "triangulate_geometry.h"

#include "procedural_object_system.h"

#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "procedural_component.h"

#include "geometry_operations/triangulate.h"

#include "../selector.h"

#include <memory>

namespace selector
{
const InterfaceName TriangulateGeometry::sInputGeometry("in", 0);
const InterfaceName TriangulateGeometry::sOutputGeometry("out", 0);
const char* TriangulateGeometry::name = "TriangulateGeometry";

TriangulateGeometry::TriangulateGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(sInputGeometry);
	CreateOutputInterface(sOutputGeometry);
}

TriangulateGeometry::~TriangulateGeometry() {}

void TriangulateGeometry::DoWork()
{
	START_PROFILE;
	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	Triangulate<Geometry> triangulate;

	while (HasInput(sInputGeometry))
	{
		// Geometry
		ProceduralObjectPtr inObject = GetInputProceduralObject(sInputGeometry);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(sOutputGeometry);

		std::shared_ptr<GeometryComponent> inGeometryComponent =
		    geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		std::shared_ptr<GeometryComponent> outGeometryComponent =
		    geometrySystem->CreateComponentAs<GeometryComponent>(outObject);

		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto outGeometry = std::make_shared<Geometry>();

		triangulate.Execute(inGeometry, outGeometry);
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
