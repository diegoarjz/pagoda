#include "split.h"

#include "dynamic_value/boolean_value.h"
#include "dynamic_value/float_value.h"
#include "dynamic_value/get_value_as.h"
#include "dynamic_value/string_value.h"
#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "procedural_object_system.h"

#include "geometry_operations/plane_splits.h"

namespace selector
{
const InterfaceName Split::s_inputGeometry("in", 0);

Split::Split(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	CreateInputInterface(s_inputGeometry);

	std::unordered_map<std::string, DynamicValueBasePtr> values = {{"axis", std::make_shared<String>("x")},
	                                                               {"split_count", std::make_shared<Integer>(0)}};

	for (uint32_t i = 1u; i <= 32; ++i)
	{
		values.emplace("split_" + std::to_string(i), std::make_shared<Integer>(0));
	}

	RegisterValues(values);
}

Split::~Split() {}

std::vector<Plane<float>> createPlanes(const Scope &scope, const std::vector<float> &sizes, const std::string &axis)
{
	CRITICAL_ASSERT_MSG(axis == "x" || axis == "y" || axis == "z", "Axis must be one of x, y, or z");

	LOG_TRACE(ProceduralObjects, "Creating split planes.");
	LOG_TRACE(ProceduralObjects, "axis: " << axis);
#ifdef DEBUG
	for (const auto &s : sizes)
	{
		LOG_TRACE(ProceduralObjects, " size: " << s);
	}
#endif

	auto scopeAxis = scope.GetAxis(axis);
	Plane<float> scopePlane;
	float scopeSize;
	if (axis == "x")
	{
		scopePlane = scope.GetYZPlane();
		scopeSize = scope.GetSize()[0];
	}
	else if (axis == "y")
	{
		scopePlane = scope.GetXZPlane();
		scopeSize = scope.GetSize()[1];
	}
	else
	{
		scopePlane = scope.GetXYPlane();
		scopeSize = scope.GetSize()[2];
	}

	std::vector<Plane<float>> planes;

	float currentDistance = sizes.front();
	auto translationVector = scopeAxis;
	scopeAxis = -1 * scopeAxis;

	for (auto i = 1u; i < sizes.size() && currentDistance < scopeSize; ++i)
	{
		auto currentPoint = scopePlane.GetPoint() + currentDistance * translationVector;
		planes.push_back(Plane<float>::FromPointAndNormal(currentPoint, scopeAxis));
		currentDistance += sizes[i];
	}

	return planes;
}

void Split::DoWork()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	auto axis = get_value_as<std::string>(*GetValue("axis"));
	auto splitCount = get_value_as<int>(*GetValue("split_count"));

	std::vector<InterfaceName> outInterfaces;
	for (auto i = 1; i <= splitCount; ++i)
	{
		InterfaceName outInterface("split_" + std::to_string(i), 0);
		CreateOutputInterface(outInterface);
		outInterfaces.push_back(outInterface);
	}

	while (HasInput(s_inputGeometry))
	{
		ProceduralObjectPtr inObject = GetInputProceduralObject(s_inputGeometry);
		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto inHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		auto inScope = inGeometryComponent->GetScope();

		std::vector<float> sizes;
		sizes.reserve(splitCount);
		for (auto i = 1; i <= splitCount; ++i)
		{
			std::string splitSizeName = "split_" + std::to_string(i);
			UpdateValue(splitSizeName);
			sizes.push_back(get_value_as<float>(*GetValue(splitSizeName)));
		}

		PlaneSplits<Geometry> planeSplit(createPlanes(inScope, sizes, axis));
		std::vector<GeometryPtr> splitGeometries;
		planeSplit.Execute(inGeometry, splitGeometries);

		for (auto i = 0u; i < static_cast<uint32_t>(splitCount) && i < splitGeometries.size(); ++i)
		{
			auto outProceduralObject = CreateOutputProceduralObject(outInterfaces[i]);
			auto outGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(outProceduralObject);
			auto outHierarchicalComponent =
			    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(outProceduralObject);

			outGeometryComponent->SetGeometry(splitGeometries[i]);
			outGeometryComponent->SetScope(
			    Scope::FromGeometryAndConstrainedRotation(splitGeometries[i], inScope.GetRotation()));
			hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
		}
	}
}
}  // namespace selector
