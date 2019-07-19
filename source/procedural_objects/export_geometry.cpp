#include "export_geometry.h"

#include "geometry_core/geometry_exporter.h"

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

ExportGeometry::~ExportGeometry()
{
}

void ExportGeometry::Execute()
{
	START_PROFILE;
	
	uint32_t objectCount = 0;
	
	while (HasInput())
	{
		m_parameterContext->SetParameter("count", objectCount);
		
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);
		m_parameterContext()->UpdateExpressions();
		
		auto geometryComponent = inObject->GetComponent<GeometryComponent>();
		auto geometry = geometryComponent->GetGeometry();
		selector::ObjExporter<Geometry> exporter(geometry);

		std::string outputPath = m_parameterContext->GetParameter("path");
		std::ofstream out_file(outputPath.c_str());
		exporter.Export(out_file);
		out_file.close();
		
		++objectCount;
	}
}

}