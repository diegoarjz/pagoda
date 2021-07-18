#include "extract_scope.h"

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/geometry/algorithms/create_box.h>

namespace pagoda::geometry::operations
{
using namespace objects;
using namespace geometry::core;
using namespace geometry::algorithms;

const std::string ExtractScope::inputGeometry("in");
const std::string ExtractScope::outputGeometry("out");
const char* ExtractScope::name = "ExtractScope";

ExtractScope::ExtractScope(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(inputGeometry);
	CreateOutputInterface(outputGeometry);
}

ExtractScope::~ExtractScope() {}

void ExtractScope::SetParameters(graph::ExecutionArgumentCallback* cb) {}

const std::string& ExtractScope::GetOperationName() const
{
	static const std::string sName{"ExtractScope"};
	return sName;
}

void ExtractScope::Interfaces(InterfaceCallback* cb)
{
	cb->InputInterface(m_input, inputGeometry, "In", Interface::Arity::Many);
  cb->OutputInterface(m_output, outputGeometry, "Out", Interface::Arity::Many);
}

void ExtractScope::DoWork()
{
	START_PROFILE;

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	while (HasInput(inputGeometry)) {
		// Geometry
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject, outputGeometry);

		std::shared_ptr<GeometryComponent> inGeometryComponent =
		  geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		std::shared_ptr<GeometryComponent> outGeometryComponent =
		  geometrySystem->CreateComponentAs<GeometryComponent>(outObject);

		GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
		auto outGeometry = std::make_shared<Geometry>();

		CreateBox<Geometry> createBox(inGeometryComponent->GetScope());
		createBox.Execute(outGeometry);

		outGeometryComponent->SetGeometry(outGeometry);
		outGeometryComponent->SetScope(inGeometryComponent->GetScope());
	}
}
}  // namespace pagoda::geometry::operations
