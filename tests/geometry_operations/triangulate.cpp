#include <geometry_operations/triangulate.h>

#include <geometry_core/geometry.h>
#include <geometry_core/geometry_builder.h>
#include <geometry_core/geometry_topology.h>

#include <gtest/gtest.h>

using namespace selector;

using GeometryType = GeometryT<SplitPointTopology<uint32_t>>;
using GeometryPtr = std::shared_ptr<GeometryType>;

TEST(TriangulateTest, when_calculating_geometry_size_for_a_quad_should_return_2_triangles)
{
	auto geometry = std::make_shared<GeometryType>(4, 3, 1);
	Vec3F vert[] = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}};
	GeometryBuilderT<GeometryType> b(geometry);

	for (auto v : vert)
	{
		b.AddPoint(v);
	}
	auto faceBuilder = b.StartFace(4);
	faceBuilder.AddIndex(0);
	faceBuilder.AddIndex(1);
	faceBuilder.AddIndex(2);
	faceBuilder.AddIndex(3);
	faceBuilder.CloseFace();

	Triangulate<GeometryType> t;

	GeometrySizes sizes = t.ResultSize(geometry);

	EXPECT_EQ(sizes.m_numVertices, 4);
	EXPECT_EQ(sizes.m_numEdges, 6);
	EXPECT_EQ(sizes.m_numFaces, 2);
}
