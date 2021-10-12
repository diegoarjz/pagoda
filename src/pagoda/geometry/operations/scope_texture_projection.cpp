#include "scope_texture_projection.h"

#include <pagoda/objects/parameter_callback.h>

#include <pagoda/geometry/core/planar_texture_projection.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/string_value.h>

#include <memory>

using namespace boost;

namespace pagoda::geometry::operations
{
using namespace objects;
using namespace math;
using namespace geometry::core;
using namespace dynamic;

const std::string ScopeTextureProjection::s_inputGeometry("in");
const std::string ScopeTextureProjection::s_outputGeometry("out");

ScopeTextureProjection::ScopeTextureProjection(
  objects::ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
}

ScopeTextureProjection::~ScopeTextureProjection()
{
}

void ScopeTextureProjection::SetParameters(objects::ParameterCallback* cb)
{
	RegisterMember("axis", cb->StringArgument("axis", "Axis", "z"));
	RegisterMember("scale_u", cb->FloatArgument("scale_u", "Scale U", 1.0f));
	RegisterMember("scale_v", cb->FloatArgument("scale_v", "Scale V", 1.0f));
	RegisterMember("offset_u", cb->FloatArgument("offset_u", "Offset U", 0.0f));
	RegisterMember("offset_v", cb->FloatArgument("offset_v", "Offset V", 0.0f));
	RegisterMember("clamp", cb->BooleanArgument("clamp", "Clamp", false));
}

const std::string& ScopeTextureProjection::GetOperationName() const
{
	static const std::string sName{"ScopeTextureProjection"};
	return sName;
}

void ScopeTextureProjection::Interfaces(InterfaceCallback* cb)
{
	cb->InputInterface(m_input, s_inputGeometry, "In", Interface::Arity::Many);
	cb->OutputInterface(m_output, s_outputGeometry, "Out",
	                    Interface::Arity::Many);
}

void ScopeTextureProjection::DoWork()
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
	*outGeometry = *inGeometry;
	outGeometryComponent->SetGeometry(outGeometry);
	outGeometryComponent->SetScope(inGeometryComponent->GetScope());

	UpdateValue("axis");
	UpdateValue("scale_u");
	UpdateValue("scale_v");
	UpdateValue("offset_u");
	UpdateValue("offset_v");
	UpdateValue("clamp");

	auto& scope = outGeometryComponent->GetScope();
	auto scopeSize = scope.GetSize();
	qvm::vec<float, 3> scopePos = scope.GetPosition();
	qvm::vec<float, 3> uAxis;
	qvm::vec<float, 3> vAxis;
	float uScale = get_value_as<float>(*GetValue("scale_u"));
	float vScale = get_value_as<float>(*GetValue("scale_v"));

	std::string axis = get_value_as<std::string>(*GetValue("axis"));
	if (axis == "x") {
		uAxis = scope.GetAxis('y');
		vAxis = scope.GetAxis('z');
		uScale *= Y(scopeSize);
		vScale *= Z(scopeSize);
	} else if (axis == "y") {
		uAxis = scope.GetAxis('x');
		vAxis = scope.GetAxis('z');
		uScale *= X(scopeSize);
		vScale *= Z(scopeSize);
	} else if (axis == "z") {
		uAxis = scope.GetAxis('x');
		vAxis = scope.GetAxis('y');
		uScale *= X(scopeSize);
		vScale *= Y(scopeSize);
	}

	PlanarTextureProjection ptp(scopePos, uAxis, vAxis);
	ptp.SetScale(uScale, vScale);
	ptp.SetOffset(get_value_as<float>(*GetValue("offset_u")),
	              get_value_as<float>(*GetValue("offset_v")));
	ptp.SetClamp(get_value_as<bool>(*GetValue("clamp")));

	for (auto i = outGeometry->PointsBegin(); i != outGeometry->PointsEnd();
	     ++i) {
		outGeometry->GetVertexAttributes(*i).m_texCoords =
		  ptp.GetProjection(outGeometry->GetPosition(*i));
	}
}
}  // namespace pagoda::geometry::operations
