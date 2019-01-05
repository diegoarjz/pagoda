#ifndef SELECTOR_GEOMETRY_CORE_GEOMETRY_TOPOLOGY_H_
#define SELECTOR_GEOMETRY_CORE_GEOMETRY_TOPOLOGY_H_

#include <unordered_set>
#include <vector>

#include "indexed_container.h"

namespace selector
{
template<typename IndexType>
struct Edge
{
	Edge(const IndexType& source, const IndexType& target) : m_sourceVertex(source), m_targetVertex(target)
	{
		DBG_ASSERT_MSG(source != target, "Cannot create an edge between the same vertices.");
	}

	bool operator==(const Edge& other) const
	{
		return m_sourceVertex == other.m_sourceVertex && m_targetVertex == other.m_targetVertex;
	};

	bool operator<=(const Edge& other) const
	{
		if (m_sourceVertex == other.m_sourceVertex)
		{
			return m_targetVertex <= other.m_targetVertex;
		}
		return m_sourceVertex <= other.m_sourceVertex;
	}

	IndexType m_sourceVertex;
	IndexType m_targetVertex;
};  // struct Edge

/**
 * @brief Hashes an \c Edge.
 *
 * Hashes a given \c Edge such that its lowest vertex index is in the MSB bits and the highest index is in the LSB.
 */
template<typename EdgeType>
struct EdgeHasher
{
	std::size_t operator()(const EdgeType& edge)
	{
		// TODO: it might be possible to specialize for index types of 32 bits.
		static constexpr uint32_t shift = 8 * sizeof(std::size_t) / 2;
		static constexpr std::size_t mask = (static_cast<std::size_t>(1) << shift) - 1;
		return ((edge.m_sourceVertex & mask) << shift) | (edge.m_targetVertex & mask);
	}
};  // struct EdgeHasher

template<typename I>
class IndexedFacesTopology
{
public:
	using IndexType = I;
	using EdgeType = Edge<IndexType>;

private:
	using FaceType = std::vector<IndexType>;
	using FaceContainerType = IndexedContainer<IndexType, FaceType>;
	using EdgeContainerType = IndexedContainer<IndexType, EdgeType>;

public:
	using FacesIterator = typename FaceContainerType::index_iterator;
	using EdgesIterator = typename EdgeContainerType::index_iterator;
	using VertexIterator = typename Range<IndexType>::iterator;

	static constexpr IndexType sInvalidIndex = std::numeric_limits<IndexType>::max();

	class FaceEdgeCirculator : public FaceType::iterator
	{
	public:
		FaceEdgeCirculator(typename FaceType::iterator start, typename FaceType::iterator end)
		    : m_currentEdge(start), m_lastEdge(end)
		{
		}

		bool operator==(const FaceEdgeCirculator& rhs) const { return m_currentEdge == rhs.m_currentEdge; }

		bool operator!=(const FaceEdgeCirculator& rhs) const { return !(*this == rhs); }

		bool IsValid() const { return m_currentEdge != m_lastEdge; }

		FaceEdgeCirculator operator++(int)
		{
			if (IsValid())
			{
				++m_currentEdge;
			}
			return *this;
		}

		FaceEdgeCirculator& operator++()
		{
			if (IsValid())
			{
				++m_currentEdge;
			}
			return *this;
		}

		const IndexType& operator*() { return *m_currentEdge; }

	private:
		typename FaceType::iterator m_currentEdge;
		typename FaceType::iterator m_lastEdge;
	};  // class FaceEdgeCirculator

	class FaceVertexCirculator
	{
	public:
		FaceVertexCirculator(IndexedFacesTopology* topology, FaceEdgeCirculator edge_circulator)
		    : m_topology(topology), m_edgeCirculator(edge_circulator)
		{
		}

		bool operator==(const FaceEdgeCirculator& rhs) const { return m_topology == rhs.m_topology && **this == *rhs; }

		bool operator!=(const FaceEdgeCirculator& rhs) const { return !(*this == rhs); }

		bool IsValid() const { return m_edgeCirculator.IsValid(); }

		FaceVertexCirculator operator++(int)
		{
			++m_edgeCirculator;
			return *this;
		}

		FaceVertexCirculator& operator++()
		{
			++m_edgeCirculator;
			return *this;
		}

		const IndexType& operator*() { return m_topology->m_edges.Get(*m_edgeCirculator).m_sourceVertex; }

	private:
		IndexedFacesTopology* m_topology;
		FaceEdgeCirculator m_edgeCirculator;
	};  // class FaceVertexCirculator

	IndexedFacesTopology() : m_reservedVertices(0), m_nextVertexIndex(0) {}

	IndexType CreateVertex()
	{
		START_PROFILE;

		if (GetNumVertices() == GetReservedVertices())
		{
			return sInvalidIndex;
		}

		auto vertex_index = m_nextVertexIndex++;
		return vertex_index;
	}

	template<class U>
	IndexType CreateFace(const U& vertex_indices)
	{
		START_PROFILE;

		if (GetNumEdges() == GetReservedEdges() || GetNumFaces() == GetReservedFaces())
		{
			return sInvalidIndex;
		}

		DBG_ASSERT_MSG(vertex_indices.size() >= 3, "Cannot create a face with less than 3 vertices");

		auto num_vertices = vertex_indices.size();
		std::vector<IndexType> face_edges;
		face_edges.reserve(num_vertices);
		for (uint32_t i = 0; i < num_vertices; ++i)
		{
			auto start_vertex = vertex_indices[i];
			auto target_vertex = vertex_indices[(i + 1) % num_vertices];

			auto edge_index = m_edges.Create(start_vertex, target_vertex);
			face_edges.push_back(edge_index);
		}

		return m_faces.Create(FaceType{face_edges.begin(), face_edges.end()});
	}

	FacesIterator FacesBegin() { return m_faces.index_begin(); }
	FacesIterator FacesEnd() { return m_faces.index_end(); }

	EdgesIterator EdgesBegin() { return m_edges.index_begin(); }
	EdgesIterator EdgesEnd() { return m_edges.index_end(); }

	VertexIterator VerticesBegin() { return Range<IndexType>(m_nextVertexIndex).begin(); }
	VertexIterator VerticesEnd() { return Range<IndexType>(m_nextVertexIndex).end(); }

	FaceEdgeCirculator FaceEdgeBegin(const IndexType& face_index)
	{
		auto& face = m_faces.Get(face_index);
		return FaceEdgeCirculator(face.begin(), face.end());
	}

	FaceVertexCirculator FaceVertexBegin(const IndexType& face_index)
	{
		return FaceVertexCirculator(this, FaceEdgeBegin(face_index));
	}

	uint32_t FaceVertexSize(const IndexType& face_index) const { return m_faces.Get(face_index).size(); }

	uint32_t GetNumFaces() const { return m_faces.size(); }
	uint32_t GetNumEdges() const { return m_edges.size(); }
	uint32_t GetNumVertices() const { return m_nextVertexIndex; }

	uint32_t GetReservedFaces() const { return m_faces.capacity(); }
	uint32_t GetReservedEdges() const { return m_edges.capacity(); }
	uint32_t GetReservedVertices() const { return m_reservedVertices; }

	void ReserveFaces(uint32_t new_size) { m_faces.reserve(new_size); }
	void ReserveEdges(uint32_t new_size) { m_edges.reserve(new_size); }
	void ReserveVertices(uint32_t new_size) { m_reservedVertices = new_size; }

private:
	IndexType m_reservedVertices;  // TODO: what should be done with this? It doesn't make sense right now and is only
	                               // used to make unit tests pass...
	IndexType m_nextVertexIndex;
	FaceContainerType m_faces;
	EdgeContainerType m_edges;
};  // class IndexedFacesTopology

template<typename I>
class SplitPointTopology
{
public:
	using IndexType = I;

private:
	struct Point
	{
		/// Edges that start in this point
		std::vector<IndexType> m_sourceEdges;
	};

	struct Vertex
	{
		IndexType m_point;
		IndexType m_face;
		IndexType m_edge;  ///< \c Edge whose source vertex is this vertex.
	};

	struct Edge
	{
		IndexType m_sourceVertex;
		IndexType m_targetVertex;
	};

	struct Face
	{
		IndexType m_vertex;
		uint32_t m_numVertices;
	};

public:
	using FaceContainerType = IndexedContainer<IndexType, Face>;
	using FacesIterator = typename FaceContainerType::index_iterator;
	using EdgeContainerType = IndexedContainer<IndexType, Edge>;
	using EdgesIterator = typename EdgeContainerType::index_iterator;
	using VertexContainerType = IndexedContainer<IndexType, Point>;
	using VertexIterator = typename VertexContainerType::index_iterator;

	static constexpr IndexType sInvalidIndex = std::numeric_limits<IndexType>::max();

	class FaceEdgeCirculator
	{
	public:
		FaceEdgeCirculator(SplitPointTopology<IndexType>* topology, uint32_t start_edge)
		    : m_topology(topology), m_startEdge(start_edge), m_currentEdge(start_edge), m_valid(true)
		{
		}

		bool operator==(const FaceEdgeCirculator& rhs) const
		{
			return m_topology == rhs.m_topology && m_currentEdge == rhs.m_currentEdge;
		}

		bool operator!=(const FaceEdgeCirculator& rhs) const { return !(*this == rhs); }

		bool IsValid() const { return m_valid; }

		FaceEdgeCirculator operator++(int)
		{
			m_currentEdge = NextEdge();

			if (m_currentEdge == m_startEdge)
			{
				m_valid = false;
			}

			return *this;
		}

		FaceEdgeCirculator& operator++()
		{
			m_currentEdge = NextEdge();

			if (m_currentEdge == m_startEdge)
			{
				m_valid = false;
			}

			return *this;
		}

		const IndexType& operator*() { return m_currentEdge; }

	private:
		IndexType NextEdge()
		{
			auto& edge = m_topology->m_edges.Get(m_currentEdge);
			auto& m_targetVertex = m_topology->vertices.Get(edge.m_targetVertex);
			return m_targetVertex.m_edge;
		}

		SplitPointTopology<IndexType>* m_topology;
		IndexType m_startEdge;
		IndexType m_currentEdge;
		bool m_valid;
	};  // class FaceEdgeCirculator

	class FaceVertexCirculator
	{
	public:
		FaceVertexCirculator(SplitPointTopology* topology, FaceEdgeCirculator edge_circulator)
		    : m_topology(topology), m_edgeCirculator(edge_circulator)
		{
		}

		bool operator==(const FaceEdgeCirculator& rhs) const { return m_topology == rhs.m_topology && **this == *rhs; }

		bool operator!=(const FaceEdgeCirculator& rhs) const { return !(*this == rhs); }

		bool IsValid() const { return m_edgeCirculator.IsValid(); }

		FaceVertexCirculator operator++(int)
		{
			++m_edgeCirculator;
			return *this;
		}

		FaceVertexCirculator& operator++()
		{
			++m_edgeCirculator;
			return *this;
		}

		const IndexType& operator*()
		{
			auto vertex_index = m_topology->m_edges.Get(*m_edgeCirculator).m_sourceVertex;
			return m_topology->vertices.Get(vertex_index).m_point;
		}

	private:
		SplitPointTopology* m_topology;
		FaceEdgeCirculator m_edgeCirculator;
	};  // class FaceVertexCirculator

	IndexType CreateVertex()
	{
		START_PROFILE;

		if (GetNumVertices() == GetReservedVertices())
		{
			return sInvalidIndex;
		}

		return points.Create(Point());
	}

	template<class U>
	IndexType CreateFace(const U& vertex_indices)
	{
		START_PROFILE;

		if (GetNumEdges() == GetReservedEdges() || GetNumFaces() == GetReservedFaces())
		{
			return sInvalidIndex;
		}

		DBG_ASSERT_MSG(vertex_indices.size() >= 3, "Cannot create a face with less than 3 vertices");

		IndexType face_index = m_faces.Create();
		auto& face = m_faces.Get(face_index);

		std::size_t num_vertices = vertex_indices.size();
		std::vector<IndexType> face_verts;
		face_verts.reserve(num_vertices);

		for (uint32_t i = 0; i < num_vertices; ++i)
		{
			IndexType v = vertices.Create();
			Vertex& vert = vertices.Get(v);
			vert.m_point = vertex_indices[i];
			vert.m_face = face_index;
			face_verts.push_back(v);
		}

		for (uint32_t i = 0; i < num_vertices; ++i)
		{
			Vertex& v1 = vertices.Get(face_verts[i]);

			IndexType edge_index = m_edges.Create();
			Edge& e = m_edges.Get(edge_index);
			e.m_sourceVertex = face_verts[i];
			e.m_targetVertex = face_verts[(i + 1) % num_vertices];

			v1.m_edge = edge_index;

			points.Get(v1.m_point).m_sourceEdges.push_back(edge_index);
		}

		face.m_vertex = face_verts[0];
		face.m_numVertices = num_vertices;
		return face_index;
	}

	FacesIterator FacesBegin() { return m_faces.index_begin(); }
	FacesIterator FacesEnd() { return m_faces.index_end(); }

	EdgesIterator EdgesBegin() { return m_edges.index_begin(); }
	EdgesIterator EdgesEnd() { return m_edges.index_end(); }

	VertexIterator VerticesBegin() { return points.index_begin(); }
	VertexIterator VerticesEnd() { return points.index_end(); }

	FaceEdgeCirculator FaceEdgeBegin(const IndexType& face_index)
	{
		auto face = m_faces.Get(face_index);
		auto start_edge = vertices.Get(face.m_vertex).m_edge;
		return FaceEdgeCirculator(this, start_edge);
	}

	FaceVertexCirculator FaceVertexBegin(const IndexType& face_index)
	{
		return FaceVertexCirculator(this, FaceEdgeBegin(face_index));
	}

	uint32_t GetNumFaces() const { return m_faces.size(); }
	uint32_t GetNumEdges() const { return m_edges.size(); }
	uint32_t GetNumVertices() const { return points.size(); }

	uint32_t GetReservedFaces() const { return m_faces.capacity(); }
	uint32_t GetReservedEdges() const { return m_edges.capacity(); }
	uint32_t GetReservedVertices() const { return points.capacity(); }

	uint32_t FaceVertexSize(const IndexType& face_index) const { return m_faces.Get(face_index).m_numVertices; }

	void ReserveFaces(uint32_t new_size) { m_faces.reserve(new_size); }
	void ReserveEdges(uint32_t new_size) { m_edges.reserve(new_size); }
	void ReserveVertices(uint32_t new_size) { points.reserve(new_size); }

private:
	IndexedContainer<IndexType, Point> points;
	FaceContainerType m_faces;
	IndexedContainer<IndexType, Vertex> vertices;
	IndexedContainer<IndexType, Edge> m_edges;
};  // class SplitPointTopology
}  // namespace selector
#endif
