#include "rotate.h"

#include <pagoda/graph/execution_argument_callback.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
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
using namespace dynamic;

const std::string Rotate::s_inputGeometry("in");
const std::string Rotate::s_outputGeometry("out");

Rotate::Rotate(ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
}

Rotate::~Rotate()
{
}

void Rotate::SetParameters(graph::ExecutionArgumentCallback* cb)
{
	RegisterMember("x", cb->FloatArgument("x", "X", 0.0f));
	RegisterMember("y", cb->FloatArgument("y", "Y", 0.0f));
	RegisterMember("z", cb->FloatArgument("z", "Z", 0.0f));
	RegisterMember("rotation_order",
	               cb->StringArgument("rotation_order", "Rotation Order", "xyz"));
	RegisterMember("world", cb->BooleanArgument("world", "World", false));
}

const std::string& Rotate::GetOperationName() const
{
	static const std::string sName{"Rotate"};
	return sName;
}

void Rotate::Interfaces(InterfaceCallback* cb)
{
	cb->InputInterface(m_input, s_inputGeometry, "In", Interface::Arity::Many);
	cb->OutputInterface(m_output, s_outputGeometry, "Out",
	                    Interface::Arity::Many);
}

void Rotate::DoWork()
{
	START_PROFILE;

	auto geometrySystem =
	  m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	ProceduralObjectPtr inObject = m_input->GetNext();
	ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject);
	m_output->SetNext(outObject);

	auto inGeometryComponent =
	  geometrySystem->GetComponentAs<GeometryComponent>(inObject);
	GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
	auto outGeometryComponent =
	  geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
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
	if (world) {
		auto rot = inScope.GetRotation();
		boost::qvm::col<0>(matrix) = XYZ0(boost::qvm::col<0>(rot));
		boost::qvm::col<1>(matrix) = XYZ0(boost::qvm::col<1>(rot));
		boost::qvm::col<2>(matrix) = XYZ0(boost::qvm::col<2>(rot));
		boost::qvm::col<3>(matrix) = Vec4F{0, 0, 0, 1};
	}

	for (std::size_t i = rotationOrder.size(); i > 0; --i) {
		char order = rotationOrder[i - 1];
		switch (order) {
			case 'x':
				matrix =
				  matrix * boost::qvm::rotx_mat<4>(static_cast<float>(Radians(x)));
				break;
			case 'y':
				matrix =
				  matrix * boost::qvm::roty_mat<4>(static_cast<float>(Radians(y)));
				break;
			case 'z':
				matrix =
				  matrix * boost::qvm::rotz_mat<4>(static_cast<float>(Radians(z)));
				break;
			default:
				throw common::exception::Exception("Invalid rotation order " +
				                                   std::string(1, order));
		}
	}

	if (world) {
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
	outGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(
	  outGeometry, rot * inScope.GetRotation()));
}  // namespace pagoda
}  // namespace pagoda::geometry::operations
