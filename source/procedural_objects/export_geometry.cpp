#include "export_geometry.h"

#include "geometry_core/geometry_exporter.h"

#include "geometry_component.h"
#include "hierarchical_component.h"
#include "procedural_object_system.h"

#include <fstream>

namespace selector
{
const char* ExportGeometry::name = "ExportGeometry";
const InterfaceName ExportGeometry::inputGeometry = InterfaceName("in", 0);

ExportGeometry::ExportGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	CreateInputInterface(inputGeometry);

    SetParameter("path", "geometry");
}

ExportGeometry::~ExportGeometry() {}

void ExportGeometry::Execute()
{
	START_PROFILE;

	uint32_t objectCount = 0;
    auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	while (HasInput(inputGeometry))
	{
		m_parameterContext->SetParameter("count", static_cast<int>(objectCount));

		auto p = m_parameterContext->GetParameter("count");

		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);
		m_parameterContext->UpdateExpressions();

		auto geometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		auto geometry = geometryComponent->GetGeometry();
		selector::ObjExporter<Geometry> exporter(geometry);

		std::string outputPath = get_parameter_as<std::string>(GetParameter("path"));
		std::ofstream out_file(outputPath.c_str());
		exporter.Export(out_file);
		out_file.close();

		++objectCount;
	}
}

}  // namespace selector
