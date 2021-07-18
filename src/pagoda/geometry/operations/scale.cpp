#include "scale.h"

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
#include <pagoda/math/matrix_base.h>

#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/mat_operations.hpp>

namespace pagoda::geometry::operations
{
using namespace objects;
using namespace math;
using namespace geometry::algorithms;
using namespace geometry::core;
using namespace dynamic;

const std::string Scale::s_inputGeometry("in");
const std::string Scale::s_outputGeometry("out");

Scale::Scale(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	CreateInputInterface(s_inputGeometry);
	CreateOutputInterface(s_outputGeometry);
}
Scale::~Scale() {}

void Scale::SetParameters(graph::ExecutionArgumentCallback* cb)
{
	RegisterMember("x", cb->FloatArgument("x", "X", 0.0f));
	RegisterMember("y", cb->FloatArgument("y", "Y", 0.0f));
	RegisterMember("z", cb->FloatArgument("z", "Z", 0.0f));
	RegisterMember("pivotal_point", cb->StringArgument("pivotal_point", "Pivotal Point", "scope_center"));
}

const std::string& Scale::GetOperationName() const
{
	static const std::string sName{"Scale"};
	return sName;
}

void Scale::Interfaces(InterfaceCallback* cb)
{
	cb->InputInterface(m_input, s_inputGeometry, "In", Interface::Arity::Many);
  cb->OutputInterface(m_output, s_outputGeometry, "Out", Interface::Arity::Many);
}

void Scale::DoWork()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	while (HasInput(s_inputGeometry)) {
		ProceduralObjectPtr inObject = GetInputProceduralObject(s_inputGeometry);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject, s_outputGeometry);

		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto outGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
		auto outGeometry = std::make_shared<Geometry>();
		outGeometryComponent->SetGeometry(outGeometry);

		auto inScope = inGeometryComponent->GetScope();
		UpdateValue("x");
		UpdateValue("y");
		UpdateValue("z");
		UpdateValue("pivotal_point");

		auto x = get_value_as<float>(*GetValue("x"));
		auto y = get_value_as<float>(*GetValue("y"));
		auto z = get_value_as<float>(*GetValue("z"));
		auto pivotalPointName = get_value_as<std::string>(*GetValue("pivotal_point"));

		Mat4x4F matrix;
		if (pivotalPointName == "scope_center") {
			Vec3F pivotalPoint = inScope.GetCenterPointInWorld();
			Mat4x4F translation = boost::qvm::translation_mat(XYZ(pivotalPoint));
			Mat4x4F scale = boost::qvm::diag_mat(XYZ1(Vec3F{x, y, z}));
			Mat4x4F invTranslation = boost::qvm::translation_mat(XYZ(-pivotalPoint));
			matrix = translation * scale * invTranslation;
		} else if (pivotalPointName == "scope_origin") {
			Vec3F pivotalPoint = inScope.GetWorldPoint(Scope::BoxPoints::LowerBottomLeft);
			matrix = boost::qvm::translation_mat(pivotalPoint) * boost::qvm::diag_mat(XYZ1(Vec3F{x, y, z})) *
			         boost::qvm::translation_mat(-pivotalPoint);
		} else if (pivotalPointName == "world_origin") {
			matrix = boost::qvm::diag_mat(XYZ1(Vec3F{x, y, z}));
		}

		MatrixTransform<Geometry> transform(matrix);

		transform.Execute(inGeometry, outGeometry);

		outGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(outGeometry, inScope.GetRotation()));
	}
}
}  // namespace pagoda::geometry::operations
