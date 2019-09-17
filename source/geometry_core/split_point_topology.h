#ifndef SELECTOR_SPLIT_POINT_TOPOLOGY_H_
#define SELECTOR_SPLIT_POINT_TOPOLOGY_H_

#include "indexed_container.h"

#include <array>
#include <unordered_set>

namespace selector
{
/**
 * Implements a Split Point Topology to be used by \c Geometry.
 */
class SplitPointTopology
{
public:
	using Index_t = uint32_t;  ///< Type used by indices.

private:
	struct Handle
	{
		explicit Handle(const Index_t &index) : m_index(index) {}
		Index_t GetIndex() const { return m_index; }

	protected:
		Index_t m_index;
	};

	template<typename T>
	struct HandleHasher
	{
		std::size_t operator()(const T &handle) { return static_cast<std::size_t>(handle.GetIndex()); }
	};  // struct HandleHasher
public:
	struct PointHandle : public Handle
	{
		explicit PointHandle(const Index_t &index) : Handle(index) {}

		bool operator==(const PointHandle &p) const { return m_index == p.m_index; }
		bool operator!=(const PointHandle &p) const { return m_index != p.m_index; }
	};

	struct SplitPointHandle : public Handle
	{
		explicit SplitPointHandle(const Index_t &index) : Handle(index) {}

		bool operator==(const SplitPointHandle &s) const { return m_index == s.m_index; }
		bool operator!=(const SplitPointHandle &s) const { return m_index != s.m_index; }
	};

	struct EdgeHandle : public Handle
	{
		explicit EdgeHandle(const Index_t &index) : Handle(index) {}

		bool operator==(const EdgeHandle &e) const { return m_index == e.m_index; }
		bool operator!=(const EdgeHandle &e) const { return m_index != e.m_index; }
	};

	struct FaceHandle : public Handle
	{
		explicit FaceHandle(const Index_t &index) : Handle(index) {}

		bool operator==(const FaceHandle &f) const { return m_index == f.m_index; }
		bool operator!=(const FaceHandle &f) const { return m_index != f.m_index; }
	};

	using PointHandleSet = std::unordered_set<PointHandle, HandleHasher<PointHandle>>;
	using SplitPointHandleSet = std::unordered_set<SplitPointHandle, HandleHasher<SplitPointHandle>>;
	using EdgeHandleSet = std::unordered_set<EdgeHandle, HandleHasher<EdgeHandle>>;
	using FaceHandleSet = std::unordered_set<FaceHandle, HandleHasher<FaceHandle>>;

	SplitPointTopology();
	~SplitPointTopology();

	/*
	 * Operations
	 */
	SplitPointHandle GetSplitPoint(const PointHandle &p, const EdgeHandle &e) const;
	FaceHandle GetFace(const PointHandle &p, const EdgeHandle &e) const;

	PointHandle GetPoint(const SplitPointHandle &s) const;
	EdgeHandle GetInEdge(const SplitPointHandle &s) const;
	EdgeHandle GetOutEdge(const SplitPointHandle &s) const;
	FaceHandle GetFace(const SplitPointHandle &s) const;

	SplitPointHandle GetSource(const EdgeHandle &e) const;
	SplitPointHandle GetDestination(const EdgeHandle &e) const;
	FaceHandle GetFace(const EdgeHandle &e) const;

	SplitPointHandle GetSplitPoint(const FaceHandle &f) const;
	EdgeHandle GetEdge(const FaceHandle &f) const;

	/*
	 * Navigating the topology
	 */
	EdgeHandle GetNextEdge(const EdgeHandle &e) const;
	EdgeHandle GetPrevEdge(const EdgeHandle &e) const;

	PointHandle GetNextPoint(const PointHandle &p, const EdgeHandle &e) const;
	PointHandle GetPrevPoint(const PointHandle &p, const EdgeHandle &e) const;

	SplitPointHandle GetNextSplitPoint(const SplitPointHandle &s) const;
	SplitPointHandle GetPrevSplitPoint(const SplitPointHandle &s) const;

	/*
	 * Creating the Topology
	 */
	struct CreateFaceResult
	{
		FaceHandle m_face;
		std::array<PointHandle, 3> m_points;
	};

	CreateFaceResult CreateFace();
	CreateFaceResult CreateFace(const PointHandle &p0);
	CreateFaceResult CreateFace(const PointHandle &p0, const PointHandle &p1);
	CreateFaceResult CreateFace(const PointHandle &p0, const PointHandle &p1, const PointHandle &p2);

	SplitPointHandle AddPointToFace(const FaceHandle &f, const EdgeHandle &e);
	SplitPointHandle AddPointToFace(const FaceHandle &f, const EdgeHandle &e, const PointHandle &p);

	/*
	 * Modifying the topology
	 */
	SplitPointHandle SplitEdge(const EdgeHandle &e);
	SplitPointHandle CollapseEdge(const EdgeHandle &e);
	SplitPointHandle CollapseEdge(const EdgeHandle &e, const PointHandle &p);

	struct SplitFaceResult
	{
		std::array<FaceHandle, 2> m_faces;
		std::array<EdgeHandle, 2> m_edges;
	};
	SplitFaceResult SplitFace(const FaceHandle &f, const EdgeHandle &e0, const EdgeHandle &e1);

	/*
	 * Deleting parts of the topology.
	 */
	void DeleteFace(const FaceHandle &f);
	void DeletePoint(const PointHandle &p);
	void DeleteEdge(const EdgeHandle &e);
	void DeleteSplitPoint(const SplitPointHandle &s);

	/*
	 * Querying the topology.
	 */
	EdgeHandleSet GetEdges(const PointHandle &p0, const PointHandle &p1);
	FaceHandleSet GetFaces(const PointHandle &p);
	FaceHandleSet GetFaces(const PointHandle &p0, const PointHandle &p1);
	EdgeHandleSet GetSharedEdges(const FaceHandle &f0, const FaceHandle &f1);
	bool IsValid();

private:
	struct Point
	{
		std::unordered_set<Index_t> m_edges;
	};

	struct SplitPoint
	{
		Index_t m_point;
		Index_t m_face;
		Index_t m_incomingEdge;
		Index_t m_outgoingEdge;
	};

	struct Edge
	{
		Index_t m_source;
		Index_t m_destination;
	};

	struct Face
	{
		Index_t m_splitPoint;
	};

	using PointContainer_t = AssociativeIndexedContainer<Index_t, Point>;
	using SplitPointContainer_t = AssociativeIndexedContainer<Index_t, SplitPoint>;
	using EdgeContainer_t = AssociativeIndexedContainer<Index_t, Edge>;
	using FaceContainer_t = AssociativeIndexedContainer<Index_t, Face>;

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
