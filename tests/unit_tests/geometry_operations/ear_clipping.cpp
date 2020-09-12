#include <pagoda/common/fs/file_util.h>
#include <pagoda/geometry/algorithms/create_box.h>
#include <pagoda/geometry/algorithms/ear_clipping.h>
#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/geometry/io/geometry_exporter.h>
#include <pagoda/math/math_utils.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include <iostream>

using namespace pagoda;
using namespace pagoda::common;
using namespace pagoda::math;
using namespace pagoda::geometry::core;
using namespace pagoda::geometry::algorithms;
using namespace pagoda::geometry::io;

using GeometryType = GeometryBase<>;
using GeometryPtr = std::shared_ptr<GeometryType>;

class EarClippingTest : public PagodaTestFixture<::testing::Test>
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

TEST_F(EarClippingTest, test_triangulate_square)
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

	EarClipping<GeometryType> t;
	t.Execute(m_geometry, m_geometryOut);

	ObjExporter<GeometryType> exporter(m_geometryOut);
	std::stringstream ss;
	exporter.Export(ss);
	MatchFile match(GetCurrentTestFileResultsDirectory() /= "geometry.obj", GetShouldWriteFiles());
	match.Match(ss.str());
}

TEST_F(EarClippingTest, test_triangulate_concave)
{
	GeometryBuilderT<GeometryType> builder(m_geometry);

	builder.AddPoint(Vec3F{0.5, 0.5, 0});
	builder.AddPoint(Vec3F{0, 1, 0});
	builder.AddPoint(Vec3F{0, 0, 0});
	builder.AddPoint(Vec3F{1, 0, 0});
	builder.AddPoint(Vec3F{1, 1, 0});

	auto face = builder.StartFace(5);
	face.AddIndex(0);
	face.AddIndex(1);
	face.AddIndex(2);
	face.AddIndex(3);
	face.AddIndex(4);
	face.CloseFace();

	EarClipping<GeometryType> t;
	t.Execute(m_geometry, m_geometryOut);

	ObjExporter<GeometryType> exporter(m_geometryOut);
	std::stringstream ss;
	exporter.Export(ss);
	MatchFile match(GetCurrentTestFileResultsDirectory() /= "geometry.obj", GetShouldWriteFiles());
	match.Match(ss.str());
}

TEST_F(EarClippingTest, test_triangulate_box)
{
	CreateBox<GeometryType> box(10, 10, 10);
	box.Execute(m_geometry);

	EarClipping<GeometryType> t;
	t.Execute(m_geometry, m_geometryOut);

	ObjExporter<GeometryType> exporter(m_geometryOut);
	std::stringstream ss;
	exporter.Export(ss);
	MatchFile match(GetCurrentTestFileResultsDirectory() /= "geometry.obj", GetShouldWriteFiles());
	match.Match(ss.str());
}

TEST_F(EarClippingTest, test_triangulate_star)
{
	uint32_t slices = 30;
	uint32_t outerRadius = 10;
	uint32_t innerRadius = 5;

	GeometryBuilderT<GeometryType> builder(m_geometry);
	auto face = builder.StartFace(slices);
	float a = 0.0;
	for (auto i = 0u; i < slices; ++i) {
		auto r = i % 2 == 0 ? outerRadius : innerRadius;
		face.AddIndex(builder.AddPoint(r * Vec3F{std::cos(a), std::sin(a), 0}));
		a += MathUtils<float>::two_pi / slices;
	}
	face.CloseFace();
	m_geometry->GetFaceAttributes(*m_geometry->FacesBegin()).m_normal = Vec3F{0, 0, 1};

	std::stringstream ss1;
	ObjExporter<GeometryType> e(m_geometry);
	e.Export(ss1);
	MatchFile match1(GetCurrentTestFileResultsDirectory() /= "in.obj", GetShouldWriteFiles());
	match1.Match(ss1.str());

	EarClipping<GeometryType> t;
	t.Execute(m_geometry, m_geometryOut);

	ObjExporter<GeometryType> exporter(m_geometryOut);
	std::stringstream ss;
	exporter.Export(ss);
	MatchFile match(GetCurrentTestFileResultsDirectory() /= "geometry.obj", GetShouldWriteFiles());
	match.Match(ss.str());
}
