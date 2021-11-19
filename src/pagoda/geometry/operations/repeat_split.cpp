#include "repeat_split.h"

#include <pagoda/objects/parameter_callback.h>

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

RepeatSplit::RepeatSplit(ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
	START_PROFILE;
}

RepeatSplit::~RepeatSplit()
{
}

void RepeatSplit::Parameters(objects::NewParameterCallback* cb)
{
	cb->FloatParameter(&m_size, "size", "Size", 1.0f);
	cb->StringParameter(&m_axis, "axis", "Axis", "x");
	cb->BooleanParameter(&m_adjust, "adjust", "Adjust", false);
}

const std::string& RepeatSplit::GetOperationName() const
{
	static const std::string sName{"RepeatSplit"};
	return sName;
}

void RepeatSplit::Interfaces(InterfaceCallback* cb)
{
	cb->InputInterface(m_input, inputGeometry, "In", Interface::Arity::Many);
	cb->OutputInterface(m_output, outputGeometry, "Out", Interface::Arity::All);
}

void RepeatSplit::DoWork()
{
	START_PROFILE;

	auto geometrySystem =
	  m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	ProceduralObjectPtr inObject = m_input->GetNext();

	auto inGeometryComponent =
	  geometrySystem->GetComponentAs<GeometryComponent>(inObject);
	GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
	auto inScope = inGeometryComponent->GetScope();

	PlaneSplits<Geometry> planeSplit(
	  CreatePlanes(inScope, m_size, m_axis, m_adjust));

	std::vector<GeometryPtr> splitGeometries;

	planeSplit.Execute(inGeometry, splitGeometries);

	int32_t createdObjectCount = 1;
	for (auto& g : splitGeometries) {
		auto outProceduralObject = CreateOutputProceduralObject(inObject);
		outProceduralObject->RegisterOrSetMember(
		  "index", std::make_shared<Integer>(createdObjectCount++));
		auto outGeometryComponent =
		  geometrySystem->CreateComponentAs<GeometryComponent>(outProceduralObject);
		m_output->Add(outProceduralObject);

		outGeometryComponent->SetGeometry(g);
		outGeometryComponent->SetScope(
		  Scope::FromGeometryAndConstrainedRotation(g, inScope.GetRotation()));
	}
}

std::vector<Plane<float>> RepeatSplit::CreatePlanes(const Scope& scope,
                                                    const float& size,
                                                    const std::string& axis,
                                                    bool adjust)
{
	CRITICAL_ASSERT_MSG(axis == "x" || axis == "y" || axis == "z",
	                    "Axis must be one of x, y, or z");

	LOG_TRACE(GeometryOperations, "Creating repeat split planes.");
	LOG_TRACE(GeometryOperations,
	          " size: " << size << " , axis: " << axis << ", adjust: " << adjust);

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

	LOG_TRACE(GeometryOperations,
	          " scopeAxis: " << scopeAxis << ", planeDistance: " << planeDistance
	                         << ", numPlanes: " << numPlanes);
	for (uint32_t i = 0; i < numPlanes; ++i) {
		auto plane = Plane<float>::FromPointAndNormal(currentPoint, scopeAxis);
		LOG_TRACE(GeometryOperations, " Split Plane: " << plane);
		planes.push_back(plane);
		currentPoint = currentPoint + translationVector;
	}

	return planes;
}
}  // namespace pagoda::geometry::operations
