#include "triangulate_geometry.h"

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/geometry/algorithms/ear_clipping.h>

#include <memory>

namespace pagoda::geometry::operations
{
using namespace objects;
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

void TriangulateGeometry::SetParameters(graph::ExecutionArgumentCallback* cb) {}

const std::string& TriangulateGeometry::GetOperationName() const
{
	static const std::string sName{"TriangulateGeometry"};
	return sName;
}

void TriangulateGeometry::DoWork()
{
	START_PROFILE;
	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	EarClipping<Geometry> earClipping;

	while (HasInput(sInputGeometry)) {
		// Geometry
		ProceduralObjectPtr inObject = GetInputProceduralObject(sInputGeometry);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject, sOutputGeometry);

		std::shared_ptr<GeometryComponent> inGeometryComponent =
		  geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		std::shared_ptr<GeometryComponent> outGeometryComponent =
		  geometrySystem->CreateComponentAs<GeometryComponent>(outObject);

		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto outGeometry = std::make_shared<Geometry>();

		earClipping.Execute(inGeometry, outGeometry);
		outGeometryComponent->SetGeometry(outGeometry);
		outGeometryComponent->SetScope(inGeometryComponent->GetScope());
	}
}

}  // namespace pagoda::geometry::operations
