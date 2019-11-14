#include "clip_geometry.h"

#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "procedural_object_system.h"

#include "geometry_operations/clip.h"

namespace selector
{
const InterfaceName ClipGeometry::inputGeometry("in", 0);
const InterfaceName ClipGeometry::frontGeometry("front", 0);
const InterfaceName ClipGeometry::backGeometry("back", 0);
const char* ClipGeometry::name = "ClipGeometry";

ClipGeometry::ClipGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(inputGeometry);
	CreateOutputInterface(frontGeometry);
	CreateOutputInterface(backGeometry);

	RegisterValues({
	    {"normal_x", std::make_shared<FloatValue>(0.0f)},
	    {"normal_y", std::make_shared<FloatValue>(0.0f)},
	    {"normal_z", std::make_shared<FloatValue>(0.0f)},
	    {"position_x", std::make_shared<FloatValue>(0.0f)},
	    {"position_y", std::make_shared<FloatValue>(0.0f)},
	    {"position_z", std::make_shared<FloatValue>(0.0f)}
    });
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
		UpdateValue("position_x");
		UpdateValue("position_y");
		UpdateValue("position_z");
		UpdateValue("normal_x");
		UpdateValue("normal_y");
		UpdateValue("normal_z");

		// clang-format off
		Clip<Geometry> clip(
		    Plane<float>::FromPointAndNormal(
            {
                get_value_as<float>(*GetValue("position_x")),
                get_value_as<float>(*GetValue("position_y")),
                get_value_as<float>(*GetValue("position_z"))
            },
            {
                get_value_as<float>(*GetValue("normal_x")),
                get_value_as<float>(*GetValue("normal_y")),
                get_value_as<float>(*GetValue("normal_z"))
            }));
		// clang-format on

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
}  // namespace selector
