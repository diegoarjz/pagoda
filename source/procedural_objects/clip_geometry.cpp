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

	SetParameter("normal_x", 0.0f);
	SetParameter("normal_y", 0.0f);
	SetParameter("normal_z", 0.0f);
	SetParameter("position_x", 0.0f);
	SetParameter("position_y", 0.0f);
	SetParameter("position_z", 0.0f);
}

ClipGeometry::~ClipGeometry() {}

void ClipGeometry::Execute()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	while (HasInput(inputGeometry))
	{
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);
		// clang-format off
		Clip<Geometry> clip(
		    Plane<float>::FromPointAndNormal(
            {
                GetParameterAs<float>("position_x"),
                GetParameterAs<float>("position_y"),
                GetParameterAs<float>("position_z")
            },
            {
                GetParameterAs<float>("normal_x"),
                GetParameterAs<float>("normal_y"),
                GetParameterAs<float>("normal_z")
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

		auto parentHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		for (const auto& object : {frontProceduralObject, backProceduralObject})
		{
			auto hierarchicalComponent = hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(object);
			hierarchicalSystem->SetParent(hierarchicalComponent, parentHierarchicalComponent);
		}
	}
}
}  // namespace selector
