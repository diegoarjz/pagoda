#include "export_geometry.h"

#include "geometry_component.h"
#include "hierarchical_component.h"
#include "procedural_object_system.h"

#include <pagoda/common/fs/file_util.h>
#include <pagoda/dynamic_value/get_value_as.h>
#include <pagoda/dynamic_value/set_value_from.h>
#include <pagoda/geometry_core/geometry_exporter.h>

#include <boost/filesystem/path.hpp>

#include <fstream>

namespace pagoda
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
		pagoda::ObjExporter<Geometry> exporter(geometry);

		std::string outputPath = get_value_as<std::string>(*GetValue("path"));
		common::fs::CreateDirectories(boost::filesystem::path(outputPath).parent_path());

		std::ofstream out_file(outputPath.c_str());
		exporter.Export(out_file);
		out_file.close();

		++objectCount;
	}
}

}  // namespace pagoda
