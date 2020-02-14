#include "export_geometry.h"

#include "dynamic_value/get_value_as.h"
#include "dynamic_value/set_value_from.h"
#include "geometry_core/geometry_exporter.h"

#include "geometry_component.h"
#include "hierarchical_component.h"
#include "procedural_object_system.h"

#include <fstream>

namespace selector
{
const char* ExportGeometry::name = "ExportGeometry";
const std::string ExportGeometry::inputGeometry("in");

ExportGeometry::ExportGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	CreateInputInterface(inputGeometry);

	RegisterValues({{"path", std::make_shared<String>("geometry.obj")}, {"count", std::make_shared<Integer>(0)}});
}

ExportGeometry::~ExportGeometry() {}

void ExportGeometry::DoWork()
{
	START_PROFILE;

	int objectCount = 0;
	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	while (HasInput(inputGeometry))
	{
		set_value_from<int>(*GetValue("count"), objectCount);
		UpdateValue("path");

		ProceduralObjectPtr inObject = GetInputProceduralObject(inputGeometry);

		auto geometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		auto geometry = geometryComponent->GetGeometry();
		selector::ObjExporter<Geometry> exporter(geometry);

		std::string outputPath = get_value_as<std::string>(*GetValue("path"));
		std::ofstream out_file(outputPath.c_str());
		exporter.Export(out_file);
		out_file.close();

		++objectCount;
	}
}

}  // namespace selector
