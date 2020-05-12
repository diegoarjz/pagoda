#include "extract_faces.h"

#include <pagoda/common/instrument/profiler.h>
#include <pagoda/geometry/algorithms/explode_to_faces.h>

#include <pagoda/procedural_objects/geometry_component.h>
#include <pagoda/procedural_objects/geometry_system.h>
#include <pagoda/procedural_objects/hierarchical_component.h>
#include <pagoda/procedural_objects/hierarchical_system.h>
#include <pagoda/procedural_objects/procedural_object_system.h>

namespace pagoda::geometry::operations
{
using namespace math;
using namespace geometry::core;
using namespace geometry::algorithms;

const std::string ExtractFaces::s_inputGeometry("in");
const std::string ExtractFaces::s_outputGeometry("out");

ExtractFaces::ExtractFaces(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(s_inputGeometry);
	CreateOutputInterface(s_outputGeometry);
}

ExtractFaces::~ExtractFaces() {}

void ExtractFaces::DoWork()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	ExplodeToFaces<Geometry> explodeToFaces;
	while (HasInput(s_inputGeometry))
	{
		ProceduralObjectPtr inObject = GetInputProceduralObject(s_inputGeometry);

		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		auto inHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);

		auto inGeometry = inGeometryComponent->GetGeometry();
		std::vector<GeometryPtr> explodedFaces;
		explodeToFaces.Execute(inGeometry, explodedFaces);

		auto inScopeZAxis = inGeometryComponent->GetScope().GetZAxis();
		for (auto &g : explodedFaces)
		{
			auto faceNormal = g->GetFaceAttributes(*g->FacesBegin()).m_normal;

			auto outObject = CreateOutputProceduralObject(s_outputGeometry);
			auto outGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
			auto outHierarchicalComponent = hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(outObject);

			outGeometryComponent->SetGeometry(g);
			if (boost::qvm::dot(faceNormal, inScopeZAxis) == 0)
			{
				Mat3x3F constrainedRotation;
				auto xAxis = boost::qvm::normalized(boost::qvm::cross(inScopeZAxis, faceNormal));
				boost::qvm::col<0>(constrainedRotation) = xAxis;
				boost::qvm::col<1>(constrainedRotation) = inScopeZAxis;
				boost::qvm::col<2>(constrainedRotation) = faceNormal;
				outGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(g, constrainedRotation));
			}
			else
			{
				outGeometryComponent->SetScope(Scope::FromGeometry(g));
			}

			hierarchicalSystem->SetParent(outHierarchicalComponent, inHierarchicalComponent);
		}
	}
}
}  // namespace pagoda::geometry::operations
