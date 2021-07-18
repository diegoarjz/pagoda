#include "face_offset.h"

#include <pagoda/graph/execution_argument_callback.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/geometry/algorithms/face_offset.h>

#include <memory>

namespace pagoda::geometry::operations
{
using namespace objects;
using namespace geometry::core;
using namespace geometry::algorithms;
using namespace dynamic;

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
}

FaceOffsetOperation::~FaceOffsetOperation() {}

void FaceOffsetOperation::SetParameters(graph::ExecutionArgumentCallback* cb)
{
	RegisterMember("amount", cb->FloatArgument("amount", "Amount", 0.0f));
}

const std::string& FaceOffsetOperation::GetOperationName() const
{
	static const std::string sName{"FaceOffset"};
	return sName;
}

void FaceOffsetOperation::Interfaces(InterfaceCallback* cb)
{
  cb->InputInterface(m_input, inputGeometry, "In", Interface::Arity::Many);
  cb->OutputInterface(m_inner, outputInnerGeometry, "Inner", Interface::Arity::Many);
  cb->OutputInterface(m_outer, outputOuterGeometry, "Outer", Interface::Arity::Many);
}

void FaceOffsetOperation::DoWork()
{
	START_PROFILE;
	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	while (HasInput(inputGeometry)) {
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);
		UpdateValue("amount");

		float amount = get_value_as<float>(*GetValue("amount"));
		FaceOffset<Geometry> offset(amount);

		std::vector<GeometryPtr> innerGeometries;
		std::vector<GeometryPtr> outerGeometries;

		std::shared_ptr<GeometryComponent> inGeometryComponent =
		  geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();

		offset.Execute(inGeometry, std::back_inserter(innerGeometries), std::back_inserter(outerGeometries));

		for (const auto& i : innerGeometries) {
			ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject, outputInnerGeometry);
			outObject->RegisterOrSetMember("offset_tag", std::make_shared<String>("inner"));

			std::shared_ptr<GeometryComponent> geometryComponent =
			  geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
			geometryComponent->SetGeometry(i);
			geometryComponent->SetScope(
			  Scope::FromGeometryAndConstrainedRotation(i, inGeometryComponent->GetScope().GetRotation()));
		}

		for (const auto& i : outerGeometries) {
			ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject, outputOuterGeometry);
			outObject->RegisterOrSetMember("offset_tag", std::make_shared<String>("outer"));

			std::shared_ptr<GeometryComponent> geometryComponent =
			  geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
			geometryComponent->SetGeometry(i);
			geometryComponent->SetScope(Scope::FromGeometry(i));
		}
	}
}
}  // namespace pagoda::geometry::operations
