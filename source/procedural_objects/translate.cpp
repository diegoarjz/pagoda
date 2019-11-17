#include "translate.h"

#include "dynamic_value/float_value.h"
#include "dynamic_value/get_value_as.h"
#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "math_lib/matrix_base.h"
#include "procedural_object_system.h"

#include "geometry_operations/matrix_transform.h"

namespace selector
{
const InterfaceName Translate::s_inputGeometry("in", 0);
const InterfaceName Translate::s_outputGeometry("out", 0);

Translate::Translate(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	CreateInputInterface(s_inputGeometry);
	CreateOutputInterface(s_outputGeometry);

	RegisterValues({{"x", std::make_shared<FloatValue>(0.0f)},
	                {"y", std::make_shared<FloatValue>(0.0f)},
	                {"z", std::make_shared<FloatValue>(0.0f)}});
}
Translate::~Translate() {}

void Translate::DoWork()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	while (HasInput(s_inputGeometry))
	{
		ProceduralObjectPtr inObject = GetInputProceduralObject(s_inputGeometry);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(s_outputGeometry);

		UpdateValue("x");
		UpdateValue("y");
		UpdateValue("z");

		auto x = get_value_as<float>(*GetValue("x"));
		auto y = get_value_as<float>(*GetValue("y"));
		auto z = get_value_as<float>(*GetValue("z"));
		auto matrix = translate_matrix(x, y, z);
		MatrixTransform<Geometry> transform(matrix);

		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto outGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
		auto outGeometry = std::make_shared<Geometry>();
		outGeometryComponent->SetGeometry(outGeometry);
		transform.Execute(inGeometry, outGeometry);

		outGeometryComponent->SetScope(
		    Scope::FromGeometryAndConstrainedRotation(outGeometry, inGeometryComponent->GetScope().GetRotation()));

		auto inHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		auto outHierarchicalComponent = hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(outObject);
		hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
	}
}
}  // namespace selector
