#include <geometry_core/geometry_topology.h>
#include <math_lib/vec_base.h>

#include <list>

#include <gtest/gtest.h>

using namespace selector;

template<class IndexType>
class EdgeHasherTest : public ::testing::Test
{
};

typedef ::testing::Types<uint32_t, int32_t> EdgeIndexTypes;
TYPED_TEST_SUITE(EdgeHasherTest, EdgeIndexTypes);

TYPED_TEST(EdgeHasherTest, test_edge_0_1)
{
	Edge<TypeParam> e(0, 1);
	EdgeHasher<Edge<TypeParam>> hasher;

	EXPECT_EQ(hasher(e), 1);
}

TYPED_TEST(EdgeHasherTest, test_edge_1_2)
{
	Edge<TypeParam> e(1, 2);
	EdgeHasher<Edge<TypeParam>> hasher;

	EXPECT_EQ(hasher(e), 2 | static_cast<size_t>(1) << (sizeof(std::size_t) * 8 / 2));
}

typedef ::testing::Types<IndexedFacesTopology<uint32_t>, SplitPointTopology<uint32_t>> TopologyTypes;

template<class Topology>
class TopologyTest : public ::testing::Test
{
protected:
	virtual void SetUp() { invalidIndex = Topology::sInvalidIndex; }

	virtual void TearDown() {}

	std::vector<typename Topology::IndexType> CreateVerts(uint32_t count)
	{
		topology.ReserveVertices(count);
		std::vector<typename Topology::IndexType> verts;
		for (uint32_t i = 0; i < count; ++i)
		{
			verts.push_back(topology.CreateVertex());
		}
		return verts;
	}

	Topology topology;
	uint32_t invalidIndex;
};
TYPED_TEST_SUITE(TopologyTest, TopologyTypes);

TYPED_TEST(TopologyTest, when_creating_should_be_empty)
{
	EXPECT_EQ(this->topology.GetNumVertices(), 0);
	EXPECT_EQ(this->topology.GetNumEdges(), 0);
	EXPECT_EQ(this->topology.GetNumFaces(), 0);
}

TYPED_TEST(TopologyTest, when_creating_should_not_reserve_sizes)
{
	EXPECT_EQ(this->topology.GetReservedVertices(), 0);
	EXPECT_EQ(this->topology.GetReservedEdges(), 0);
	EXPECT_EQ(this->topology.GetReservedFaces(), 0);
}

TYPED_TEST(TopologyTest, when_reserving_sizes_should_increase_space)
{
	this->topology.ReserveFaces(1);
	this->topology.ReserveEdges(3);
	this->topology.ReserveVertices(3);

	EXPECT_EQ(this->topology.GetReservedVertices(), 3);
	EXPECT_EQ(this->topology.GetReservedEdges(), 3);
	EXPECT_EQ(this->topology.GetReservedFaces(), 1);
}

TYPED_TEST(TopologyTest, when_creating_vertices_before_reserving_space_should_return_invalid_index)
{
	EXPECT_EQ(this->topology.CreateVertex(), this->invalidIndex);
}

TYPED_TEST(TopologyTest, when_creating_vertices_before_reserving_space_should_not_create_vertex)
{
	this->topology.CreateVertex();
	EXPECT_EQ(this->topology.GetNumVertices(), 0);
}

TYPED_TEST(TopologyTest, when_creating_faces_before_reserving_space_should_return_invalid_index)
{
	EXPECT_EQ(this->topology.CreateFace(this->CreateVerts(3)), this->invalidIndex);
}

TYPED_TEST(TopologyTest, when_creating_faces_before_reserving_space_should_not_create_face)
{
	this->topology.CreateFace(this->CreateVerts(3));

	EXPECT_EQ(this->topology.GetNumFaces(), 0);
}

TYPED_TEST(TopologyTest, when_creating_faces_before_reserving_space_for_edges_should_return_invalid_index)
{
	this->topology.ReserveFaces(1);
	EXPECT_EQ(this->topology.CreateFace(this->CreateVerts(3)), this->invalidIndex);
}

TYPED_TEST(TopologyTest, when_creating_faces_before_reserving_space_for_edges_should_not_create_face_nor_edges)
{
	this->topology.ReserveFaces(1);
	this->topology.CreateFace(this->CreateVerts(3));

	EXPECT_EQ(this->topology.GetNumFaces(), 0);
	EXPECT_EQ(this->topology.GetNumEdges(), 0);
}

TYPED_TEST(TopologyTest, when_creating_vertices_should_return_incremental_indices)
{
	this->topology.ReserveVertices(4);
	EXPECT_EQ(this->topology.CreateVertex(), 0);
	EXPECT_EQ(this->topology.CreateVertex(), 1);
	EXPECT_EQ(this->topology.CreateVertex(), 2);
	EXPECT_EQ(this->topology.CreateVertex(), 3);
}

TYPED_TEST(TopologyTest, when_creating_vertices_should_increase_the_vertex_count)
{
	this->CreateVerts(3);
	EXPECT_EQ(this->topology.GetNumVertices(), 3);
}

TYPED_TEST(TopologyTest, when_creating_faces_should_increase_the_face_and_edge_count)
{
	this->topology.ReserveFaces(1);
	this->topology.ReserveEdges(3);
	this->topology.CreateFace(this->CreateVerts(3));

	EXPECT_EQ(this->topology.GetNumFaces(), 1);
	EXPECT_EQ(this->topology.GetNumEdges(), 3);
}

TYPED_TEST(TopologyTest, when_creating_faces_should_return_incremental_indices)
{
	std::vector<typename TypeParam::IndexType> verts = this->CreateVerts(3);
	this->topology.ReserveFaces(2);
	this->topology.ReserveEdges(6);
	EXPECT_EQ(this->topology.CreateFace(verts), 0);
	EXPECT_EQ(this->topology.CreateFace(verts), 1);
}

TYPED_TEST(TopologyTest, when_creating_faces_should_create_faces_with_correct_vertex_count)
{
	this->topology.ReserveFaces(1);
	this->topology.ReserveEdges(3);
	this->topology.CreateFace(this->CreateVerts(3));

	auto face = *(this->topology.FacesBegin());
	EXPECT_EQ(this->topology.FaceVertexSize(face), 3);
}

template<class Topology>
class TopologyIteratorTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		triangle_topology.ReserveVertices(3);
		triangle_topology.ReserveEdges(3);
		triangle_topology.ReserveFaces(1);
		triangle = {triangle_topology.CreateVertex(), triangle_topology.CreateVertex(),
		            triangle_topology.CreateVertex()};
		triangle_topology.CreateFace(triangle);

		triangle_strip_topology.ReserveVertices(4);
		triangle_strip_topology.ReserveEdges(5);
		triangle_strip_topology.ReserveFaces(2);
		square = {triangle_strip_topology.CreateVertex(), triangle_strip_topology.CreateVertex(),
		          triangle_strip_topology.CreateVertex(), triangle_strip_topology.CreateVertex()};
		triangle_strip_topology.CreateFace(std::vector<typename Topology::IndexType>{square[0], square[1], square[2]});
		triangle_strip_topology.CreateFace(std::vector<typename Topology::IndexType>{square[0], square[2], square[3]});
	}

	virtual void TearDown() {}

	Topology triangle_topology;
	Topology triangle_strip_topology;
	std::vector<typename Topology::IndexType> triangle;
	std::vector<typename Topology::IndexType> square;
};
TYPED_TEST_SUITE(TopologyIteratorTest, TopologyTypes);

TYPED_TEST(TopologyIteratorTest, when_iterating_over_all_faces_last_element_should_be_faces_end)
{
	typename TypeParam::FacesIterator face = this->triangle_topology.FacesBegin();
	EXPECT_EQ(*face, 0);
	EXPECT_EQ(++face, this->triangle_topology.FacesEnd());
}

TYPED_TEST(TopologyIteratorTest, when_iterating_over_all_edges_last_element_should_be_edges_end)
{
	typename TypeParam::EdgesIterator edge = this->triangle_topology.EdgesBegin();
	++edge;
	++edge;
	++edge;
	EXPECT_EQ(edge, this->triangle_topology.EdgesEnd());
}

TYPED_TEST(TopologyIteratorTest, when_iterating_over_all_vertices_last_element_should_be_vertices_end)
{
	typename TypeParam::VertexIterator vertex = this->triangle_topology.VerticesBegin();
	++vertex;
	++vertex;
	++vertex;
	EXPECT_EQ(vertex, this->triangle_topology.VerticesEnd());
}

TYPED_TEST(TopologyIteratorTest, when_iterating_over_faces_should_visit_all_faces)
{
	typename TypeParam::FacesIterator face = this->triangle_strip_topology.FacesBegin();
	typename TypeParam::FacesIterator face_end = this->triangle_strip_topology.FacesEnd();

	uint32_t i;
	for (i = 0; face != face_end; ++face, ++i)
	{
		EXPECT_EQ(*face, i);
	}
	EXPECT_EQ(i, 2);
}

TYPED_TEST(TopologyIteratorTest, when_iterating_over_edges_should_visit_all_edges)
{
	typename TypeParam::EdgesIterator edge = this->triangle_strip_topology.EdgesBegin();
	typename TypeParam::EdgesIterator edge_end = this->triangle_strip_topology.EdgesEnd();

	uint32_t i;
	for (i = 0u; edge != edge_end; ++edge, ++i)
	{
		EXPECT_EQ(*edge, i);
	}
	EXPECT_EQ(i, 6);
}

TYPED_TEST(TopologyIteratorTest, when_iterating_over_vertices_should_visit_all_vertices)
{
	typename TypeParam::VertexIterator vertex = this->triangle_strip_topology.VerticesBegin();
	typename TypeParam::VertexIterator vertices_end = this->triangle_strip_topology.VerticesEnd();

	uint32_t i;
	for (i = 0; vertex != vertices_end; ++vertex, ++i)
	{
		EXPECT_EQ(*vertex, i);
	}
	EXPECT_EQ(i, 4);
}

TYPED_TEST(TopologyIteratorTest, when_circulating_edges_of_a_face_should_visit_all_edges)
{
	auto face_index = *(this->triangle_topology.FacesBegin());
	auto circulator = this->triangle_topology.FaceEdgeBegin(face_index);

	uint32_t i;
	for (i = 0; circulator.IsValid(); ++i, ++circulator)
	{
		EXPECT_EQ(*circulator, i);
	}
	EXPECT_EQ(i, 3);
}

TYPED_TEST(TopologyIteratorTest, when_circulating_vertices_of_a_face_should_visit_all_vertices)
{
	auto face_index = *(this->triangle_topology.FacesBegin());
	auto circulator = this->triangle_topology.FaceVertexBegin(face_index);

	uint32_t i;
	for (i = 0; circulator.IsValid(); ++i, ++circulator)
	{
		EXPECT_EQ(*circulator, i);
	}
	EXPECT_EQ(i, 3);
}

TYPED_TEST(TopologyIteratorTest, test_vertex_face_circulator) {}

TYPED_TEST(TopologyIteratorTest, test_vertex_edge_circulator) {}
