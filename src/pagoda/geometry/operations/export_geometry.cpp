#include "export_geometry.h"
#include "boost/filesystem/file_status.hpp"

#include <pagoda/objects/parameter_callback.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>
#include <pagoda/geometry/io/geometry_exporter.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/common/fs/file_util.h>

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/set_value_from.h>

#include <boost/filesystem.hpp>

#include <fstream>

namespace pagoda::geometry::operations {
using namespace objects;
using namespace geometry::core;
using namespace geometry::io;
using namespace dynamic;

const char *ExportGeometry::name = "ExportGeometry";
const std::string ExportGeometry::inputGeometry("in");

ExportGeometry::ExportGeometry(ProceduralObjectSystemPtr objectSystem)
    : ProceduralOperation(objectSystem), m_objectCount(0) {}

ExportGeometry::~ExportGeometry() {}

void ExportGeometry::Parameters(objects::NewParameterCallback *cb) {
  if (auto par = cb->IntegerParameter(&m_objectCount, "count", "Count", 0)) {
    par->SetFlag(ParameterBase::Flag::NoWrite, true);
  }
  cb->StringParameter(&m_path, "path", "Path", "");
}

const std::string &ExportGeometry::GetOperationName() const {
  static const std::string sName{"ExportGeometry"};
  return sName;
}

void ExportGeometry::Interfaces(InterfaceCallback *cb) {
  cb->InputInterface(m_input, inputGeometry, "In", Interface::Arity::Many);
}

void ExportGeometry::DoWork() {
  START_PROFILE;

  auto geometrySystem =
      m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

  while (ProceduralObjectPtr inObject = m_input->GetNext()) {

    auto geometryComponent =
        geometrySystem->GetComponentAs<GeometryComponent>(inObject);
    auto geometry = geometryComponent->GetGeometry();
    ObjExporter<Geometry> exporter(geometry);

    common::fs::CreateDirectories(
        boost::filesystem::path(m_path).parent_path());

    std::ofstream out_file(m_path.c_str());
    exporter.Export(out_file);
    out_file.close();

    ++m_objectCount;
  }
}

} // namespace pagoda::geometry::operations
