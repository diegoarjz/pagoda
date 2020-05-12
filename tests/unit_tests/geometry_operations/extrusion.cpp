#include <pagoda/geometry/algorithms/extrusion.h>
#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_exporter.h>
#include <pagoda/math/vec_base.h>

#include <pagoda/common/fs/file_util.h>

#include "../test_utils.h"

#include <gtest/gtest.h>

#include <memory>

using namespace pagoda;
using namespace pagoda::math;
using namespace pagoda::geometry::core;
using namespace pagoda::geometry::algorithms;

using GeometryType = GeometryBase<>;
using GeometryPtr = std::shared_ptr<GeometryType>;

class ExtrusionTest : public PagodaTestFixture<::testing::Test>
{
public:
	void SetUp() {}

	void TearDown() {}
};

TEST_F(ExtrusionTest, test_extrusion)
{
	// Initialisation
	auto geom = std::make_shared<GeometryType>();
	GeometryBuilderT<GeometryType> builder(geom);

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

	Extrusion<GeometryType> extrusion(1.0f);
	GeometryPtr out = std::make_shared<GeometryType>();

	extrusion.Execute(geom, out);

	ObjExporter<GeometryType> exporter(out);
	std::stringstream ss;
	exporter.Export(ss);
	MatchFile match(GetCurrentTestFileResultsDirectory() /= "geometry.obj", GetShouldWriteFiles());
	match.Match(ss.str());
}
