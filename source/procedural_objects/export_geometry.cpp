#include "export_geometry.h"

#include "geometry_core/geometry_exporter.h"

#include "geometry_component.h"
#include "hierarchical_component.h"

#include <fstream>

namespace selector
{
const char* ExportGeometry::name = "ExportGeometry";
const InterfaceName ExportGeometry::inputGeometry = InterfaceName("in", 0);

ExportGeometry::ExportGeometry()
{
	ProceduralObjectMask inGeometryMask;
	inGeometryMask.set(static_cast<uint32_t>(GeometryComponent::GetType()));
	inGeometryMask.set(static_cast<uint32_t>(HierarchicalComponent::GetType()));
	CreateInputInterface(inputGeometry, inGeometryMask);
}

ExportGeometry::~ExportGeometry() {}

void ExportGeometry::Execute()
{
	START_PROFILE;

	uint32_t objectCount = 0;

	while (HasInput(inputGeometry))
	{
		m_parameterContext->SetParameter("count", objectCount);

		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);
		execution_context->parameter_context->UpdateExpressions();

		auto geometryComponent = inObject->GetComponent<GeometryComponent>();
		auto geometry = geometryComponent->GetGeometry();
		selector::ObjExporter<Geometry> exporter(geometry);

		std::string outputPath = get_parameter_as<std::string>(execution_context->parameter_context->GetParameter("path"));
		std::ofstream out_file(outputPath.c_str());
		exporter.Export(out_file);
		out_file.close();

		++objectCount;
	}
}

}  // namespace selector
