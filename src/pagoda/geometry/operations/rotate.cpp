#include "rotate.h"

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/hierarchical_component.h>
#include <pagoda/objects/hierarchical_system.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic_value/boolean_value.h>
#include <pagoda/dynamic_value/float_value.h>
#include <pagoda/dynamic_value/get_value_as.h>
#include <pagoda/geometry/algorithms/matrix_transform.h>
#include <pagoda/math/degrees.h>
#include <pagoda/math/matrix_base.h>
#include <pagoda/math/radians.h>

#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/mat_operations.hpp>

namespace pagoda::geometry::operations
{
using namespace objects;
using namespace geometry::algorithms;
using namespace geometry::core;
using namespace math;

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

		Mat4x4F matrix(boost::qvm::diag_mat(Vec4F{1.0f, 1.0f, 1.0f, 1.0f}));
		if (world)
		{
			auto rot = inScope.GetRotation();
			boost::qvm::col<0>(matrix) = XYZ0(boost::qvm::col<0>(rot));
			boost::qvm::col<1>(matrix) = XYZ0(boost::qvm::col<1>(rot));
			boost::qvm::col<2>(matrix) = XYZ0(boost::qvm::col<2>(rot));
			boost::qvm::col<3>(matrix) = Vec4F{0, 0, 0, 1};
		}

		for (std::size_t i = rotationOrder.size(); i > 0; --i)
		{
			char order = rotationOrder[i - 1];
			switch (order)
			{
				case 'x':
					matrix = matrix * boost::qvm::rotx_mat<4>(static_cast<float>(Radians(x)));
					break;
				case 'y':
					matrix = matrix * boost::qvm::roty_mat<4>(static_cast<float>(Radians(y)));
					break;
				case 'z':
					matrix = matrix * boost::qvm::rotz_mat<4>(static_cast<float>(Radians(z)));
					break;
				default:
					throw common::exception::Exception("Invalid rotation order " + std::string(1, order));
			}
		}

		if (world)
		{
			auto rot = inScope.GetInverseRotation();
			Mat4x4F invRot;
			boost::qvm::col<0>(invRot) = XYZ0(boost::qvm::col<0>(rot));
			boost::qvm::col<1>(invRot) = XYZ0(boost::qvm::col<1>(rot));
			boost::qvm::col<2>(invRot) = XYZ0(boost::qvm::col<2>(rot));
			boost::qvm::col<3>(invRot) = Vec4F{0, 0, 0, 1};
			matrix = matrix * invRot;
		}

		MatrixTransform<Geometry> transform(matrix);
		transform.Execute(inGeometry, outGeometry);
		Mat3x3F rot;
		boost::qvm::col<0>(rot) = XYZ(boost::qvm::col<0>(matrix));
		boost::qvm::col<1>(rot) = XYZ(boost::qvm::col<1>(matrix));
		boost::qvm::col<2>(rot) = XYZ(boost::qvm::col<2>(matrix));
		outGeometryComponent->SetScope(
		    Scope::FromGeometryAndConstrainedRotation(outGeometry, rot * inScope.GetRotation()));

		auto inHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		auto outHierarchicalComponent = hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(outObject);
		hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
	}
}  // namespace pagoda
}  // namespace pagoda::geometry::operations
