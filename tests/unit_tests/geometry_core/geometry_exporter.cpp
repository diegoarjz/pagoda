#include <sstream>

#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/geometry/io/geometry_exporter.h>
#include <pagoda/math/vec_base.h>

#include <pagoda/common/fs/file_util.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;
using namespace pagoda::math;
using namespace pagoda::geometry::core;
using namespace pagoda::geometry::io;

using GeometryType = GeometryBase<>;

class GeometryExporterTest : public PagodaTestFixture<::testing::Test>
{
	public:
	void SetUp() { m_geometry = std::make_shared<GeometryType>(); }

	void TearDown() {}

	std::shared_ptr<GeometryType> m_geometry;
};

TEST_F(GeometryExporterTest, geometry_exporter_export_obj_one_face)
{
	GeometryBuilderT<GeometryType> builder(m_geometry);

	builder.AddPoint(Vec3F{0, 0, 0});
	builder.AddPoint(Vec3F{1, 0, 0});
	builder.AddPoint(Vec3F{1, 1, 0});
	builder.AddPoint(Vec3F{0, 1, 0});

	auto face = builder.StartFace(4);
	face.AddIndex(0);
	face.AddIndex(1);
	face.AddIndex(2);
	face.AddIndex(3);
	face.CloseFace();

	ObjExporter<GeometryType> exporter(m_geometry);
	std::stringstream ss;
	exporter.Export(ss);

	MatchFile match(GetCurrentTestFileResultsDirectory() /= "geometry.obj", GetShouldWriteFiles());
	match.Match(ss.str());
}

TEST_F(GeometryExporterTest, geometry_exporter_export_cube)
{
	GeometryBuilderT<GeometryType> builder(m_geometry);

	// Bottom face
	builder.AddPoint(Vec3F{0, 0, 0});
	builder.AddPoint(Vec3F{1, 0, 0});
	builder.AddPoint(Vec3F{1, 1, 0});
	builder.AddPoint(Vec3F{0, 1, 0});

	// Top face
	builder.AddPoint(Vec3F{0, 0, 1});
	builder.AddPoint(Vec3F{1, 0, 1});
	builder.AddPoint(Vec3F{1, 1, 1});
	builder.AddPoint(Vec3F{0, 1, 1});

	auto bottomFace = builder.StartFace(4);
	auto topFace = builder.StartFace(4);
	typename GeometryBuilderT<GeometryType>::FaceBuilder sideFaces[] = {
	  builder.StartFace(4),
	  builder.StartFace(4),
	  builder.StartFace(4),
	  builder.StartFace(4),
	};

	for (auto i : {3, 2, 1, 0}) {
		bottomFace.AddIndex(i);
	}
	for (auto i : {4, 5, 6, 7}) {
		topFace.AddIndex(i);
	}
	bottomFace.CloseFace();
	topFace.CloseFace();

	uint32_t i = 0;
	std::vector<std::vector<uint32_t>> sideFaceIndices;
	sideFaceIndices.push_back({0, 1, 5, 4});
	sideFaceIndices.push_back({1, 2, 6, 5});
	sideFaceIndices.push_back({2, 3, 7, 6});
	sideFaceIndices.push_back({3, 0, 4, 7});
	for (auto &f : sideFaceIndices) {
		for (auto index : f) {
			sideFaces[i].AddIndex(index);
		}
		sideFaces[i++].CloseFace();
	}

	ObjExporter<GeometryType> exporter(m_geometry);

	std::stringstream ss;
	exporter.Export(ss);
	MatchFile match(GetCurrentTestFileResultsDirectory() /= "geometry.obj", GetShouldWriteFiles());
	match.Match(ss.str());
}
