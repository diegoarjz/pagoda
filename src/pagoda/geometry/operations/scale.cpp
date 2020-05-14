#include "scale.h"

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/hierarchical_component.h>
#include <pagoda/objects/hierarchical_system.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic_value/boolean_value.h>
#include <pagoda/dynamic_value/float_value.h>
#include <pagoda/dynamic_value/get_value_as.h>
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

const std::string Scale::s_inputGeometry("in");
const std::string Scale::s_outputGeometry("out");

Scale::Scale(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	CreateInputInterface(s_inputGeometry);
	CreateOutputInterface(s_outputGeometry);

	RegisterValues({{"x", std::make_shared<FloatValue>(0.0f)},
	                {"y", std::make_shared<FloatValue>(0.0f)},
	                {"z", std::make_shared<FloatValue>(0.0f)},
	                {"pivotal_point", std::make_shared<String>("scope_center")}});
}
Scale::~Scale() {}

void Scale::DoWork()
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
		UpdateValue("pivotal_point");

		auto x = get_value_as<float>(*GetValue("x"));
		auto y = get_value_as<float>(*GetValue("y"));
		auto z = get_value_as<float>(*GetValue("z"));
		auto pivotalPointName = get_value_as<std::string>(*GetValue("pivotal_point"));

		Mat4x4F matrix;
		if (pivotalPointName == "scope_center")
		{
			Vec3F pivotalPoint = inScope.GetCenterPointInWorld();
			Mat4x4F translation = boost::qvm::translation_mat(XYZ(pivotalPoint));
			Mat4x4F scale = boost::qvm::diag_mat(XYZ1(Vec3F{x, y, z}));
			Mat4x4F invTranslation = boost::qvm::translation_mat(XYZ(-pivotalPoint));
			matrix = translation * scale * invTranslation;
		}
		else if (pivotalPointName == "scope_origin")
		{
			Vec3F pivotalPoint = inScope.GetWorldPoint(Scope::BoxPoints::LowerBottomLeft);
			matrix = boost::qvm::translation_mat(pivotalPoint) * boost::qvm::diag_mat(XYZ1(Vec3F{x, y, z})) *
			         boost::qvm::translation_mat(-pivotalPoint);
		}
		else if (pivotalPointName == "world_origin")
		{
			matrix = boost::qvm::diag_mat(XYZ1(Vec3F{x, y, z}));
		}

		MatrixTransform<Geometry> transform(matrix);

		transform.Execute(inGeometry, outGeometry);

		outGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(outGeometry, inScope.GetRotation()));

		auto inHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		auto outHierarchicalComponent = hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(outObject);
		hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
	}
}
}  // namespace pagoda::geometry::operations

