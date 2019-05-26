#include <sstream>

#include <geometry_core/geometry.h>
#include <geometry_core/geometry_builder.h>
#include <geometry_core/geometry_exporter.h>
#include <geometry_core/geometry_topology.h>
#include <math_lib/vec_base.h>

#include <gtest/gtest.h>

using namespace selector;

using GeometryType = GeometryT<SplitPointTopology<uint32_t>>;

TEST(GeometryExporter, geometry_exporter_export_obj_one_face)
{
	auto geom = std::make_shared<GeometryType>(4, 1, 5);
	GeometryBuilderT<GeometryType> builder(geom);

	builder.AddPoint(Vec3F(0, 0, 0));
	builder.AddPoint(Vec3F(1, 0, 0));
	builder.AddPoint(Vec3F(1, 1, 0));
	builder.AddPoint(Vec3F(0, 1, 0));

	auto face = builder.StartFace(4);
	face.AddIndex(0);
	face.AddIndex(1);
	face.AddIndex(2);
	face.AddIndex(3);
	face.CloseFace();

	ObjExporter<GeometryType> exporter(geom);

	std::stringstream ss;

	exporter.Export(ss);

	std::string expected_string = "v 0 0 0\n"
	                              "v 1 0 0\n"
	                              "v 1 1 0\n"
	                              "v 0 1 0\n"
	                              "f 1 2 3 4 \n";
	auto out_string = ss.str();
	EXPECT_EQ(out_string, expected_string);
}
