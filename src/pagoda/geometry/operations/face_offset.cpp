#include "face_offset.h"

#include <pagoda/procedural_objects/geometry_component.h>
#include <pagoda/procedural_objects/geometry_system.h>
#include <pagoda/procedural_objects/hierarchical_component.h>
#include <pagoda/procedural_objects/hierarchical_system.h>
#include <pagoda/procedural_objects/procedural_component.h>
#include <pagoda/procedural_objects/procedural_object_system.h>

#include <pagoda/dynamic_value/float_value.h>
#include <pagoda/dynamic_value/get_value_as.h>
#include <pagoda/geometry/algorithms/face_offset.h>

#include <memory>

namespace pagoda::geometry::operations
{
using namespace geometry::core;
using namespace geometry::algorithms;

const char* FaceOffsetOperation::name = "FaceOffsetOperation";
const std::string FaceOffsetOperation::inputGeometry("in");
const std::string FaceOffsetOperation::outputInnerGeometry("inner");
const std::string FaceOffsetOperation::outputOuterGeometry("outer");

FaceOffsetOperation::FaceOffsetOperation(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(inputGeometry);
	CreateOutputInterface(outputInnerGeometry);
	CreateOutputInterface(outputOuterGeometry);

	RegisterValues({{"amount", std::make_shared<FloatValue>(0.0f)}});
}

FaceOffsetOperation::~FaceOffsetOperation() {}

void FaceOffsetOperation::DoWork()
{
	START_PROFILE;
	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	while (HasInput(inputGeometry))
	{
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);
		UpdateValue("amount");

		float amount = get_value_as<float>(*GetValue("amount"));
		FaceOffset<Geometry> offset(amount);

		std::vector<GeometryPtr> innerGeometries;
		std::vector<GeometryPtr> outerGeometries;

		std::shared_ptr<GeometryComponent> inGeometryComponent =
		    geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		std::shared_ptr<HierarchicalComponent> inHierarchicalComponent =
		    hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();

		offset.Execute(inGeometry, std::back_inserter(innerGeometries), std::back_inserter(outerGeometries));

		for (const auto& i : innerGeometries)
		{
			ProceduralObjectPtr outObject = CreateOutputProceduralObject(outputInnerGeometry);
			outObject->RegisterOrSetMember("offset_tag", std::make_shared<String>("inner"));

			std::shared_ptr<GeometryComponent> geometryComponent =
			    geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
			geometryComponent->SetGeometry(i);
			geometryComponent->SetScope(
			    Scope::FromGeometryAndConstrainedRotation(i, inGeometryComponent->GetScope().GetRotation()));

			std::shared_ptr<HierarchicalComponent> outHierarchicalComponent =
			    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(outObject);

			hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
		}

		for (const auto& i : outerGeometries)
		{
			ProceduralObjectPtr outObject = CreateOutputProceduralObject(outputOuterGeometry);
			outObject->RegisterOrSetMember("offset_tag", std::make_shared<String>("outer"));

			std::shared_ptr<GeometryComponent> geometryComponent =
			    geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
			geometryComponent->SetGeometry(i);
			geometryComponent->SetScope(Scope::FromGeometry(i));

			std::shared_ptr<HierarchicalComponent> outHierarchicalComponent =
			    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(outObject);

			hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
		}
	}
}
}  // namespace pagoda::geometry::operations

