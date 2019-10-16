#include "repeat_split.h"

#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "procedural_object_system.h"

#include "geometry_operations/plane_splits.h"

namespace selector
{
const InterfaceName RepeatSplit::inputGeometry("in", 0);
const InterfaceName RepeatSplit::outputGeometry("out", 0);
const char* RepeatSplit::name = "RepeatSplit";

RepeatSplit::RepeatSplit(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(inputGeometry);
	CreateOutputInterface(outputGeometry);

	SetParameter("size", 0.0f);
	SetParameter("axis", "x");
	SetParameter("adjust", false);
}

RepeatSplit::~RepeatSplit() {}

void RepeatSplit::Execute()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	while (HasInput(inputGeometry))
	{
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);
		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto inHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		auto inScope = inGeometryComponent->GetScope();

		auto size = GetParameterAs<float>("size");
		auto axis = GetParameterAs<std::string>("axis");
		auto adjust = GetParameterAs<bool>("adjust");
		PlaneSplits<Geometry> planeSplit(CreatePlanes(inScope, size, axis, adjust));

		std::vector<GeometryPtr> splitGeometries;

		planeSplit.Execute(inGeometry, splitGeometries);

		for (auto& g : splitGeometries)
		{
			auto outProceduralObject = CreateOutputProceduralObject(outputGeometry);
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
	LOG_TRACE(ProceduralObjects, " size: %f, axis: %s, adjust: %d", size, axis.c_str(), adjust);

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
	float planeDistance = size;
	uint32_t numPlanes = scopeSize / planeDistance - 1;
	if (adjust)
	{
		planeDistance = scopeSize / numPlanes;
	}

	auto translationVector = scopeAxis * planeDistance;
	scopeAxis = -1 * scopeAxis;
	auto currentPoint = scopePlane.GetPoint() + translationVector;

	LOG_TRACE(ProceduralObjects, " scopeAxis: %s, planeDistance: %f, numPlanes: %d", to_string(scopeAxis).c_str(),
	          planeDistance, numPlanes);
	for (uint32_t i = 0; i < numPlanes; ++i)
	{
		auto plane = Plane<float>::FromPointAndNormal(currentPoint, scopeAxis);
		LOG_TRACE(ProceduralObjects, " Split Plane: %s", to_string(plane).c_str());
		planes.push_back(plane);
		currentPoint = currentPoint + translationVector;
	}

	return planes;
}
}  // namespace selector
