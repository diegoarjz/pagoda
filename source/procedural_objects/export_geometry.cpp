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

	RegisterMember("path", std::make_shared<String>("geometry.obj"));
}

ExportGeometry::~ExportGeometry() {}

void ExportGeometry::Execute()
{
	START_PROFILE;

	uint32_t objectCount = 0;
	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	while (HasInput(inputGeometry))
	{
		SetMember("count", std::make_shared<Integer>(static_cast<int>(objectCount)));

		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);

		auto geometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		auto geometry = geometryComponent->GetGeometry();
		selector::ObjExporter<Geometry> exporter(geometry);

		std::string outputPath = get_parameter_as<std::string>(GetMember("path"));
		std::ofstream out_file(outputPath.c_str());
		exporter.Export(out_file);
		out_file.close();

		++objectCount;
	}
}

}  // namespace selector
