#include <geometry_core/geometry.h>
#include <geometry_operations/extrusion.h>
#include <math_lib/vec_base.h>

#include <gtest/gtest.h>

#include <memory>

using namespace selector;

using GeometryType = GeometryBase<>;
using GeometryPtr = std::shared_ptr<GeometryType>;

TEST(Extrusion, extrusion_triangle)
{
    /*
	// Initialisation
	auto in = std::make_shared<GeometryType>(3, 1, 4);
	Vec3F points[] = {
	    Vec3F(0, 0, 0),
	    Vec3F(1, 0, 0),
	    Vec3F(1, 1, 0),
	};

	for (auto p : points)
	{
		in->CreateVertex(p);
	}
	in->CreateFace(std::vector<typename GeometryType::IndexType>{0, 1, 2});

	in->GetFaceAttributes(0).m_normal = Vec3F(0, 0, 1);

	Extrusion<GeometryType> extrusion(1.0f);
	GeometrySizes sizes = extrusion.ResultSize(in);
	GeometryPtr out = std::make_shared<GeometryType>(sizes.m_numVertices, sizes.m_numEdges, sizes.m_numFaces);

	extrusion.Execute(in, out);

	ASSERT_EQ(out->GetNumFaces(), 5);
	ASSERT_EQ(out->GetNumVertices(), 6);
	ASSERT_EQ(out->GetNumEdges(), 18);

	Vec3F expected[] = {{1, 1, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}, {1, 0, 0}, {1, 0, 1},
	                    {0, 0, 1}, {1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1}, {1, 1, 0},
	                    {0, 0, 0}, {0, 0, 1}, {1, 1, 1}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1}};

	auto i = 0u;
	for (auto f = out->FacesBegin(); f != out->FacesEnd(); ++f)
	{
		for (auto face_vertex = out->FaceVertexBegin(*f); face_vertex.IsValid(); ++face_vertex)
		{
			auto point = out->GetVertexAttributes(*face_vertex).m_position;
			EXPECT_EQ(point, expected[i]);
			// std::cout << point << std::endl;
			++i;
		}
	}
    */
}

TEST(Extrusion, extrusion_square)
{
    /*
	// Initialisation
	GeometryPtr in = std::make_shared<GeometryType>(4, 1, 5);
	Vec3F points[] = {Vec3F(0, 0, 0), Vec3F(1, 0, 0), Vec3F(1, 1, 0), Vec3F(0, 1, 0)};

	for (auto p : points)
	{
		in->CreateVertex(p);
	}
	in->CreateFace(std::vector<typename GeometryType::IndexType>{0, 1, 2, 3});
	in->GetFaceAttributes(0).m_normal = Vec3F(0, 0, 1);

	Extrusion<GeometryType> extrusion(1.0f);
	GeometrySizes sizes = extrusion.ResultSize(in);
	GeometryPtr out = std::make_shared<GeometryType>(sizes.m_numVertices, sizes.m_numEdges, sizes.m_numFaces);

	extrusion.Execute(in, out);

	ASSERT_EQ(out->GetNumFaces(), 6);
	ASSERT_EQ(out->GetNumVertices(), 8);
	ASSERT_EQ(out->GetNumEdges(), 24);

	Vec3F expected[] = {
	    {0, 1, 0}, {1, 1, 0}, {1, 0, 0}, {0, 0, 0},

	    {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1},

	    {1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1},

	    {1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {1, 1, 1},

	    {0, 1, 0}, {0, 0, 0}, {0, 0, 1}, {0, 1, 1},

	    {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1},
	};

	auto i = 0u;
	for (auto f = out->FacesBegin(); f != out->FacesEnd(); ++f)
	{
		for (auto face_vertex = out->FaceVertexBegin(*f); face_vertex.IsValid(); ++face_vertex)
		{
			auto point = out->GetVertexAttributes(*face_vertex).m_position;
			ASSERT_EQ(point, expected[i]);
			++i;
		}
	}
    */
}

TEST(Extrusion, test_two_face_extrusion)
{
    /*
	// Initialisation
	auto in = std::make_shared<GeometryType>(4, 2, 8);
	Vec3F points[] = {
	    Vec3F(0, 0, 0),
	    Vec3F(1, 0, 0),
	    Vec3F(1, 1, 0),
	    Vec3F(0, 1, 0),
	};
	for (auto p : points)
	{
		in->CreateVertex(p);
	}
	in->CreateFace(std::vector<typename GeometryType::IndexType>{0, 1, 2});
	in->CreateFace(std::vector<typename GeometryType::IndexType>{0, 2, 3});
	in->GetFaceAttributes(0).m_normal = Vec3F(0, 0, 1);
	in->GetFaceAttributes(1).m_normal = Vec3F(0, 0, 1);

	Extrusion<GeometryType> extrusion(1.0f);
	GeometrySizes sizes = extrusion.ResultSize(in);
	GeometryPtr out = std::make_shared<GeometryType>(sizes.m_numVertices, sizes.m_numEdges, sizes.m_numFaces);

	extrusion.Execute(in, out);

	Vec3F expected[] = {
	    {1, 1, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}, {1, 0, 0}, {1, 1, 0},
	    {1, 1, 1}, {1, 0, 1}, {1, 1, 0}, {0, 0, 0}, {0, 0, 1}, {1, 1, 1}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1},
	    {0, 1, 0}, {1, 1, 0}, {0, 0, 0}, {0, 0, 0}, {1, 1, 0}, {1, 1, 1}, {0, 0, 1}, {1, 1, 0}, {0, 1, 0},
	    {0, 1, 1}, {1, 1, 1}, {0, 1, 0}, {0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 0, 1}, {1, 1, 1}, {0, 1, 1},
	};

	ASSERT_EQ(out->GetNumFaces(), 10);
	ASSERT_EQ(out->GetNumVertices(), 10);
	ASSERT_EQ(out->GetNumEdges(), 36);

	auto i = 0u;
	for (auto f = out->FacesBegin(); f != out->FacesEnd(); ++f)
	{
		for (auto face_vertex = out->FaceVertexBegin(*f); face_vertex.IsValid(); ++face_vertex)
		{
			auto point = out->GetVertexAttributes(*face_vertex).m_position;
			ASSERT_EQ(point, expected[i]);
			++i;
		}
	}
    */
}
