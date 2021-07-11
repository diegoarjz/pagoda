#include "read_geojson.h"

#include <pagoda/graph/execution_argument_callback.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>
#include <pagoda/geometry/io/geojson_reader.h>

#include <pagoda/common/fs/file_util.h>

#include <pagoda/dynamic/get_value_as.h>
#include "pagoda/dynamic/float_value.h"

#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>
#include <boost/qvm/map_vec_mat.hpp>

#include <iterator>
#include <memory>

namespace pagoda::geometry::operations
{
using namespace geometry::core;
using namespace geometry::io;
using namespace common::fs;
using namespace dynamic;
using namespace objects;
using namespace math;

const std::string ReadGeoJson::outputGeometry = "out";

ReadGeoJson::ReadGeoJson(objects::ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;
	CreateOutputInterface(outputGeometry);
}

ReadGeoJson::~ReadGeoJson() {}

void ReadGeoJson::SetParameters(graph::ExecutionArgumentCallback *cb)
{
	RegisterMember("file", cb->StringArgument("file", "File"));
	RegisterMember("ref_latitude", cb->FloatArgument("ref_latitude", "Reference Latitude", 0.0f));
	RegisterMember("ref_longitude", cb->FloatArgument("ref_longitude", "Reference Longitude", 0.0f));
}

const std::string &ReadGeoJson::GetOperationName() const
{
	static const std::string sName{"ReadGeoJson"};
	return sName;
}

void ReadGeoJson::DoWork()
{
	START_PROFILE;

	std::string file = get_value_as<std::string>(*GetValue("file"));
	std::string json = LoadFileToString(file);

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	GeoJsonReader reader(json);
	reader.SetReferenceCoordinate(
	  math::Vec2F{get_value_as<float>(*GetValue("ref_latitude")), get_value_as<float>(*GetValue("ref_longitude"))});

	reader.Read([&](GeoJsonReader::Feature &&f) {
		if (f.m_polygon.GetPointCount() < 3) {
			return true;
		}
		auto geometry = std::make_shared<Geometry>();
		std::vector<Vec3F> positions;
		std::transform(f.m_polygon.GetPoints().begin(), f.m_polygon.GetPoints().end(), std::back_inserter(positions),
		               [](const Vec2F &p) {
			               return Vec3F{X(p), Y(p), 0};
		               });
		geometry->CreateFaceFromPositions(positions);

		ProceduralObjectPtr object = CreateOutputProceduralObject(outputGeometry);
		auto geometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(object);

		geometryComponent->SetGeometry(geometry);
		geometryComponent->SetScope(
		  Scope::FromGeometryAndConstrainedRotation(geometry, Mat3x3F(boost::qvm::diag_mat(Vec3F{1.0f, 1.0f, 1.0f}))));

		for (const auto &property : f.m_properties) {
			object->RegisterMember(property.first, property.second);
		}

		return true;
	});
}

}  // namespace pagoda::geometry::operations
