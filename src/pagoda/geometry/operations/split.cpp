#include "split.h"

#include <pagoda/graph/execution_argument_callback.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/string_value.h>
#include <pagoda/geometry/algorithms/plane_splits.h>

namespace pagoda::geometry::operations
{
using namespace math;
using namespace objects;
using namespace geometry::algorithms;
using namespace geometry::core;
using namespace dynamic;

const std::string Split::s_inputGeometry("in");

Split::Split(ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
}

Split::~Split()
{
}

void Split::SetParameters(graph::ExecutionArgumentCallback *cb)
{
	RegisterMember("axis", cb->StringArgument("axis", "Axis", "x"));
	RegisterMember("split_count",
	               cb->IntegerArgument("split_count", "Split Count", 0));
	for (uint32_t i = 1u; i <= 8; ++i) {
		auto name = "split_" + std::to_string(i);
		RegisterMember(
		  name, cb->FloatArgument(name.c_str(),
		                          ("Split " + std::to_string(i)).c_str(), 0));
	}
}

const std::string &Split::GetOperationName() const
{
	static const std::string sName{"Split"};
	return sName;
}

void Split::Interfaces(InterfaceCallback *cb)
{
	cb->InputInterface(m_input, s_inputGeometry, "In", Interface::Arity::Many);
	for (uint32_t i = 1u; i <= 8; ++i) {
		auto name = "split_" + std::to_string(i);
		cb->OutputInterface(m_outputs[i - 1], "split_" + std::to_string(i),
		                    "Split " + std::to_string(i), Interface::Arity::Many);
	}
}

std::vector<Plane<float>> createPlanes(const Scope &scope,
                                       const std::vector<float> &sizes,
                                       const std::string &axis)
{
	CRITICAL_ASSERT_MSG(axis == "x" || axis == "y" || axis == "z",
	                    "Axis must be one of x, y, or z");

	LOG_TRACE(ProceduralObjects, "Creating split planes.");
	LOG_TRACE(ProceduralObjects, "axis: " << axis);
#ifdef DEBUG
	for (const auto &s : sizes) {
		LOG_TRACE(ProceduralObjects, " size: " << s);
	}
#endif

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

	float currentDistance = sizes.front();
	auto translationVector = scopeAxis;
	scopeAxis = -1 * scopeAxis;

	for (auto i = 1u; i < sizes.size() && currentDistance < scopeSize; ++i) {
		auto currentPoint =
		  scopePlane.GetPoint() + currentDistance * translationVector;
		planes.push_back(Plane<float>::FromPointAndNormal(currentPoint, scopeAxis));
		currentDistance += sizes[i];
	}

	return planes;
}

void Split::DoWork()
{
	START_PROFILE;

	auto geometrySystem =
	  m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	auto axis = get_value_as<std::string>(*GetValue("axis"));
	auto splitCount = get_value_as<int>(*GetValue("split_count"));

	std::vector<std::string> outInterfaces;
	for (auto i = 1; i <= splitCount; ++i) {
		std::string outInterface("split_" + std::to_string(i));
		//CreateOutputInterface(outInterface);
		outInterfaces.push_back(outInterface);
	}

	ProceduralObjectPtr inObject = m_input->GetNext();
	auto inGeometryComponent =
	  geometrySystem->GetComponentAs<GeometryComponent>(inObject);
	GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
	auto inScope = inGeometryComponent->GetScope();

	std::vector<float> sizes;
	sizes.reserve(splitCount);
	for (auto i = 1; i <= splitCount; ++i) {
		std::string splitSizeName = "split_" + std::to_string(i);
		UpdateValue(splitSizeName);
		sizes.push_back(get_value_as<float>(*GetValue(splitSizeName)));
	}

	PlaneSplits<Geometry> planeSplit(createPlanes(inScope, sizes, axis));
	std::vector<GeometryPtr> splitGeometries;
	planeSplit.Execute(inGeometry, splitGeometries);

	for (auto i = 0u;
	     i < static_cast<uint32_t>(splitCount) && i < splitGeometries.size();
	     ++i) {
		auto outProceduralObject = CreateOutputProceduralObject(inObject);
		m_outputs[i]->SetNext(outProceduralObject);
		auto outGeometryComponent =
		  geometrySystem->CreateComponentAs<GeometryComponent>(outProceduralObject);

		outGeometryComponent->SetGeometry(splitGeometries[i]);
		outGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(
		  splitGeometries[i], inScope.GetRotation()));
	}
}
}  // namespace pagoda::geometry::operations
