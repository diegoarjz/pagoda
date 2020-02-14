#include "rotate.h"

#include "math_lib/degrees.h"
#include "math_lib/radians.h"

#include "dynamic_value/boolean_value.h"
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
const std::string Rotate::s_inputGeometry("in");
const std::string Rotate::s_outputGeometry("out");

Rotate::Rotate(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	CreateInputInterface(s_inputGeometry);
	CreateOutputInterface(s_outputGeometry);

	RegisterValues({{"x", std::make_shared<FloatValue>(0.0f)},
	                {"y", std::make_shared<FloatValue>(0.0f)},
	                {"z", std::make_shared<FloatValue>(0.0f)},
	                {"rotation_order", std::make_shared<String>("xyz")},
	                {"world", std::make_shared<Boolean>(false)}});
}

Rotate::~Rotate() {}

void Rotate::DoWork()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	while (HasInput(s_inputGeometry))
	{
		ProceduralObjectPtr inObject = GetInputProceduralObject(s_inputGeometry);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(s_outputGeometry);

		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto outGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
		auto outGeometry = std::make_shared<Geometry>();
		outGeometryComponent->SetGeometry(outGeometry);

		auto inScope = inGeometryComponent->GetScope();
		UpdateValue("x");
		UpdateValue("y");
		UpdateValue("z");
		UpdateValue("rotation_order");
		UpdateValue("world");

		auto x = Degrees<float>(get_value_as<float>(*GetValue("x")));
		auto y = Degrees<float>(get_value_as<float>(*GetValue("y")));
		auto z = Degrees<float>(get_value_as<float>(*GetValue("z")));
		auto rotationOrder = get_value_as<std::string>(*GetValue("rotation_order"));
		auto world = get_value_as<std::string>(*GetValue("world")) == "true";

		Mat4x4F matrix(1);
		if (world)
		{
			matrix = Mat4x4F(inScope.GetRotation());
		}

		for (std::size_t i = rotationOrder.size(); i > 0; --i)
		{
			char order = rotationOrder[i - 1];
			switch (order)
			{
				case 'x':
					matrix = matrix * rotate_x_matrix(Radians(x));
					break;
				case 'y':
					matrix = matrix * rotate_y_matrix(Radians(y));
					break;
				case 'z':
					matrix = matrix * rotate_z_matrix(Radians(z));
					break;
				default:
					throw Exception("Invalid rotation order " + std::string(1, order));
			}
		}

		if (world)
		{
			matrix = matrix * Mat4x4F(inScope.GetInverseRotation());
		}

		MatrixTransform<Geometry> transform(matrix);
		transform.Execute(inGeometry, outGeometry);
		outGeometryComponent->SetScope(
		    Scope::FromGeometryAndConstrainedRotation(outGeometry, Mat3x3F(matrix) * inScope.GetRotation()));

		auto inHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		auto outHierarchicalComponent = hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(outObject);
		hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
	}
}
}  // namespace selector
