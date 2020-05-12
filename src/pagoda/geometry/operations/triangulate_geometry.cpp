#include "triangulate_geometry.h"

#include <pagoda/procedural_objects/geometry_component.h>
#include <pagoda/procedural_objects/geometry_system.h>
#include <pagoda/procedural_objects/hierarchical_component.h>
#include <pagoda/procedural_objects/hierarchical_system.h>
#include <pagoda/procedural_objects/procedural_component.h>
#include <pagoda/procedural_objects/procedural_object_system.h>

#include <pagoda/geometry/algorithms/ear_clipping.h>

#include <memory>

namespace pagoda::geometry::operations
{
using namespace geometry::algorithms;
using namespace geometry::core;

const std::string TriangulateGeometry::sInputGeometry("in");
const std::string TriangulateGeometry::sOutputGeometry("out");
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

	EarClipping<Geometry> earClipping;

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

		earClipping.Execute(inGeometry, outGeometry);
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
