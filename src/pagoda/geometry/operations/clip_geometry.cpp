#include "clip_geometry.h"

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>
#include <pagoda/objects/hierarchical_component.h>
#include <pagoda/objects/hierarchical_system.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic_value/float_value.h>
#include <pagoda/dynamic_value/get_value_as.h>
#include <pagoda/geometry/algorithms/clip.h>

namespace pagoda::geometry::operations
{
using namespace objects;
using namespace math;
using namespace geometry::core;
using namespace geometry::algorithms;

const std::string ClipGeometry::inputGeometry("in");
const std::string ClipGeometry::frontGeometry("front");
const std::string ClipGeometry::backGeometry("back");
const char* ClipGeometry::name = "ClipGeometry";

ClipGeometry::ClipGeometry(objects::ProceduralObjectSystemPtr objectSystem) : objects::ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(inputGeometry);
	CreateOutputInterface(frontGeometry);
	CreateOutputInterface(backGeometry);

	RegisterValues({{"plane", std::make_shared<DynamicPlane>()}});
}

ClipGeometry::~ClipGeometry() {}

void ClipGeometry::DoWork()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	while (HasInput(inputGeometry))
	{
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);
		UpdateValue("plane");

		Clip<Geometry> clip(get_value_as<Plane<float>>(*GetValue("plane")));

		// Geometry
		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();

		auto frontProceduralObject = CreateOutputProceduralObject(frontGeometry);
		auto frontGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(frontProceduralObject);
		auto front = std::make_shared<Geometry>();
		frontGeometryComponent->SetGeometry(front);

		auto backProceduralObject = CreateOutputProceduralObject(backGeometry);
		auto backGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(backProceduralObject);
		auto back = std::make_shared<Geometry>();
		backGeometryComponent->SetGeometry(back);

		clip.Execute(inGeometry, front, back);

		frontGeometryComponent->SetScope(
		    Scope::FromGeometryAndConstrainedRotation(front, inGeometryComponent->GetScope().GetRotation()));
		backGeometryComponent->SetScope(
		    Scope::FromGeometryAndConstrainedRotation(back, inGeometryComponent->GetScope().GetRotation()));

		auto parentHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		for (const auto& object : {frontProceduralObject, backProceduralObject})
		{
			auto hierarchicalComponent = hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(object);
			hierarchicalSystem->SetParent(hierarchicalComponent, parentHierarchicalComponent);
		}
	}
}
}  // namespace pagoda::geometry::operations
