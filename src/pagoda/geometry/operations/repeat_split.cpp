#include "repeat_split.h"

#include <pagoda/procedural_objects/geometry_component.h>
#include <pagoda/procedural_objects/geometry_system.h>
#include <pagoda/procedural_objects/hierarchical_component.h>
#include <pagoda/procedural_objects/hierarchical_system.h>
#include <pagoda/procedural_objects/procedural_object_system.h>

#include <pagoda/dynamic_value/boolean_value.h>
#include <pagoda/dynamic_value/float_value.h>
#include <pagoda/dynamic_value/get_value_as.h>
#include <pagoda/dynamic_value/integer_value.h>
#include <pagoda/dynamic_value/string_value.h>
#include <pagoda/geometry/algorithms/plane_splits.h>

namespace pagoda::geometry::operations
{
using namespace math;
using namespace core;
using namespace algorithms;

const std::string RepeatSplit::inputGeometry("in");
const std::string RepeatSplit::outputGeometry("out");
const char* RepeatSplit::name = "RepeatSplit";

RepeatSplit::RepeatSplit(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(inputGeometry);
	CreateOutputInterface(outputGeometry);

	RegisterValues({{"size", std::make_shared<FloatValue>(0.0f)},
	                {"axis", std::make_shared<String>("x")},
	                {"adjust", std::make_shared<Boolean>(false)}});
}

RepeatSplit::~RepeatSplit() {}

void RepeatSplit::DoWork()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	while (HasInput(inputGeometry))
	{
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);

		UpdateValue("size");
		UpdateValue("axis");
		UpdateValue("adjust");

		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto inHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		auto inScope = inGeometryComponent->GetScope();

		auto size = get_value_as<float>(*GetValue("size"));
		auto axis = get_value_as<std::string>(*GetValue("axis"));
		auto adjust = get_value_as<std::string>(*GetValue("adjust"));
		PlaneSplits<Geometry> planeSplit(CreatePlanes(inScope, size, axis, adjust == "true"));

		std::vector<GeometryPtr> splitGeometries;

		planeSplit.Execute(inGeometry, splitGeometries);

		int32_t createdObjectCount = 1;
		for (auto& g : splitGeometries)
		{
			auto outProceduralObject = CreateOutputProceduralObject(outputGeometry);
			outProceduralObject->RegisterOrSetMember("index", std::make_shared<Integer>(createdObjectCount++));
			auto outGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(outProceduralObject);
			auto outHierarchicalComponent =
			    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(outProceduralObject);

			outGeometryComponent->SetGeometry(g);
			outGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(g, inScope.GetRotation()));
			hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
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
	if (axis == "x")
	{
		scopePlane = scope.GetYZPlane();
		scopeSize = X(scope.GetSize());
	}
	else if (axis == "y")
	{
		scopePlane = scope.GetXZPlane();
		scopeSize = Y(scope.GetSize());
	}
	else
	{
		scopePlane = scope.GetXYPlane();
		scopeSize = Z(scope.GetSize());
	}

	std::vector<Plane<float>> planes;
	float planeDistance = size;
	uint32_t numPlanes = scopeSize / planeDistance - 1;
	if (adjust)
	{
		planeDistance = scopeSize / numPlanes;
	}

	auto translationVector = scopeAxis * planeDistance;
	scopeAxis = -1 * scopeAxis;
	auto currentPoint = scopePlane.GetPoint() + translationVector;

	LOG_TRACE(ProceduralObjects,
	          " scopeAxis: " << scopeAxis << ", planeDistance: " << planeDistance << ", numPlanes: " << numPlanes);
	for (uint32_t i = 0; i < numPlanes; ++i)
	{
		auto plane = Plane<float>::FromPointAndNormal(currentPoint, scopeAxis);
		LOG_TRACE(ProceduralObjects, " Split Plane: " << plane);
		planes.push_back(plane);
		currentPoint = currentPoint + translationVector;
	}

	return planes;
}
}  // namespace pagoda::geometry::operations
