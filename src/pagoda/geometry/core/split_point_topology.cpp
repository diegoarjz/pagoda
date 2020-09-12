#include "split_point_topology.h"

#include <pagoda/common/debug/logger.h>

namespace pagoda::geometry::core
{
const SplitPointTopology::Index_t SplitPointTopology::s_invalidIndex =
  std::numeric_limits<SplitPointTopology::Index_t>::max();

std::size_t SplitPointTopology::GetFaceCount() const { return m_faces.Count(); }
std::size_t SplitPointTopology::GetPointCount() const { return m_points.Count(); }
std::size_t SplitPointTopology::GetSplitPointCount() const { return m_splitPoints.Count(); }
std::size_t SplitPointTopology::GetEdgeCount() const { return m_edges.Count(); }

/*
 * Operations
 */
SplitPointTopology::PointHandle SplitPointTopology::GetPoint(const SplitPointHandle &s) const
{
	return PointHandle(m_splitPoints.Get(s).m_point);
}

SplitPointTopology::EdgeHandle SplitPointTopology::GetInEdge(const SplitPointHandle &s) const
{
	return EdgeHandle(m_splitPoints.Get(s).m_incomingEdge);
}

SplitPointTopology::EdgeHandle SplitPointTopology::GetOutEdge(const SplitPointHandle &s) const
{
	return EdgeHandle(m_splitPoints.Get(s).m_outgoingEdge);
}

SplitPointTopology::FaceHandle SplitPointTopology::GetFace(const SplitPointHandle &s) const
{
	return FaceHandle(m_splitPoints.Get(s).m_face);
}

SplitPointTopology::SplitPointHandle SplitPointTopology::GetSource(const EdgeHandle &e) const
{
	return SplitPointHandle(m_edges.Get(e).m_source);
}

SplitPointTopology::SplitPointHandle SplitPointTopology::GetDestination(const EdgeHandle &e) const
{
	return SplitPointHandle(m_edges.Get(e).m_destination);
}

SplitPointTopology::FaceHandle SplitPointTopology::GetFace(const EdgeHandle &e) const
{
	return FaceHandle(m_splitPoints.Get(GetSource(e)).m_face);
}

SplitPointTopology::SplitPointHandle SplitPointTopology::GetSplitPoint(const FaceHandle &f) const
{
	return SplitPointHandle(m_faces.Get(f).m_splitPoint);
}

SplitPointTopology::EdgeHandle SplitPointTopology::GetEdge(const FaceHandle &f) const
{
	return EdgeHandle(GetOutEdge(GetSplitPoint(FaceHandle(f))));
}

SplitPointTopology::EdgeHandleSet SplitPointTopology::GetOutEdges(const PointHandle &p)
{
	EdgeHandleSet result;
	const auto &point = m_points.Get(p);

	for (const auto &e : point.m_edges) {
		result.insert(e);
	}

	return result;
}

/*
 * Navigating the topology
 */
SplitPointTopology::EdgeHandle SplitPointTopology::GetNextEdge(const EdgeHandle &e) const
{
	auto &edge = m_edges.Get(e);
	auto &nextSplitPoint = m_splitPoints.Get(edge.m_destination);
	return nextSplitPoint.m_outgoingEdge;
}

SplitPointTopology::EdgeHandle SplitPointTopology::GetPrevEdge(const EdgeHandle &e) const
{
	auto &edge = m_edges.Get(e);
	auto &splitPoint = m_splitPoints.Get(edge.m_source);
	return splitPoint.m_incomingEdge;
}

SplitPointTopology::PointHandle SplitPointTopology::GetNextPoint(const PointHandle &p, const EdgeHandle &e) const
{
	auto &edge = m_edges.Get(e);
	auto &nextSplitPoint = m_splitPoints.Get(edge.m_destination);
	return nextSplitPoint.m_point;
}

SplitPointTopology::PointHandle SplitPointTopology::GetPrevPoint(const PointHandle &p, const EdgeHandle &e) const
{
	auto &edge = m_edges.Get(e);
	auto &splitPoint = m_splitPoints.Get(edge.m_source);
	return splitPoint.m_point;
}

SplitPointTopology::SplitPointHandle SplitPointTopology::GetNextSplitPoint(const SplitPointHandle &s) const
{
	auto &splitPoint = m_splitPoints.Get(s);
	auto &edge = m_edges.Get(splitPoint.m_outgoingEdge);
	return edge.m_destination;
}

SplitPointTopology::SplitPointHandle SplitPointTopology::GetPrevSplitPoint(const SplitPointHandle &s) const
{
	auto &splitPoint = m_splitPoints.Get(s);
	auto &prevEdge = m_edges.Get(splitPoint.m_incomingEdge);
	return prevEdge.m_source;
}

/*
 * Creating the Topology
 */

SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace()
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Creating a Face with 3 new points");

	// Points
	PointHandleArray_t points = {m_points.Create(), m_points.Create(), m_points.Create()};

	// SplitPoints
	SplitPointHandleArray_t splitPoints = {m_splitPoints.Create(), m_splitPoints.Create(), m_splitPoints.Create()};

	// Edges
	EdgeHandleArray_t edges = {m_edges.Create(), m_edges.Create(), m_edges.Create()};

	return CreateFace(points, splitPoints, edges);
}

SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace(const PointHandle &p0)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Creating a face with point " << p0.GetIndex() << " and 2 new points");
	// Points
	PointHandleArray_t points = {p0, m_points.Create(), m_points.Create()};

	// SplitPoints
	SplitPointHandleArray_t splitPoints = {m_splitPoints.Create(), m_splitPoints.Create(), m_splitPoints.Create()};

	// Edges
	EdgeHandleArray_t edges = {m_edges.Create(), m_edges.Create(), m_edges.Create()};

	return CreateFace(points, splitPoints, edges);
}

SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace(const PointHandle &p0, const PointHandle &p1)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore,
	          "Creating a face with points " << p0.GetIndex() << " and " << p1.GetIndex() << " and 1 new point");
	// Points
	PointHandleArray_t points = {p0, p1, m_points.Create()};

	// SplitPoints
	SplitPointHandleArray_t splitPoints = {m_splitPoints.Create(), m_splitPoints.Create(), m_splitPoints.Create()};

	// Edges
	EdgeHandleArray_t edges = {m_edges.Create(), m_edges.Create(), m_edges.Create()};

	return CreateFace(points, splitPoints, edges);
}

SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace(const PointHandle &p0, const PointHandle &p1,
                                                                    const PointHandle &p2)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore,
	          "Creating a face with points " << p0.GetIndex() << "," << p1.GetIndex() << " and " << p2.GetIndex());
	auto getOrCreatePoint = [this](const PointHandle &p) {
		if (p.GetIndex() == s_invalidIndex) {
			return m_points.Create();
		}
		return p.GetIndex();
	};
	// Points
	PointHandleArray_t points = {getOrCreatePoint(p0), getOrCreatePoint(p1), getOrCreatePoint(p2)};

	// SplitPoints
	SplitPointHandleArray_t splitPoints = {m_splitPoints.Create(), m_splitPoints.Create(), m_splitPoints.Create()};

	// Edges
	EdgeHandleArray_t edges = {m_edges.Create(), m_edges.Create(), m_edges.Create()};

	return CreateFace(points, splitPoints, edges);
}

SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace(const std::vector<PointHandle> &points)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Creating face with multiple points.");

	auto getOrCreatePoint = [this](const PointHandle &p) {
		if (p.GetIndex() == s_invalidIndex) {
			return m_points.Create();
		}
		return p.GetIndex();
	};

	PointHandleArray_t facePoints;
	facePoints.reserve(points.size());
	SplitPointHandleArray_t splitPoints;
	splitPoints.reserve(points.size());
	EdgeHandleArray_t edges;
	edges.reserve(points.size());

	for (const auto &p : points) {
		facePoints.emplace_back(getOrCreatePoint(p));
		splitPoints.emplace_back(m_splitPoints.Create());
		edges.emplace_back(m_edges.Create());
	}

	return CreateFace(facePoints, splitPoints, edges);
}

SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace(const PointHandleArray_t &points,
                                                                    const SplitPointHandleArray_t &splitPoints,
                                                                    const EdgeHandleArray_t &edges)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Creating a face");
#ifdef DEBUG
	LOG_TRACE(GeometryCore, "Points");
	for (const auto &p : points) {
		LOG_TRACE(GeometryCore, " Point " << p);
	}
	LOG_TRACE(GeometryCore, "SplitPoints");
	for (const auto &s : splitPoints) {
		LOG_TRACE(GeometryCore, " SplitPoint " << s);
	}
	LOG_TRACE(GeometryCore, "Edges");
	for (const auto &e : edges) {
		LOG_TRACE(GeometryCore, " Edge " << e);
	}
#endif

	FaceContainer_t::IndexValuePair face = m_faces.CreateAndGet();
	LOG_TRACE(GeometryCore, "New Face " << face.m_index);

	// Connect the elements
	auto size = points.size();
	for (uint32_t i = 0; i < size; ++i) {
		AddEdge(points[i], edges[i]);
		SetOutgoingEdge(m_splitPoints.Get(splitPoints[i]), m_edges.Get(edges[i]), splitPoints[i], edges[i]);
		SetIncomingEdge(m_splitPoints.Get(splitPoints[i]), m_edges.Get(edges[(i - 1) % size]), splitPoints[i],
		                edges[(i - 1) % size]);
		SetPoint(m_splitPoints.Get(splitPoints[i]), m_points.Get(points[i]), splitPoints[i], points[i]);
		SetFace(m_splitPoints.Get(splitPoints[i]), face.m_value, splitPoints[i], face.m_index);
		SetIncomingEdge(m_splitPoints.Get(splitPoints[(i + 1) % size]), m_edges.Get(edges[i]), splitPoints[(i + 1) % size],
		                edges[i]);
	}
	face.m_value.m_splitPoint = splitPoints[0];

#ifdef DEBUG
	LOG_TRACE(GeometryCore, "CreateFace: IsValid: " << IsValid());
#endif
	return CreateFaceResult(face.m_index, {splitPoints[0], splitPoints[1], splitPoints[2]});
}

void SplitPointTopology::SetOutgoingEdge(SplitPoint &splitPoint, Edge &edge, const SplitPointHandle &s,
                                         const EdgeHandle &e)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Setting outgoing edge of SplitPoint " << s << " to " << e);
	splitPoint.m_outgoingEdge = e;
	edge.m_source = s;
}

void SplitPointTopology::SetIncomingEdge(SplitPoint &splitPoint, Edge &edge, const SplitPointHandle &s,
                                         const EdgeHandle &e)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Setting incoming edge of SplitPoint " << s << " to " << e);
	splitPoint.m_incomingEdge = e;
	edge.m_destination = s;
}

void SplitPointTopology::SetSource(Edge &edge, SplitPoint &splitPoint, const EdgeHandle &e, const SplitPointHandle &s)
{
	SetIncomingEdge(splitPoint, edge, s, e);
}

void SplitPointTopology::SetDestination(Edge &edge, SplitPoint &splitPoint, const EdgeHandle &e,
                                        const SplitPointHandle &s)
{
	SetOutgoingEdge(splitPoint, edge, s, e);
}

void SplitPointTopology::SetFace(SplitPoint &splitPoint, Face &face, const SplitPointHandle &s, const FaceHandle &f)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Setting face of SplitPoint " << s << " to " << f);
	splitPoint.m_face = f;
	face.m_splitPoint = s;
}

void SplitPointTopology::SetSplitPoint(Face &face, SplitPoint &splitPoint, const FaceHandle &f,
                                       const SplitPointHandle &s)
{
	SetFace(splitPoint, face, s, f);
}

void SplitPointTopology::SetPoint(SplitPoint &splitPoint, Point &point, const SplitPointHandle &s, const PointHandle &p)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Setting point of SplitPoint " << s << " to " << p);
	splitPoint.m_point = p;
	point.m_edges.insert(splitPoint.m_outgoingEdge);
}

void SplitPointTopology::AddEdge(const PointHandle &p, const EdgeHandle &e)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Adding Edge " << e << " to Point " << p);
	Point &point = m_points.Get(p);
	point.m_edges.insert(e);
}

/*
 * Modifying the topology
 */
SplitPointTopology::SplitPointHandle SplitPointTopology::SplitEdge(const EdgeHandle &e)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Splitting Edge " << e << " with a new Point");
	auto newPoint = m_points.Create();
	return SplitEdge(e, newPoint);
}

SplitPointTopology::SplitPointHandle SplitPointTopology::SplitEdge(const EdgeHandle &e, const PointHandle &p)
{
	/*
	 *   A----B             A----B
	 *   |   /              |    |
	 *   |  /e       =>     |    |e
	 *   | /                |    |
	 *   C    p             C----p
	 *                        f
	 */
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Splitting Edge " << e << " with Point " << p);

	SplitPointHandle splitPointC = GetDestination(e);
	auto edgeF = m_edges.Create();
	auto splitPointP = m_splitPoints.Create();
	auto &splitPoint_splitPointP = m_splitPoints.Get(splitPointP);
	auto &splitPoint_splitPointC = m_splitPoints.Get(splitPointC);
	auto &edge_e = m_edges.Get(e);
	auto &edge_F = m_edges.Get(edgeF);
	auto &face = m_faces.Get(splitPoint_splitPointC.m_face);

	SetSource(edge_e, splitPoint_splitPointP, e, splitPointP);
	SetFace(splitPoint_splitPointP, face, splitPointP, splitPoint_splitPointC.m_face);
	SetSource(edge_F, splitPoint_splitPointC, edgeF, splitPointC);
	SetDestination(edge_F, splitPoint_splitPointP, edgeF, splitPointP);

	SetPoint(splitPoint_splitPointP, m_points.Get(p), splitPointP, p);

#ifdef DEBUG
	IsValid();
#endif

	return splitPointP;
}

SplitPointTopology::SplitPointHandle SplitPointTopology::CollapseEdge(const EdgeHandle &e)
{
	/*
	 *         prevEdge
	 *    ~ A-----B             ~ A    B
	 *            |                 \
	 *            | e     =>         \ prevEdge
	 *            |                   \
	 *      ~ ----C             ~ -----C
	 *         nextEdge              nextEdge
	 */
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Collapsing Edge " << e);

	auto splitPointB = GetSource(e);
	auto splitPointC = GetDestination(e);
	auto prevEdgeHandle = GetPrevEdge(e);
	auto splitPointAHandle = GetSource(prevEdgeHandle);
	auto pointHandleB = GetPoint(splitPointB);
	auto &pointB = m_points.Get(pointHandleB);
	auto &face = m_faces.Get(GetFace(splitPointAHandle));

	auto &splitPointA = m_splitPoints.Get(splitPointAHandle);
	auto &prevEdge = m_edges.Get(prevEdgeHandle);
	SetOutgoingEdge(splitPointA, prevEdge, splitPointAHandle, prevEdgeHandle);
	auto &splitPoint_splitPointC = m_splitPoints.Get(splitPointC);
	auto &edge_prevEdgeHandle = m_edges.Get(prevEdgeHandle);
	SetIncomingEdge(splitPoint_splitPointC, edge_prevEdgeHandle, splitPointC, prevEdgeHandle);
	face.m_splitPoint = splitPointAHandle;

	m_splitPoints.Delete(splitPointB);
	m_edges.Delete(e);
	pointB.m_edges.erase(e);
	if (pointB.m_edges.size() == 0) {
		m_points.Delete(pointHandleB);
	}

#ifdef DEBUG
	IsValid();
#endif

	return splitPointB;
}

SplitPointTopology::FaceHandle SplitPointTopology::SplitFace(const FaceHandle &f, const EdgeHandle &e0,
                                                             const EdgeHandle &e1)
{
	/*
	 *        e1
	 *(p1)A------B            A/A'-----B
	 *    |      |              |\  f0 |
	 *    |      |              | \    |
	 *    |      |    =>      e4|  \e2 |       Original points (A and D) remain in f0
	 *    |      |e0            | e3\  |
	 *    |      |              |    \ |
	 *    |      |              | f1  \|
	 *    C------D(p0)          C------D/D'
	 *                              e5
	 */

	auto a = GetDestination(e1);
	auto d = GetSource(e0);
	auto e4 = GetNextEdge(e1);
	auto e5 = GetPrevEdge(e0);
	auto p0 = GetPoint(d);
	Point &point_p0 = m_points.Get(p0);
	auto p1 = GetPoint(a);
	Point &point_p1 = m_points.Get(p1);

	LOG_TRACE(GeometryCore, "Splitting face " << f << " for edges " << e0 << " and " << e1);
	// 1. Create face f1
	auto f1 = m_faces.Create();
	auto &face = m_faces.Get(f1);

	LOG_TRACE(GeometryCore, "Created face " << f1);
	// 2. Create split points A' and D'
	auto a_ = m_splitPoints.Create();
	auto d_ = m_splitPoints.Create();
	auto &splitPoint_a_ = m_splitPoints.Get(a_);
	auto &splitPoint_d_ = m_splitPoints.Get(d_);

	LOG_TRACE(GeometryCore, "Created split points " << a_ << " and " << d_);
	// 2.1 Set these split points face to f1
	SetFace(splitPoint_a_, face, a_, f1);
	SetFace(splitPoint_d_, face, d_, f1);
	// 3. Iterate over the edges from e1 to e0 (The edges in the new face)
	auto currentEdge = GetNextEdge(e1);
	auto lastEdge = GetPrevEdge(e0);
	while (currentEdge != lastEdge) {
		LOG_TRACE(GeometryCore, "Iterating edge " << currentEdge);
		// 3.1 Set all the split points to the new face
		SplitPointHandle splitPointHandle = GetDestination(currentEdge);
		SplitPoint &splitPoint = m_splitPoints.Get(splitPointHandle);
		SetFace(splitPoint, face, splitPointHandle, f1);
		currentEdge = GetNextEdge(currentEdge);
	}
	// 4. Create edge e2 and e3
	auto e2 = m_edges.Create();
	auto e3 = m_edges.Create();
	LOG_TRACE(GeometryCore, "Created edges " << e2 << " and " << e3);
	// 4.1 Set A outgoing edge as e2
	auto &splitPoint_a = m_splitPoints.Get(a);
	auto &edge_e2 = m_edges.Get(e2);
	SetOutgoingEdge(splitPoint_a, edge_e2, a, e2);
	// 4.2 Set A' outgoing edge as e4
	auto &edge_e4 = m_edges.Get(e4);
	SetOutgoingEdge(splitPoint_a_, edge_e4, a_, e4);
	auto &edge_e3 = m_edges.Get(e3);
	SetIncomingEdge(splitPoint_a_, edge_e3, a_, e3);
	// 4.3 Set D incoming edge as e2
	auto &splitPoint_d = m_splitPoints.Get(d);
	SetIncomingEdge(splitPoint_d, edge_e2, d, e2);
	// 4.4 Set D' outgoing edge as e3
	SetOutgoingEdge(splitPoint_d_, edge_e3, d_, e3);
	auto &edge_e5 = m_edges.Get(e5);
	SetIncomingEdge(splitPoint_d_, edge_e5, d_, e5);

	SetPoint(splitPoint_a_, point_p1, a_, p1);
	SetPoint(splitPoint_a, point_p1, a, p1);
	SetPoint(splitPoint_d_, point_p0, d_, p0);
	SetPoint(splitPoint_d, point_p0, d, p0);

	// Set the old face's split point to a point that we know is still in the old face
	SetSplitPoint(m_faces.Get(f), splitPoint_a, f, a);

#ifdef DEBUG
	IsValid();
#endif

	return f1;
}

/*
 * Deleting parts of the topology.
 */
void SplitPointTopology::DeleteFace(const FaceHandle &f)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Deleting Face " << f);

	PointHandleSet pointsToDelete;
	SplitPointHandleSet splitPointsToDelete;
	EdgeHandleSet edgesToDelete;
	for (auto iter = FaceSplitPointCirculatorBegin(f); iter; ++iter) {
		Index_t splitPointIndex = (*iter).GetIndex();
		LOG_TRACE(GeometryCore, " Iterating on SplitPoint " << splitPointIndex);

		SplitPoint &splitPoint = m_splitPoints.Get(splitPointIndex);
		Point &point = m_points.Get(splitPoint.m_point);
		point.m_edges.erase(GetOutEdge(splitPointIndex));

		LOG_TRACE(GeometryCore,
		          "  Point " << splitPoint.m_point << " still has " << point.m_edges.size() << " outgoing edges");

		if (point.m_edges.size() == 0) {
			LOG_TRACE(GeometryCore, "   Adding to delete list");
			pointsToDelete.insert(splitPoint.m_point);
		}
		splitPointsToDelete.insert(splitPointIndex);
		edgesToDelete.insert(splitPoint.m_outgoingEdge);
	}

	for (auto &p : pointsToDelete) {
		LOG_TRACE(GeometryCore, " Deleting Point " << p);
		m_points.Delete(p);
	}
	for (auto &sp : splitPointsToDelete) {
		LOG_TRACE(GeometryCore, " Deleting Split Point " << sp);
		m_splitPoints.Delete(sp);
	}
	for (auto &e : edgesToDelete) {
		LOG_TRACE(GeometryCore, " Deleting Edge " << e);
		m_edges.Delete(e);
	}

	m_faces.Delete(f);
#ifdef DEBUG
	IsValid();
#endif
}

void SplitPointTopology::DeletePoint(const PointHandle &p)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Deleting Point " << p);
	EdgeHandleSet edgesToDelete;
	for (auto iter = PointEdgesBegin(p); iter != PointEdgesEnd(p); ++iter) {
		edgesToDelete.insert((*iter).GetIndex());
	}
	for (const auto &e : edgesToDelete) {
		DeleteEdge(e);
	}
#ifdef DEBUG
	IsValid();
#endif
}

void SplitPointTopology::DeleteEdge(const EdgeHandle &e)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Deleting Edge " << e);
	Edge &edge = m_edges.Get(e);
	SplitPoint &splitPoint = m_splitPoints.Get(edge.m_source);
	DeleteFace(splitPoint.m_face);
#ifdef DEBUG
	IsValid();
#endif
}

void SplitPointTopology::DeleteSplitPoint(const SplitPointHandle &s)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Deleting Split Point " << s);
	SplitPoint &splitPoint = m_splitPoints.Get(s);
	DeleteFace(splitPoint.m_face);
#ifdef DEBUG
	IsValid();
#endif
}

/*
 * Querying the topology.
 */
SplitPointTopology::EdgeHandleSet SplitPointTopology::GetEdges(const PointHandle &p0, const PointHandle &p1)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Getting edges shared by Points " << p0 << " and " << p1);
	EdgeHandleSet edges;
	const Point &point0 = m_points.Get(p0);
	const Point &point1 = m_points.Get(p1);

	std::unordered_set<Index_t> edgesToCheck = point0.m_edges;
	edgesToCheck.insert(std::begin(point1.m_edges), std::end(point1.m_edges));

	for (const auto &edgeIndex : edgesToCheck) {
		const Index_t sourcePointIndex = GetPoint(GetSource(edgeIndex));
		const Index_t destinationPointIndex = GetPoint(GetDestination(edgeIndex));

		if ((sourcePointIndex == p0 && destinationPointIndex == p1) ||
		    (sourcePointIndex == p1 && destinationPointIndex == p0)) {
			edges.insert(EdgeHandle(edgeIndex));
		}
	}

	return edges;
}

SplitPointTopology::FaceHandleSet SplitPointTopology::GetFaces(const PointHandle &p)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Getting Faces adjacent to Point " << p);
	FaceHandleSet faces;

	const Point &point = m_points.Get(p);
	for (const auto &edge : point.m_edges) {
		faces.insert(GetFace(EdgeHandle(edge)));
	}
	return faces;
}

SplitPointTopology::FaceHandleSet SplitPointTopology::GetFaces(const PointHandle &p0, const PointHandle &p1)
{
	START_PROFILE;
	LOG_TRACE(GeometryCore, "Getting Faces shared by Points " << p0 << " and " << p1);
	FaceHandleSet faces;
	EdgeHandleSet edges = GetEdges(p0, p1);

	for (const auto &e : edges) {
		const auto &sourceSplitPoint = m_splitPoints.Get(GetSource(e));
		faces.insert(sourceSplitPoint.m_face);
	}

	return faces;
}

SplitPointTopology::PointIterator SplitPointTopology::PointsBegin() { return PointIterator(m_points.begin()); }

SplitPointTopology::PointIterator SplitPointTopology::PointsEnd() { return PointIterator(m_points.end()); }

SplitPointTopology::SplitPointIterator SplitPointTopology::SplitPointsBegin()
{
	return SplitPointIterator(m_splitPoints.begin());
}

SplitPointTopology::SplitPointIterator SplitPointTopology::SplitPointsEnd()
{
	return SplitPointIterator(m_splitPoints.end());
}

SplitPointTopology::EdgeIterator SplitPointTopology::EdgesBegin() { return EdgeIterator(m_edges.begin()); }

SplitPointTopology::EdgeIterator SplitPointTopology::EdgesEnd() { return EdgeIterator(m_edges.end()); }

SplitPointTopology::FaceIterator SplitPointTopology::FacesBegin() { return FaceIterator(m_faces.begin()); }

SplitPointTopology::FaceIterator SplitPointTopology::FacesEnd() { return FaceIterator(m_faces.end()); }

SplitPointTopology::PointEdgeIterator SplitPointTopology::PointEdgesBegin(const PointHandle &p)
{
	auto &point = m_points.Get(p);
	return PointEdgeIterator(point.m_edges.begin());
}

SplitPointTopology::PointEdgeIterator SplitPointTopology::PointEdgesEnd(const PointHandle &p)
{
	auto &point = m_points.Get(p);
	return PointEdgeIterator(point.m_edges.end());
}

SplitPointTopology::PointSplitPointIterator SplitPointTopology::PointSplitPointBegin(const PointHandle &p)
{
	return PointSplitPointIterator(this, PointEdgesBegin(p));
}

SplitPointTopology::PointSplitPointIterator SplitPointTopology::PointSplitPointEnd(const PointHandle &p)
{
	return PointSplitPointIterator(this, PointEdgesEnd(p));
}

SplitPointTopology::PointFaceIterator SplitPointTopology::PointFaceBegin(const PointHandle &p)
{
	return PointFaceIterator(this, PointEdgesBegin(p));
}

SplitPointTopology::PointFaceIterator SplitPointTopology::PointFaceEnd(const PointHandle &p)
{
	return PointFaceIterator(this, PointEdgesEnd(p));
}

SplitPointTopology::FaceEdgeCirculator SplitPointTopology::FaceEdgeCirculatorBegin(const FaceHandle &f)
{
	return FaceEdgeCirculator(this, f);
}

SplitPointTopology::FaceSplitPointCirculator SplitPointTopology::FaceSplitPointCirculatorBegin(const FaceHandle &f)
{
	return FaceSplitPointCirculator(this, FaceEdgeCirculatorBegin(f));
}

SplitPointTopology::FacePointCirculator SplitPointTopology::FacePointCirculatorBegin(const FaceHandle &f)
{
	return FacePointCirculator(this, FaceEdgeCirculatorBegin(f));
}

bool SplitPointTopology::IsValid()
{
	bool isValid = true;
	for (const auto &p : m_points) {
		isValid = isValid && ValidatePoint(p.m_index);
	}

	for (const auto &s : m_splitPoints) {
		isValid = isValid && ValidateSplitPoint(s.m_index);
	}

	for (const auto &e : m_edges) {
		isValid = isValid && ValidateEdge(e.m_index);
	}

	for (const auto &f : m_faces) {
		isValid = isValid && ValidateFace(f.m_index);
	}

	CRITICAL_ASSERT_MSG(isValid, "Invalid topology");
	return isValid;
}

bool SplitPointTopology::ValidatePoint(const Index_t &p)
{
	if (!m_points.HasIndex(p)) {
		LOG_ERROR("Point " << p << " doesn't exist");
		return false;
	}

	bool isValid = true;
	// Check that all edges exist
	for (const auto &e : m_points.Get(p).m_edges) {
		if (!m_edges.HasIndex(e)) {
			isValid = false;
			LOG_ERROR("Edge " << e << " doesn't exist for point " << p);
		} else {
			if (GetPoint(GetSource(e)).GetIndex() != p) {
				LOG_ERROR("Edge " << e << " referenced by point " << p << " is not an outgoing edge for this point.");
				isValid = false;
			}
		}
	}
	return isValid;
}

bool SplitPointTopology::ValidateSplitPoint(const Index_t &s)
{
	if (!m_splitPoints.HasIndex(s)) {
		LOG_ERROR("Split Point " << s << " doesn't exist.");
		return false;
	}

	bool isValid = true;
	const SplitPoint &splitPoint = m_splitPoints.Get(s);

	// Check that the incoming and outgoing edges exist
	if (!m_edges.HasIndex(splitPoint.m_incomingEdge)) {
		LOG_ERROR("Incoming Edge " << splitPoint.m_incomingEdge << " doesn't exist for Split Point " << s);
		isValid = false;
	}
	if (!m_edges.HasIndex(splitPoint.m_outgoingEdge)) {
		LOG_ERROR("Outgoing Edge " << splitPoint.m_outgoingEdge << " doesn't exist for Split Point " << s);
		isValid = false;
	}

	// Check that the edges reference the split point
	Edge incomingEdge = m_edges.Get(splitPoint.m_incomingEdge);
	Edge outgoingEdge = m_edges.Get(splitPoint.m_outgoingEdge);
	if (incomingEdge.m_destination != s) {
		LOG_ERROR("Incoming Edge " << splitPoint.m_incomingEdge << " of Split Point " << s << " references Split Point "
		                           << incomingEdge.m_destination);
		isValid = false;
	}
	if (outgoingEdge.m_source != s) {
		LOG_ERROR("Outgoing Edge " << splitPoint.m_outgoingEdge << " of Split Point " << s << " references Split Point "
		                           << outgoingEdge.m_source);
		isValid = false;
	}

	// Check that the face exists
	if (!m_faces.HasIndex(splitPoint.m_face)) {
		LOG_ERROR("Face " << splitPoint.m_face << " for Split Point " << s << " doesn't exist");
		isValid = false;
	}

	// Check that the point exists
	if (!m_points.HasIndex(splitPoint.m_point)) {
		LOG_ERROR("Point " << splitPoint.m_point << " for Split Point " << s << " doesn't exist");
		isValid = false;
	}

	return isValid;
}

bool SplitPointTopology::ValidateEdge(const Index_t &e)
{
	if (!m_edges.HasIndex(e)) {
		LOG_ERROR("Edge " << e << " doesn't exist");
		return false;
	}

	bool isValid = true;
	Edge &edge = m_edges.Get(e);

	// Check that the split points exist.
	if (!m_splitPoints.HasIndex(edge.m_source)) {
		LOG_ERROR("Source Split Point " << edge.m_source << " for Edge " << e << " doesn't exist");
		isValid = false;
	}
	if (!m_splitPoints.HasIndex(edge.m_destination)) {
		LOG_ERROR("Destination Split Point " << edge.m_destination << " for Edge " << e << " doesn't exist");
		isValid = false;
	}

	// Check that the split points reference this edge.
	SplitPoint &source = m_splitPoints.Get(edge.m_source);
	SplitPoint &destination = m_splitPoints.Get(edge.m_destination);
	if (source.m_outgoingEdge != e) {
		LOG_ERROR("Source Split Point " << edge.m_source << " doesn't reference Edge " << e << " (references "
		                                << source.m_outgoingEdge << ")");
		isValid = false;
	}
	if (destination.m_incomingEdge != e) {
		LOG_ERROR("Destination Split Point " << edge.m_destination << " doesn't reference Edge " << e << " (references "
		                                     << destination.m_incomingEdge << ")");
		isValid = false;
	}

	// Check that the edge is in the set of edges of the source point
	Point &point = m_points.Get(GetPoint(edge.m_source));
	if (point.m_edges.find(e) == std::end(point.m_edges)) {
		LOG_ERROR("Edge " << e << " is not in the set of outgoing edges of Point " << GetPoint(edge.m_source));
		isValid = false;
	}

	return isValid;
}

bool SplitPointTopology::ValidateFace(const Index_t &f)
{
	if (!m_faces.HasIndex(f)) {
		LOG_ERROR("Face " << f << " doesn't exist");
		return false;
	}

	bool isValid = true;
	Face &face = m_faces.Get(f);

	// Check that the split point exists.
	if (!m_splitPoints.HasIndex(face.m_splitPoint)) {
		LOG_ERROR("Split Point " << face.m_splitPoint << " for Face " << f << " doesn't exist");
	}

	Index_t currentSplitPointIndex = GetNextSplitPoint(SplitPointHandle(face.m_splitPoint));
	uint32_t splitPointCount = 1;

	while (currentSplitPointIndex != face.m_splitPoint) {
		const SplitPoint &splitPoint = m_splitPoints.Get(currentSplitPointIndex);

		// Check that all split points point to this face.
		if (splitPoint.m_face != f) {
			LOG_ERROR("Split Point " << currentSplitPointIndex << " doesn't reference Face " << f << " (references "
			                         << splitPoint.m_face << ")");
			isValid = false;
		}

		++splitPointCount;
		currentSplitPointIndex = GetNextSplitPoint(SplitPointHandle(currentSplitPointIndex));

		if (splitPointCount >= 100) {
			LOG_ERROR("Possible infinite loop in Face " << f);
			isValid = false;
			break;
		}
	}

	// Check that there are at least 3 points in this face.
	if (splitPointCount < 3) {
		LOG_ERROR("Face " << f << " has less than 3 Split Points. It has " << splitPointCount << " Split Points.");
		isValid = false;
	}

	return isValid;
}

void SplitPointTopology::DumpToStream(std::ostream &outStream)
{
	outStream << "IsValid: " << (IsValid() ? "true" : "false") << std::endl;
	outStream << "Points:" << std::endl;
	for (const auto &p : m_points) {
		outStream << " " << p.m_index << " Edges: ";
		for (const auto &e : p.m_value.m_edges) {
			outStream << e << ", ";
		}
		outStream << std::endl;
	}
	outStream << std::endl;

	outStream << "SplitPoints:" << std::endl;
	for (const auto &sp : m_splitPoints) {
		outStream << " " << sp.m_index << ": ";
		outStream << " P: " << sp.m_value.m_point;
		outStream << " F: " << sp.m_value.m_face;
		outStream << " In: " << sp.m_value.m_incomingEdge;
		outStream << " Out: " << sp.m_value.m_outgoingEdge;
		outStream << std::endl;
	}
	outStream << std::endl;

	outStream << "Edges:" << std::endl;
	for (const auto &e : m_edges) {
		outStream << " " << e.m_index << ": ";
		outStream << e.m_value.m_source << " -> " << e.m_value.m_destination;
		outStream << std::endl;
	}
	outStream << std::endl;

	outStream << "Faces:" << std::endl;
	for (const auto &f : m_faces) {
		outStream << " " << f.m_index << ": ";
		outStream << "  SP: " << f.m_value.m_splitPoint;
		outStream << std::endl;
	}
	outStream << std::endl;
}
}  // namespace pagoda::geometry::core
