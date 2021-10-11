#include "extract_faces.h"

#include <pagoda/geometry/algorithms/explode_to_faces.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/common/instrument/profiler.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_object_system.h>

namespace pagoda::geometry::operations
{
using namespace math;
using namespace objects;
using namespace geometry::core;
using namespace geometry::algorithms;

const std::string ExtractFaces::s_inputGeometry("in");
const std::string ExtractFaces::s_outputGeometry("out");

ExtractFaces::ExtractFaces(ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
	START_PROFILE;
}

ExtractFaces::~ExtractFaces()
{
}

void ExtractFaces::SetParameters(graph::ExecutionArgumentCallback* cb)
{
}

const std::string& ExtractFaces::GetOperationName() const
{
	static const std::string sName{"ExtractFaces"};
	return sName;
}

void ExtractFaces::Interfaces(InterfaceCallback* cb)
{
	cb->InputInterface(m_input, s_inputGeometry, "In", Interface::Arity::Many);
	cb->OutputInterface(m_output, s_outputGeometry, "Out", Interface::Arity::All);
}

void ExtractFaces::DoWork()
{
	START_PROFILE;

	auto geometrySystem =
	  m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	ExplodeToFaces<Geometry> explodeToFaces;
	ProceduralObjectPtr inObject = m_input->GetNext();

	auto inGeometryComponent =
	  geometrySystem->GetComponentAs<GeometryComponent>(inObject);

	auto inGeometry = inGeometryComponent->GetGeometry();
	std::vector<GeometryPtr> explodedFaces;
	explodeToFaces.Execute(inGeometry, explodedFaces);

	auto inScopeZAxis = inGeometryComponent->GetScope().GetZAxis();
	for (auto& g : explodedFaces) {
		auto faceNormal = g->GetFaceAttributes(*g->FacesBegin()).m_normal;

		auto outObject = CreateOutputProceduralObject(inObject);
		auto outGeometryComponent =
		  geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
		m_output->Add(outObject);

		outGeometryComponent->SetGeometry(g);
		if (boost::qvm::dot(faceNormal, inScopeZAxis) == 0) {
			Mat3x3F constrainedRotation;
			auto xAxis =
			  boost::qvm::normalized(boost::qvm::cross(inScopeZAxis, faceNormal));
			boost::qvm::col<0>(constrainedRotation) = xAxis;
			boost::qvm::col<1>(constrainedRotation) = inScopeZAxis;
			boost::qvm::col<2>(constrainedRotation) = faceNormal;
			outGeometryComponent->SetScope(
			  Scope::FromGeometryAndConstrainedRotation(g, constrainedRotation));
		} else {
			outGeometryComponent->SetScope(Scope::FromGeometry(g));
		}
	}
}
}  // namespace pagoda::geometry::operations
