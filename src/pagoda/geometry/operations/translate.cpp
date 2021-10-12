#include "translate.h"

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

#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/map_vec_mat.hpp>

namespace pagoda::geometry::operations
{
using namespace objects;
using namespace math;
using namespace geometry::algorithms;
using namespace geometry::core;
using namespace dynamic;

const std::string Translate::s_inputGeometry("in");
const std::string Translate::s_outputGeometry("out");

Translate::Translate(ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
}
Translate::~Translate()
{
}

void Translate::SetParameters(objects::ParameterCallback* cb)
{
	RegisterMember("x", cb->FloatArgument("x", "X", 0.0f));
	RegisterMember("y", cb->FloatArgument("y", "Y", 0.0f));
	RegisterMember("z", cb->FloatArgument("z", "Z", 0.0f));
	RegisterMember("world", cb->BooleanArgument("world", "World", false));
}

const std::string& Translate::GetOperationName() const
{
	static const std::string sName{"Translate"};
	return sName;
}

void Translate::Interfaces(InterfaceCallback* cb)
{
	cb->InputInterface(m_input, s_inputGeometry, "In", Interface::Arity::Many);
	cb->OutputInterface(m_output, s_outputGeometry, "Out",
	                    Interface::Arity::Many);
}

void Translate::DoWork()
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
	UpdateValue("world");

	auto x = get_value_as<float>(*GetValue("x"));
	auto y = get_value_as<float>(*GetValue("y"));
	auto z = get_value_as<float>(*GetValue("z"));
	auto inWorldCoordinates =
	  get_value_as<std::string>(*GetValue("world")) == "true";
	Mat4x4F matrix;
	if (inWorldCoordinates) {
		matrix = boost::qvm::translation_mat(Vec3F{x, y, z});
	} else {
		matrix =
		  boost::qvm::translation_mat(inScope.GetLocalVector(Vec3F{x, y, z}));
	}
	MatrixTransform<Geometry> transform(matrix);

	transform.Execute(inGeometry, outGeometry);

	outGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(
	  outGeometry, inScope.GetRotation()));
}
}  // namespace pagoda::geometry::operations
