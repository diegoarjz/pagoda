#ifndef SELECTOR_SPLIT_POINT_TOPOLOGY_H_
#define SELECTOR_SPLIT_POINT_TOPOLOGY_H_

#include "indexed_container.h"

#include <array>
#include <iostream>
#include <unordered_set>

namespace selector
{
/**
 * Implements a Split Point Topology to be used by \c Geometry.
 */
class SplitPointTopology
{
public:
	using Index_t = uint32_t;             ///< Type used by indices.
	static const Index_t s_invalidIndex;  ///< Indicate an invalid index.

private:
	/**
	 * Represents a handle in that references a topology component.
	 */
	struct Handle
	{
		Handle(const Index_t &index) : m_index(index) {}
		Index_t GetIndex() const { return m_index; }
		operator Index_t() const { return m_index; }

	protected:
		Index_t m_index;
	};

	/**
	 * Hashes a handle of a given type.
	 */
	template<typename T>
	struct HandleHasher
	{
		std::size_t operator()(const T &handle) const { return static_cast<std::size_t>(handle); }
	};  // struct HandleHasher

	/**
	 * Represents a point.
	 */
	struct Point
	{
		std::unordered_set<Index_t> m_edges;  ///< Contains a set of edges that leave the point.
	};

	/**
	 * Represents a split point.
	 */
	struct SplitPoint
	{
		SplitPoint()
		    : m_point(s_invalidIndex),
		      m_face(s_invalidIndex),
		      m_incomingEdge(s_invalidIndex),
		      m_outgoingEdge(s_invalidIndex)
		{
		}

		Index_t m_point;         ///< \c Point to which the \c SplitPoint belongs.
		Index_t m_face;          ///< \c Face to which the \c SplitPoint belongs.
		Index_t m_incomingEdge;  ///< \c Edge that has this \c SplitPoint as its destination.
		Index_t m_outgoingEdge;  ///< \c Edge that has this \c SplitPoint as its source.
	};

	/**
	 * Represents an \c Edge.
	 */
	struct Edge
	{
		Edge() : m_source(s_invalidIndex), m_destination(s_invalidIndex) {}
		Index_t m_source;       ///< Source \c SplitPoint.
		Index_t m_destination;  ///< Destination \c SplitPoint.
	};

	/**
	 * Represents a \c Face.
	 */
	struct Face
	{
		Face() : m_splitPoint(s_invalidIndex) {}
		Index_t m_splitPoint;  ///< A \c SplitPoint for this \c Face.
	};

	using PointContainer_t = AssociativeIndexedContainer<Index_t, Point>;
	using SplitPointContainer_t = AssociativeIndexedContainer<Index_t, SplitPoint>;
	using EdgeContainer_t = AssociativeIndexedContainer<Index_t, Edge>;
	using FaceContainer_t = AssociativeIndexedContainer<Index_t, Face>;

public:
	/**
	 * Handle to a \c Point.
	 */
	struct PointHandle : public Handle
	{
		PointHandle(const Index_t &index) : Handle(index) {}

		/**
		 * Implicit cast to an \c Index_t
		 */
		operator Index_t() const { return m_index; }

		bool operator==(const PointHandle &p) const { return m_index == p.m_index; }
		bool operator!=(const PointHandle &p) const { return m_index != p.m_index; }
	};

	/**
	 * Handle to a \c SplitPoint.
	 */
	struct SplitPointHandle : public Handle
	{
		SplitPointHandle(const Index_t &index) : Handle(index) {}

		/**
		 * Implicit cast to an \c Index_t
		 */
		operator Index_t() const { return m_index; }

		bool operator==(const SplitPointHandle &s) const { return m_index == s.m_index; }
		bool operator!=(const SplitPointHandle &s) const { return m_index != s.m_index; }
	};

	/**
	 * Handle to an \c Edge
	 */
	struct EdgeHandle : public Handle
	{
		EdgeHandle(const Index_t &index) : Handle(index) {}

		/**
		 * Implicit cast to an \c Index_t
		 */
		operator Index_t() const { return m_index; }

		bool operator==(const EdgeHandle &e) const { return m_index == e.m_index; }
		bool operator!=(const EdgeHandle &e) const { return m_index != e.m_index; }
	};

	/**
	 * Handle to a \c Face.
	 */
	struct FaceHandle : public Handle
	{
		FaceHandle(const Index_t &index) : Handle(index) {}

		/**
		 * Implicit cast to an \c Index_t
		 */
		operator Index_t() const { return m_index; }

		bool operator==(const FaceHandle &f) const { return m_index == f.m_index; }
		bool operator!=(const FaceHandle &f) const { return m_index != f.m_index; }
	};

	/// Set of \c PointHandle.
	using PointHandleSet = std::unordered_set<PointHandle, HandleHasher<PointHandle>>;
	/// Set of \c SplitPointHandle.
	using SplitPointHandleSet = std::unordered_set<SplitPointHandle, HandleHasher<SplitPointHandle>>;
	/// Set of \c EdgeHandle.
	using EdgeHandleSet = std::unordered_set<EdgeHandle, HandleHasher<EdgeHandle>>;
	/// Set of \c FaceHandle.
	using FaceHandleSet = std::unordered_set<FaceHandle, HandleHasher<FaceHandle>>;

	std::size_t GetFaceCount() const;
	std::size_t GetPointCount() const;
	std::size_t GetSplitPointCount() const;
	std::size_t GetEdgeCount() const;

	/*
	 * Operations
	 */
	/**
	 * Gets the \c FaceHandle that belongs to the \c PointHandle \p p and is the source \c SplitPoint of \p e.
	 */
	FaceHandle GetFace(const PointHandle &p, const EdgeHandle &e) const;
	/**
	 * Gets a handle to the \c Point that the \c SplitPointHandle \p s belongs to.
	 */
	PointHandle GetPoint(const SplitPointHandle &s) const;
	/**
	 * Gets a handle to the \c Edge that has the \c SplitPoint \p s at its destination.
	 */
	EdgeHandle GetInEdge(const SplitPointHandle &s) const;
	/**
	 * Gets a handle to the \c Edge that has the \c SplitPoint \p s at its source.
	 */
	EdgeHandle GetOutEdge(const SplitPointHandle &s) const;
	/**
	 * Gets a handle to the \c Face that the \c SplitPoint referred to by \p s belongs to.
	 */
	FaceHandle GetFace(const SplitPointHandle &s) const;

	/**
	 * Gets a handle to the \c SplitPoint that is at the source of the \c Edge \p e.
	 */
	SplitPointHandle GetSource(const EdgeHandle &e) const;
	/**
	 * Gets a handle to the \c SplitPoint that is at the destination of the \c Edge \p e.
	 */
	SplitPointHandle GetDestination(const EdgeHandle &e) const;
	/**
	 * Gets a handle to the \c Face that the the \c Edge \p e belongs to.
	 */
	FaceHandle GetFace(const EdgeHandle &e) const;

	/**
	 * Gets a handle to a \c SplitPoint belonging to the \c Face given by \p f.
	 */
	SplitPointHandle GetSplitPoint(const FaceHandle &f) const;
	/**
	 * Gets a handle to an \c Edge belonging to the \c Face given by \p f.
	 */
	EdgeHandle GetEdge(const FaceHandle &f) const;

	/**
	 * Gets the set of \e EdgeHandle that leave the \c Point \p.
	 */
	EdgeHandleSet GetOutEdges(const PointHandle &p);

	/*
	 * Navigating the topology
	 */
	/**
	 * Returns a \c EdgeHandle to the \c Edge at the destination of \p e.
	 */
	EdgeHandle GetNextEdge(const EdgeHandle &e) const;
	/**
	 * Returns a \c EdgeHandle to the \c Edge at the source of \p e.
	 */
	EdgeHandle GetPrevEdge(const EdgeHandle &e) const;

	/**
	 * Returns the next \c Point starting at \p p along the \c Edge \p e.
	 */
	PointHandle GetNextPoint(const PointHandle &p, const EdgeHandle &e) const;
	/**
	 * Returns the previous \c Point starting at \p p along the \c Edge \p e.
	 */
	PointHandle GetPrevPoint(const PointHandle &p, const EdgeHandle &e) const;

	/**
	 * Returns the next \c SplitPoint along the \c Edge that leaves \p s.
	 */
	SplitPointHandle GetNextSplitPoint(const SplitPointHandle &s) const;
	/**
	 * Returns the previous \c SplitPoint along the \c Edge that arrives at \p s.
	 */
	SplitPointHandle GetPrevSplitPoint(const SplitPointHandle &s) const;

	/*
	 * Creating the Topology
	 */
	struct CreateFaceResult
	{
		CreateFaceResult() : m_face(s_invalidIndex), m_splitPoints{s_invalidIndex, s_invalidIndex, s_invalidIndex} {};
		CreateFaceResult(FaceHandle f, std::array<SplitPointHandle, 3> splitPoints)
		    : m_face(f), m_splitPoints(splitPoints)
		{
		}
		FaceHandle m_face;
		std::array<SplitPointHandle, 3> m_splitPoints;
	};

	/**
	 * Creates a \c Face in this topology with 3 \c Point.
	 * All the \c Point are disconnected from the rest of the topology.
	 */
	CreateFaceResult CreateFace();
	/**
	 * Creates a \c Face in this topology with 3 \c Point.
	 * \c Point \p p0 must already be defined in the topology and the remaining 2 \c Point are connected to \p p0.
	 */
	CreateFaceResult CreateFace(const PointHandle &p0);
	/**
	 * Creates a \c Face in this topology with 3 \c Point.
	 * \c Point \p p0 and \p p1 must already be defined in the topology and the remaining \c Point is connected to both.
	 */
	CreateFaceResult CreateFace(const PointHandle &p0, const PointHandle &p1);
	/**
	 * Creates a \c Face in this topology with 3 \c Point.
	 * If any of the points doesn't exist, it is created.
	 */
	CreateFaceResult CreateFace(const PointHandle &p0, const PointHandle &p1, const PointHandle &p2);

	/*
	 * Modifying the topology
	 */
	/**
	 * Splits the \c Edge referenced by \p e creating a new \c Point.
	 */
	SplitPointHandle SplitEdge(const EdgeHandle &e);
	/**
	 * Splits the \c Edge referenced by \p e using a the \c Point \p p already defined.
	 */
	SplitPointHandle SplitEdge(const EdgeHandle &e, const PointHandle &p);
	/**
	 * Collapses the \c Edge referenced by \p e into a single \c Point.
	 * The \c Point the \c Edge is collapsed to is the \c Point at its destination.
	 * The \c SplitPoint at its source is deleted and the deletion is cascaded.
	 */
	SplitPointHandle CollapseEdge(const EdgeHandle &e);

	/**
	 * Splits a face between two split points.
	 * @return \c FaceHandle with referring to the new \c Face.
	 */
	FaceHandle SplitFace(const FaceHandle &f, const EdgeHandle &e0, const EdgeHandle &e1);

	/*
	 * Deleting parts of the topology.
	 */
	/**
	 * Deletes the \c Face referenced by \p f, cascading to other elements.
	 */
	void DeleteFace(const FaceHandle &f);
	/**
	 * Deletes the \c Point referenced by \p p, cascading to other elements.
	 */
	void DeletePoint(const PointHandle &p);
	/**
	 * Deletes the \c Edge referenced by \p e, cascading to other elements.
	 */
	void DeleteEdge(const EdgeHandle &e);
	/**
	 * Deletes the \c SplitPoint referenced by \p s, cascading to other elements.
	 */
	void DeleteSplitPoint(const SplitPointHandle &s);

	/*
	 * Querying the topology.
	 */
	/**
	 * Returns the set of \c Edge that are shared between the \c Point \p p0 and \p p1.
	 */
	EdgeHandleSet GetEdges(const PointHandle &p0, const PointHandle &p1);
	/**
	 * Returns the \c Faces that are adjacent to \c Point \p p.
	 */
	FaceHandleSet GetFaces(const PointHandle &p);
	/**
	 * Returns the \c Faces that are adjacent to both \p p0 and \p p1.
	 */
	FaceHandleSet GetFaces(const PointHandle &p0, const PointHandle &p1);

	bool IsValid();

	/*
	 * Iterators and Circulators.
	 */

	template<typename ContainerType, typename HandleType>
	class Iterator
	{
	public:
		Iterator(const typename ContainerType::iterator &iter) : m_currentIterator(iter) {}

		HandleType operator*() { return HandleType(m_currentIterator->first); }

		HandleType operator->() { return HandleType(m_currentIterator->first); }

		Iterator &operator++()
		{
			++m_currentIterator;
			return *this;
		}

		Iterator &operator++(int)
		{
			++m_currentIterator;
			return *this;
		}

		bool operator==(const Iterator &other) { return m_currentIterator == other.m_currentIterator; }

		bool operator!=(const Iterator &other) { return m_currentIterator != other.m_currentIterator; }

	private:
		typename ContainerType::iterator m_currentIterator;
	};

	using PointIterator = Iterator<PointContainer_t, PointHandle>;
	PointIterator PointsBegin();
	PointIterator PointsEnd();

	using SplitPointIterator = Iterator<SplitPointContainer_t, SplitPointHandle>;
	SplitPointIterator SplitPointsBegin();
	SplitPointIterator SplitPointsEnd();

	using EdgeIterator = Iterator<EdgeContainer_t, EdgeHandle>;
	EdgeIterator EdgesBegin();
	EdgeIterator EdgesEnd();

	using FaceIterator = Iterator<FaceContainer_t, FaceHandle>;
	FaceIterator FacesBegin();
	FaceIterator FacesEnd();

	class PointEdgeIterator
	{
	public:
		PointEdgeIterator(const std::unordered_set<Index_t>::iterator &iter) : m_currentIterator(iter) {}

		EdgeHandle operator*() { return EdgeHandle(*m_currentIterator); }
		EdgeHandle operator->() { return EdgeHandle(*m_currentIterator); }

		PointEdgeIterator &operator++()
		{
			++m_currentIterator;
			return *this;
		}

		PointEdgeIterator &operator++(int)
		{
			++m_currentIterator;
			return *this;
		}

		bool operator==(const PointEdgeIterator &other) { return m_currentIterator == other.m_currentIterator; }
		bool operator!=(const PointEdgeIterator &other) { return m_currentIterator != other.m_currentIterator; }

	private:
		std::unordered_set<Index_t>::iterator m_currentIterator;
	};

	PointEdgeIterator PointEdgesBegin(const PointHandle &p);
	PointEdgeIterator PointEdgesEnd(const PointHandle &p);

	class PointSplitPointIterator
	{
	public:
		PointSplitPointIterator(const SplitPointTopology *topology, const PointEdgeIterator &pointEdgeIterator)
		    : m_topology(topology), m_currentIterator(pointEdgeIterator)
		{
		}

		SplitPointHandle operator*() { return m_topology->GetSource((*m_currentIterator).GetIndex()); }
		SplitPointHandle operator->() { return m_topology->GetSource((*m_currentIterator).GetIndex()); }

		PointSplitPointIterator &operator++()
		{
			++m_currentIterator;
			return *this;
		}

		PointSplitPointIterator &operator++(int)
		{
			++m_currentIterator;
			return *this;
		}

		bool operator==(const PointSplitPointIterator &other) { return m_currentIterator == other.m_currentIterator; }
		bool operator!=(const PointSplitPointIterator &other) { return m_currentIterator != other.m_currentIterator; }

	private:
		const SplitPointTopology *m_topology;
		PointEdgeIterator m_currentIterator;
	};

	PointSplitPointIterator PointSplitPointBegin(const PointHandle &p);
	PointSplitPointIterator PointSplitPointEnd(const PointHandle &p);

	class PointFaceIterator
	{
	public:
		PointFaceIterator(const SplitPointTopology *topology, const PointEdgeIterator &pointEdgeIterator)
		    : m_topology(topology), m_currentIterator(pointEdgeIterator)
		{
		}

		FaceHandle operator*() { return m_topology->GetFace(EdgeHandle((*m_currentIterator).GetIndex())); }
		FaceHandle operator->() { return m_topology->GetFace(EdgeHandle((*m_currentIterator).GetIndex())); }

		PointFaceIterator &operator++()
		{
			++m_currentIterator;
			return *this;
		}

		PointFaceIterator &operator++(int)
		{
			++m_currentIterator;
			return *this;
		}

		bool operator==(const PointFaceIterator &other) { return m_currentIterator == other.m_currentIterator; }
		bool operator!=(const PointFaceIterator &other) { return m_currentIterator != other.m_currentIterator; }

	private:
		const SplitPointTopology *m_topology;
		PointEdgeIterator m_currentIterator;
	};

	PointFaceIterator PointFaceBegin(const PointHandle &p);
	PointFaceIterator PointFaceEnd(const PointHandle &p);

	class FaceEdgeCirculator
	{
	public:
		FaceEdgeCirculator(const SplitPointTopology *topology, const FaceHandle &faceHandle)
		    : m_topology(topology),
		      m_currentEdge(m_topology->GetEdge(faceHandle)),
		      m_lastEdge(m_currentEdge),
		      m_initialPosition(true)
		{
		}

		bool IsValid() const { return m_initialPosition || m_currentEdge != m_lastEdge; }

		operator bool() const { return IsValid(); }

		EdgeHandle operator*() { return m_currentEdge; }
		EdgeHandle operator->() { return m_currentEdge; }

		FaceEdgeCirculator &operator++()
		{
			m_currentEdge = m_topology->GetNextEdge(m_currentEdge);
			m_initialPosition = false;
			return *this;
		}
		FaceEdgeCirculator &operator++(int)
		{
			m_currentEdge = m_topology->GetNextEdge(m_currentEdge);
			m_initialPosition = false;
			return *this;
		}

		bool operator==(const FaceEdgeCirculator &other) { return m_currentEdge == other.m_currentEdge; }
		bool operator!=(const FaceEdgeCirculator &other) { return m_currentEdge != other.m_currentEdge; }

	private:
		const SplitPointTopology *m_topology;
		EdgeHandle m_currentEdge;
		EdgeHandle m_lastEdge;
		bool m_initialPosition;
	};

	FaceEdgeCirculator FaceEdgeCirculatorBegin(const FaceHandle &f);

	class FaceSplitPointCirculator
	{
	public:
		FaceSplitPointCirculator(const SplitPointTopology *topology, const FaceEdgeCirculator &circulator)
		    : m_topology(topology), m_currentCirculator(circulator)
		{
		}

		bool IsValid() const { return m_currentCirculator.IsValid(); }

		operator bool() const { return IsValid(); }

		SplitPointHandle operator*() { return m_topology->GetSource((*m_currentCirculator).GetIndex()); }
		SplitPointHandle operator->() { return m_topology->GetSource((*m_currentCirculator).GetIndex()); }

		FaceSplitPointCirculator &operator++()
		{
			++m_currentCirculator;
			return *this;
		}
		FaceSplitPointCirculator &operator++(int)
		{
			++m_currentCirculator;
			return *this;
		}

		bool operator==(const FaceSplitPointCirculator &other)
		{
			return m_currentCirculator == other.m_currentCirculator;
		}
		bool operator!=(const FaceSplitPointCirculator &other)
		{
			return m_currentCirculator != other.m_currentCirculator;
		}

	private:
		const SplitPointTopology *m_topology;
		FaceEdgeCirculator m_currentCirculator;
	};

	FaceSplitPointCirculator FaceSplitPointCirculatorBegin(const FaceHandle &f);

	class FacePointCirculator
	{
	public:
		FacePointCirculator(const SplitPointTopology *topology, const FaceEdgeCirculator &circulator)
		    : m_topology(topology), m_currentCirculator(circulator)
		{
		}

		bool IsValid() const { return m_currentCirculator.IsValid(); }

		operator bool() const { return IsValid(); }

		PointHandle operator*()
		{
			return m_topology->GetPoint(m_topology->GetSource((*m_currentCirculator).GetIndex()));
		}
		PointHandle operator->()
		{
			return m_topology->GetPoint(m_topology->GetSource((*m_currentCirculator).GetIndex()));
		}

		FacePointCirculator &operator++()
		{
			++m_currentCirculator;
			return *this;
		}
		FacePointCirculator &operator++(int)
		{
			++m_currentCirculator;
			return *this;
		}

		bool operator==(const FacePointCirculator &other) { return m_currentCirculator == other.m_currentCirculator; }
		bool operator!=(const FacePointCirculator &other) { return m_currentCirculator != other.m_currentCirculator; }

	private:
		const SplitPointTopology *m_topology;
		FaceEdgeCirculator m_currentCirculator;
	};

	FacePointCirculator FacePointCirculatorBegin(const FaceHandle &f);

	void DumpToStream(std::ostream &outStream);

private:
	template<int size>
	using IndexPointPairArray_t = std::array<PointContainer_t::IndexValuePair_t, size>;
	template<int size>
	using IndexSplitPointPairArray_t = std::array<SplitPointContainer_t::IndexValuePair_t, size>;
	template<int size>
	using IndexEdgePairArray_t = std::array<EdgeContainer_t::IndexValuePair_t, size>;
	template<int size>
	using IndexFacePairArray_t = std::array<FaceContainer_t::IndexValuePair_t, size>;

	CreateFaceResult CreateFace(const IndexPointPairArray_t<3> &points,
	                            const IndexSplitPointPairArray_t<3> &splitPoints, const IndexEdgePairArray_t<3> &edges);

	void SetOutgoingEdge(SplitPoint &splitPoint, Edge &edge, const SplitPointHandle &s, const EdgeHandle &e);
	void SetIncomingEdge(SplitPoint &splitPoint, Edge &edge, const SplitPointHandle &s, const EdgeHandle &e);
	void SetSource(Edge &edge, SplitPoint &splitPoint, const EdgeHandle &e, const SplitPointHandle &s);
	void SetDestination(Edge &edge, SplitPoint &splitPoint, const EdgeHandle &e, const SplitPointHandle &s);
	void SetFace(SplitPoint &splitPoint, Face &face, const SplitPointHandle &s, const FaceHandle &f);
	void SetSplitPoint(Face &face, SplitPoint &splitPoint, const FaceHandle &f, const SplitPointHandle &s);
	void SetPoint(SplitPoint &splitPoint, Point &point, const SplitPointHandle &s, const PointHandle &p);
	void AddEdge(const PointHandle &p, const EdgeHandle &e);

	bool ValidatePoint(const Index_t &p);
	bool ValidateSplitPoint(const Index_t &s);
	bool ValidateEdge(const Index_t &e);
	bool ValidateFace(const Index_t &f);

	PointContainer_t m_points;
	SplitPointContainer_t m_splitPoints;
	EdgeContainer_t m_edges;
	FaceContainer_t m_faces;
};

}  // namespace selector

#endif
