#include "scale.h"

#include <pagoda/objects/parameter_callback.h>

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

Scale::Scale(ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
}
Scale::~Scale()
{
}

void Scale::Parameters(objects::NewParameterCallback* cb)
{
	cb->FloatParameter(&m_scaleX, "x", "X", 1.0f);
	cb->FloatParameter(&m_scaleY, "y", "Y", 1.0f);
	cb->FloatParameter(&m_scaleZ, "z", "Z", 1.0f);
	cb->StringParameter(&m_scaleCenter, "scale_center", "Scale Center",
	                    "scope_center");
}

const std::string& Scale::GetOperationName() const
{
	static const std::string sName{"Scale"};
	return sName;
}

void Scale::Interfaces(InterfaceCallback* cb)
{
	cb->InputInterface(m_input, s_inputGeometry, "In", Interface::Arity::Many);
	cb->OutputInterface(m_output, s_outputGeometry, "Out",
	                    Interface::Arity::Many);
}

void Scale::DoWork()
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

	Mat4x4F matrix;
	if (m_scaleCenter == "scope_center") {
		Vec3F pivotalPoint = inScope.GetCenterPointInWorld();
		Mat4x4F translation = boost::qvm::translation_mat(XYZ(pivotalPoint));
		Mat4x4F scale =
		  boost::qvm::diag_mat(XYZ1(Vec3F{m_scaleX, m_scaleY, m_scaleZ}));
		Mat4x4F invTranslation = boost::qvm::translation_mat(XYZ(-pivotalPoint));
		matrix = translation * scale * invTranslation;
	} else if (m_scaleCenter == "scope_origin") {
		Vec3F pivotalPoint =
		  inScope.GetWorldPoint(Scope::BoxPoints::LowerBottomLeft);
		matrix = boost::qvm::translation_mat(pivotalPoint) *
		         boost::qvm::diag_mat(XYZ1(Vec3F{m_scaleX, m_scaleY, m_scaleZ})) *
		         boost::qvm::translation_mat(-pivotalPoint);
	} else if (m_scaleCenter == "world_origin") {
		matrix = boost::qvm::diag_mat(XYZ1(Vec3F{m_scaleX, m_scaleY, m_scaleZ}));
	}

	MatrixTransform<Geometry> transform(matrix);

	transform.Execute(inGeometry, outGeometry);

	outGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(
	  outGeometry, inScope.GetRotation()));
}
}  // namespace pagoda::geometry::operations
