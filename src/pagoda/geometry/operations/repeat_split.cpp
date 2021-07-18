#include "repeat_split.h"

#include <pagoda/graph/execution_argument_callback.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/integer_value.h>
#include <pagoda/dynamic/string_value.h>
#include <pagoda/geometry/algorithms/plane_splits.h>

namespace pagoda::geometry::operations
{
using namespace objects;
using namespace math;
using namespace core;
using namespace algorithms;
using namespace dynamic;

const std::string RepeatSplit::inputGeometry("in");
const std::string RepeatSplit::outputGeometry("out");
const char* RepeatSplit::name = "RepeatSplit";

RepeatSplit::RepeatSplit(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(inputGeometry);
	CreateOutputInterface(outputGeometry);
}

RepeatSplit::~RepeatSplit() {}

void RepeatSplit::SetParameters(graph::ExecutionArgumentCallback* cb)
{
	RegisterMember("size", cb->FloatArgument("size", "Size", 1.0f));
	RegisterMember("axis", cb->StringArgument("axis", "Axis", "x"));
	RegisterMember("adjust", cb->BooleanArgument("adjust", "Adjust", false));
}

const std::string& RepeatSplit::GetOperationName() const
{
	static const std::string sName{"RepeatSplit"};
	return sName;
}

void RepeatSplit::Interfaces(InterfaceCallback* cb)
{
	cb->InputInterface(m_input, inputGeometry, "In", Interface::Arity::Many);
  cb->OutputInterface(m_output, outputGeometry, "Out", Interface::Arity::Many);
}

void RepeatSplit::DoWork()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	while (HasInput(inputGeometry)) {
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);

		UpdateValue("size");
		UpdateValue("axis");
		UpdateValue("adjust");

		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto inScope = inGeometryComponent->GetScope();

		auto size = get_value_as<float>(*GetValue("size"));
		auto axis = get_value_as<std::string>(*GetValue("axis"));
		auto adjust = get_value_as<std::string>(*GetValue("adjust"));
		PlaneSplits<Geometry> planeSplit(CreatePlanes(inScope, size, axis, adjust == "true"));

		std::vector<GeometryPtr> splitGeometries;

		planeSplit.Execute(inGeometry, splitGeometries);

		int32_t createdObjectCount = 1;
		for (auto& g : splitGeometries) {
			auto outProceduralObject = CreateOutputProceduralObject(inObject, outputGeometry);
			outProceduralObject->RegisterOrSetMember("index", std::make_shared<Integer>(createdObjectCount++));
			auto outGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(outProceduralObject);

			outGeometryComponent->SetGeometry(g);
			outGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(g, inScope.GetRotation()));
		}
	}
}

std::vector<Plane<float>> RepeatSplit::CreatePlanes(const Scope& scope, const float& size, const std::string& axis,
                                                    bool adjust)
{
	CRITICAL_ASSERT_MSG(axis == "x" || axis == "y" || axis == "z", "Axis must be one of x, y, or z");

	LOG_TRACE(ProceduralObjects, "Creating repeat split planes.");
	LOG_TRACE(ProceduralObjects, " size: " << size << " , axis: " << axis << ", adjust: " << adjust);

	auto scopeAxis = scope.GetAxis(axis);
	Plane<float> scopePlane;
	float scopeSize;
	if (axis == "x") {
		scopePlane = scope.GetYZPlane();
		scopeSize = X(scope.GetSize());
	} else if (axis == "y") {
		scopePlane = scope.GetXZPlane();
		scopeSize = Y(scope.GetSize());
	} else {
		scopePlane = scope.GetXYPlane();
		scopeSize = Z(scope.GetSize());
	}

	std::vector<Plane<float>> planes;
	float planeDistance = size;
	uint32_t numPlanes = scopeSize / planeDistance - 1;
	if (adjust) {
		planeDistance = scopeSize / numPlanes;
	}

	auto translationVector = scopeAxis * planeDistance;
	scopeAxis = -1 * scopeAxis;
	auto currentPoint = scopePlane.GetPoint() + translationVector;

	LOG_TRACE(ProceduralObjects,
	          " scopeAxis: " << scopeAxis << ", planeDistance: " << planeDistance << ", numPlanes: " << numPlanes);
	for (uint32_t i = 0; i < numPlanes; ++i) {
		auto plane = Plane<float>::FromPointAndNormal(currentPoint, scopeAxis);
		LOG_TRACE(ProceduralObjects, " Split Plane: " << plane);
		planes.push_back(plane);
		currentPoint = currentPoint + translationVector;
	}

	return planes;
}
}  // namespace pagoda::geometry::operations
