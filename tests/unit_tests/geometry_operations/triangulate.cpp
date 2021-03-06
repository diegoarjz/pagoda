#include <common/file_util.h>
#include <geometry_core/geometry.h>
#include <geometry_core/geometry_builder.h>
#include <geometry_core/geometry_exporter.h>
#include <geometry_operations/triangulate.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;

using GeometryType = GeometryBase<>;
using GeometryPtr = std::shared_ptr<GeometryType>;

class TriangulateTest : public PagodaTestFixture<::testing::Test>
{
public:
	void SetUp()
	{
		m_geometry = std::make_shared<GeometryType>();
		m_geometryOut = std::make_shared<GeometryType>();
	}

	void TearDown() {}

	std::shared_ptr<GeometryType> m_geometry;
	std::shared_ptr<GeometryType> m_geometryOut;
};

TEST_F(TriangulateTest, test_triangulate_triangle)
{
	GeometryBuilderT<GeometryType> builder(m_geometry);

	builder.AddPoint(Vec3F{0, 0, 0});
	builder.AddPoint(Vec3F{1, 0, 0});
	builder.AddPoint(Vec3F{1, 1, 0});

	auto face = builder.StartFace(3);
	face.AddIndex(0);
	face.AddIndex(1);
	face.AddIndex(2);
	face.CloseFace();

	Triangulate<GeometryType> t;
	t.Execute(m_geometry, m_geometryOut);

	ObjExporter<GeometryType> exporter(m_geometryOut);
	std::stringstream ss;
	exporter.Export(ss);
	MatchFile match(GetCurrentTestFileResultsDirectory() /= "geometry.obj", GetShouldWriteFiles());
	match.Match(ss.str());
}

TEST_F(TriangulateTest, test_triangulate_square)
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

	Triangulate<GeometryType> t;
	t.Execute(m_geometry, m_geometryOut);

	ObjExporter<GeometryType> exporter(m_geometryOut);
	std::stringstream ss;
	exporter.Export(ss);
	MatchFile match(GetCurrentTestFileResultsDirectory() /= "geometry.obj", GetShouldWriteFiles());
	match.Match(ss.str());
}
