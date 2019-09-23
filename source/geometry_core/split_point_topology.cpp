#include "split_point_topology.h"

#include "common/logger.h"

namespace selector
{
const SplitPointTopology::Index_t SplitPointTopology::s_invalidIndex =
    std::numeric_limits<SplitPointTopology::Index_t>::max();

SplitPointTopology::SplitPointTopology() {}

SplitPointTopology::~SplitPointTopology() {}

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

	for (const auto &e : point.m_edges)
	{
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
	// Points
	IndexPointPairArray_t<3> points = {m_points.CreateAndGet(), m_points.CreateAndGet(), m_points.CreateAndGet()};

	// SplitPoints
	IndexSplitPointPairArray_t<3> splitPoints = {m_splitPoints.CreateAndGet(), m_splitPoints.CreateAndGet(),
	                                             m_splitPoints.CreateAndGet()};

	// Edges
	IndexEdgePairArray_t<3> edges = {m_edges.CreateAndGet(), m_edges.CreateAndGet(), m_edges.CreateAndGet()};

	return CreateFace(points, splitPoints, edges);
}

SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace(const PointHandle &p0)
{
	// Points
	auto p = m_points.Get(p0);
	IndexPointPairArray_t<3> points = {PointContainer_t::IndexValuePair_t{p0, m_points.Get(p0)},
	                                   m_points.CreateAndGet(), m_points.CreateAndGet()};

	// SplitPoints
	IndexSplitPointPairArray_t<3> splitPoints = {m_splitPoints.CreateAndGet(), m_splitPoints.CreateAndGet(),
	                                             m_splitPoints.CreateAndGet()};

	// Edges
	IndexEdgePairArray_t<3> edges = {m_edges.CreateAndGet(), m_edges.CreateAndGet(), m_edges.CreateAndGet()};

	return CreateFace(points, splitPoints, edges);
}

SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace(const PointHandle &p0, const PointHandle &p1)
{
	// Points
	IndexPointPairArray_t<3> points = {PointContainer_t::IndexValuePair_t{p0, m_points.Get(p0)},
	                                   PointContainer_t::IndexValuePair_t{p1, m_points.Get(p1)},
	                                   m_points.CreateAndGet()};

	// SplitPoints
	IndexSplitPointPairArray_t<3> splitPoints = {m_splitPoints.CreateAndGet(), m_splitPoints.CreateAndGet(),
	                                             m_splitPoints.CreateAndGet()};

	// Edges
	IndexEdgePairArray_t<3> edges = {m_edges.CreateAndGet(), m_edges.CreateAndGet(), m_edges.CreateAndGet()};

	return CreateFace(points, splitPoints, edges);
}

/*
SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace(const PointHandle &p0, const PointHandle &p1,
                                                                    const PointHandle &p2)
{
    // Points
    IndexPointPairArray_t<3> points = {PointContainer_t::IndexValuePair_t{p0, m_points.Get(p0)},
                                       PointContainer_t::IndexValuePair_t{p1, m_points.Get(p1)},
                                       PointContainer_t::IndexValuePair_t{p2, m_points.Get(p2)}};

    // SplitPoints
    IndexSplitPointPairArray_t<3> splitPoints = {m_splitPoints.CreateAndGet(), m_splitPoints.CreateAndGet(),
                                                 m_splitPoints.CreateAndGet()};

    // Edges
    IndexEdgePairArray_t<3> edges = {m_edges.CreateAndGet(), m_edges.CreateAndGet(), m_edges.CreateAndGet()};

    return CreateFace(points, splitPoints, edges);
}
*/

SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace(const IndexPointPairArray_t<3> &points,
                                                                    const IndexSplitPointPairArray_t<3> &splitPoints,
                                                                    const IndexEdgePairArray_t<3> &edges)
{
	FaceContainer_t::IndexValuePair_t face = m_faces.CreateAndGet();
	std::get<1>(face).m_splitPoint = std::get<0>(splitPoints[0]);

	// Connect the elements
	for (uint32_t i = 0; i < 3; ++i)
	{
		auto &point = std::get<1>(points[i]);
		auto &edge = std::get<1>(edges[i]);
		// auto &prevEdge = std::get<1>(edges[(i - 1) % 3]);
		auto &sourceSplitPoint = std::get<1>(splitPoints[i]);
		auto &destinationSplitPoint = std::get<1>(splitPoints[(i + 1) % 3]);

		point.m_edges.insert(std::get<0>(edges[i]));  // TODO: check if the point was inserted

		sourceSplitPoint.m_point = std::get<0>(points[i]);
		sourceSplitPoint.m_face = std::get<0>(face);
		sourceSplitPoint.m_outgoingEdge = std::get<0>(edges[i]);
		sourceSplitPoint.m_incomingEdge = std::get<0>(edges[(i - 1) % 3]);

		destinationSplitPoint.m_incomingEdge = sourceSplitPoint.m_outgoingEdge;

		edge.m_source = std::get<0>(splitPoints[i]);
		edge.m_destination = std::get<0>(splitPoints[(i + 1) % 3]);
	}

	// TODO: Validate face
	// ValidateFace(std::get<0>(face));

	CreateFaceResult result(std::get<0>(face),
	                        {std::get<0>(points[0]), std::get<0>(points[1]), std::get<0>(points[2])});

	return result;
}

void SplitPointTopology::SetOutgoingEdge(const SplitPointHandle &s, const EdgeHandle &e)
{
	auto &splitPoint = m_splitPoints.Get(s);
	auto &edge = m_edges.Get(e);

	splitPoint.m_outgoingEdge = e;
	edge.m_source = s;
}

void SplitPointTopology::SetIncomingEdge(const SplitPointHandle &s, const EdgeHandle &e)
{
	auto &splitPoint = m_splitPoints.Get(s);
	auto &edge = m_edges.Get(e);

	splitPoint.m_incomingEdge = e;
	edge.m_destination = s;
}

void SplitPointTopology::SetSource(const EdgeHandle &e, const SplitPointHandle &s) { SetIncomingEdge(s, e); }

void SplitPointTopology::SetDestination(const EdgeHandle &e, const SplitPointHandle &s) { SetOutgoingEdge(s, e); }

void SplitPointTopology::SetFace(const SplitPointHandle &s, const FaceHandle &f)
{
	auto &splitPoint = m_splitPoints.Get(s);
	auto &face = m_faces.Get(f);

	splitPoint.m_face = f;
	face.m_splitPoint = s;
}

void SplitPointTopology::SetSplitPoint(const FaceHandle &f, const SplitPointHandle &s) { SetFace(s, f); }

void SplitPointTopology::SetPoint(const SplitPointHandle &s, const PointHandle &p)
{
	auto &splitPoint = m_splitPoints.Get(s);
	auto &point = m_points.Get(p);
	splitPoint.m_point = p;
	point.m_edges.insert(splitPoint.m_outgoingEdge);
}

void SplitPointTopology::AddEdge(const PointHandle &p, const EdgeHandle &e)
{
	auto &point = m_points.Get(p);
	auto &sourceSplitPoint = m_splitPoints.Get(m_edges.Get(e).m_source);
	point.m_edges.insert(e);
	sourceSplitPoint.m_point = p;
}

/*
SplitPointTopology::SplitPointHandle SplitPointTopology::AddPointToFace(const FaceHandle &f, const EdgeHandle &e)
{
    PointContainer_t::IndexValuePair_t newPoint = m_points.CreateAndGet();
    return AddPointToFace(f, e, std::get<0>(newPoint));
}

SplitPointTopology::SplitPointHandle SplitPointTopology::AddPointToFace(const FaceHandle &f, const EdgeHandle &e,
                                                                        const PointHandle &p)
{
    Edge &edge = m_edges.Get(e);
    EdgeContainer_t::IndexValuePair_t newEdge = m_edges.CreateAndGet();
    SplitPointContainer_t::IndexValuePair_t newSplitPoint = m_splitPoints.CreateAndGet();
    SplitPoint &nextSplitPoint = m_splitPoints.Get(edge.m_destination);

    m_points.Get(p).m_edges.insert(std::get<0>(newEdge));  // TODO: Check if the edge was inserted

    std::get<1>(newSplitPoint).m_point = p;
    std::get<1>(newSplitPoint).m_face = f;
    std::get<1>(newSplitPoint).m_incomingEdge = e;
    std::get<1>(newSplitPoint).m_outgoingEdge = std::get<0>(newEdge);

    std::get<1>(newEdge).m_source = std::get<0>(newSplitPoint);
    std::get<1>(newEdge).m_destination = edge.m_destination;

    nextSplitPoint.m_incomingEdge = std::get<0>(newEdge);
    edge.m_destination = std::get<0>(newSplitPoint);

    // TODO: ValidateFace(f);

    return std::get<0>(newSplitPoint);
}
*/

/*
 * Modifying the topology
 */
SplitPointTopology::SplitPointHandle SplitPointTopology::SplitEdge(const EdgeHandle &e)
{
	PointContainer_t::IndexValuePair_t newPoint = m_points.CreateAndGet();
	return SplitEdge(e, std::get<0>(newPoint));
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

	SplitPointHandle splitPointC = GetDestination(e);
	EdgeContainer_t::IndexValuePair_t edgeF = m_edges.CreateAndGet();
	SplitPointContainer_t::IndexValuePair_t splitPointP = m_splitPoints.CreateAndGet();
	SetSource(e, std::get<0>(splitPointP));
	SetFace(std::get<0>(splitPointP), m_splitPoints.Get(splitPointC).m_face);
	SetSource(std::get<0>(edgeF), splitPointC);
	SetDestination(std::get<0>(edgeF), std::get<0>(splitPointP));
	SetPoint(std::get<0>(splitPointP), p);

	return std::get<0>(splitPointP);
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

	auto splitPointB = GetSource(e);
	auto splitPointC = GetDestination(e);
	auto prevEdge = GetPrevEdge(e);
	auto splitPointA = GetSource(prevEdge);
	auto pointHandleB = GetPoint(splitPointB);
	auto &pointB = m_points.Get(pointHandleB);
	auto &face = m_faces.Get(GetFace(splitPointA));

	SetOutgoingEdge(splitPointA, prevEdge);
	SetIncomingEdge(splitPointC, prevEdge);
	face.m_splitPoint = splitPointA;

	m_splitPoints.Delete(splitPointB);
	m_edges.Delete(e);
	pointB.m_edges.erase(e);
	if (pointB.m_edges.size() == 0)
	{
		m_points.Delete(pointHandleB);
	}

	return splitPointB;
	/*
	Edge &edge = m_edges.Get(e);
	SplitPoint &prevSplitPoint = m_splitPoints.Get(edge.m_source);
	SplitPoint &nextSplitPoint = m_splitPoints.Get(edge.m_destination);

	Edge &prevEdge = m_edges.Get(GetPrevEdge(e));
	Edge &nextEdge = m_edges.Get(GetNextEdge(e));

	prevEdge.m_destination = nextEdge.m_source;
	nextSplitPoint.m_incomingEdge = GetPrevEdge(e);

	Point &prevPoint = m_points.Get(prevSplitPoint.m_point);
	prevPoint.m_edges.erase(e);

	m_splitPoints.Delete(edge.m_source);
	m_edges.Delete(e);
	prevPoint.m_edges.erase(edge.m_source);
	if (prevPoint.m_edges.size() == 0)
	{
	    m_points.Delete(prevSplitPoint.m_point);
	}
	DumpToStream(std::cout);
	//DeleteSplitPoint(edge.m_source);
	if (prevPoint.m_splitPoints.size() == 0)
	{
	    // TODO: delete point
	}
	// TODO: delete split point
	// TODO: delete edge
	// TODO: Delete previous edges, split points and points
	// TODO: ValidateFace();

	return edge.m_destination;
	*/
}

/*
SplitPointTopology::SplitFaceResult SplitPointTopology::SplitFace(const FaceHandle &f, const EdgeHandle &e0,
                                                                  const EdgeHandle &e1)
{
    SplitFaceResult result;

    return result;
}
*/

/*
 * Deleting parts of the topology.
 */
void SplitPointTopology::DeleteFace(const FaceHandle &f)
{
	std::cout << "Deleting face " << f << std::endl;
	Face &face = m_faces.Get(f);
	std::cout << " " << face.m_splitPoint << std::endl;
	// SplitPoint &splitPoint = m_splitPoints.Get(face.m_splitPoint);

	Index_t currentSplitPointIndex = GetNextSplitPoint(face.m_splitPoint);
	std::cout << " currentSplitPointIndex " << currentSplitPointIndex << std::endl;
	uint32_t splitPointCount = 0;

	PointHandleSet pointsToDelete;
	SplitPointHandleSet splitPointsToDelete;
	EdgeHandleSet edgesToDelete;
	while (currentSplitPointIndex != face.m_splitPoint)
	{
		SplitPoint &splitPoint = m_splitPoints.Get(currentSplitPointIndex);
		Point &point = m_points.Get(splitPoint.m_point);
		point.m_edges.erase(currentSplitPointIndex);

		if (point.m_edges.size() == 0)
		{
			pointsToDelete.insert(splitPoint.m_point);
			// m_points.Delete(splitPoint.m_point);
		}
		// m_splitPoints.Delete(currentSplitPointIndex);
		// m_edges.Delete(splitPoint.m_outgoingEdge);
		splitPointsToDelete.insert(currentSplitPointIndex);
		edgesToDelete.insert(splitPoint.m_outgoingEdge);

		++splitPointCount;
		currentSplitPointIndex = GetNextSplitPoint(currentSplitPointIndex);
	}

	m_faces.Delete(f);

	IsValid();
}

void SplitPointTopology::DeletePoint(const PointHandle &p)
{
	std::cout << "Deleting point " << p << std::endl;
	Point &point = m_points.Get(p);
	std::cout << " " << point.m_edges.size() << std::endl;

	for (auto &edge : point.m_edges)
	{
		DeleteEdge(EdgeHandle(edge));
	}

	IsValid();
}

void SplitPointTopology::DeleteEdge(const EdgeHandle &e)
{
	std::cout << "Deleting edge " << e << std::endl;
	Edge &edge = m_edges.Get(e);
	std::cout << " " << edge.m_destination << std::endl;
	SplitPoint &splitPoint = m_splitPoints.Get(edge.m_source);

	DeleteFace(splitPoint.m_face);

	IsValid();
}

void SplitPointTopology::DeleteSplitPoint(const SplitPointHandle &s)
{
	std::cout << "Deleting split point " << s << std::endl;
	SplitPoint &splitPoint = m_splitPoints.Get(s);
	std::cout << " " << splitPoint.m_incomingEdge << " : " << splitPoint.m_outgoingEdge << std::endl;

	DeleteFace(splitPoint.m_face);

	IsValid();
}

/*
 * Querying the topology.
 */
SplitPointTopology::EdgeHandleSet SplitPointTopology::GetEdges(const PointHandle &p0, const PointHandle &p1)
{
	EdgeHandleSet edges;
	const Point &point0 = m_points.Get(p0);
	const Point &point1 = m_points.Get(p1);

	// TODO: Check how to do a set union
	std::unordered_set<Index_t> edgesToCheck = point0.m_edges;
	edgesToCheck.insert(std::begin(point1.m_edges), std::end(point1.m_edges));
	//(point1.m_edges);

	for (const auto &edgeIndex : edgesToCheck)
	{
		const Index_t sourcePointIndex = GetPoint(GetSource(edgeIndex));
		const Index_t destinationPointIndex = GetPoint(GetDestination(edgeIndex));

		if ((sourcePointIndex == p0 && destinationPointIndex == p1) ||
		    (sourcePointIndex == p1 && destinationPointIndex == p0))
		{
			edges.insert(EdgeHandle(edgeIndex));
		}
	}

	return edges;
}

SplitPointTopology::FaceHandleSet SplitPointTopology::GetFaces(const PointHandle &p)
{
	FaceHandleSet faces;

	const Point &point = m_points.Get(p);
	for (const auto &edge : point.m_edges)
	{
		faces.insert(GetFace(EdgeHandle(edge)));
	}
	return faces;
}

SplitPointTopology::FaceHandleSet SplitPointTopology::GetFaces(const PointHandle &p0, const PointHandle &p1)
{
	FaceHandleSet faces;
	EdgeHandleSet edges = GetEdges(p0, p1);

	for (const auto &e : edges)
	{
		const auto &sourceSplitPoint = m_splitPoints.Get(GetSource(e));
		faces.insert(sourceSplitPoint.m_face);
	}

	return faces;
}

SplitPointTopology::PointIterator SplitPointTopology::PointsBegin() { return PointIterator(m_points.begin()); }

SplitPointTopology::PointIterator SplitPointTopology::PointsEnd() { return PointIterator(m_points.end()); }

SplitPointTopology::SplitPointIterator SplitPointTopology::SplitPointsBegin() { return SplitPointIterator(m_splitPoints.begin()); }

SplitPointTopology::SplitPointIterator SplitPointTopology::SplitPointsEnd() { return SplitPointIterator(m_splitPoints.end()); }

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
	for (const auto &p : m_points)
	{
		isValid = isValid && ValidatePoint(p.first);
	}

	for (const auto &s : m_splitPoints)
	{
		isValid = isValid && ValidateSplitPoint(s.first);
	}

	for (const auto &e : m_edges)
	{
		isValid = isValid && ValidateEdge(e.first);
	}

	for (const auto &f : m_faces)
	{
		isValid = isValid && ValidateFace(f.first);
	}

	// CRITICAL_ASSERT_MSG(isValid, "Invalid topology");
	return isValid;
}

bool SplitPointTopology::ValidatePoint(const Index_t &p)
{
	if (!m_points.HasIndex(p))
	{
		LOG_ERROR("Point %d doesn't exist", p);
		return false;
	}

	bool isValid = true;
	// Check that all edges exist
	for (const auto &e : m_points.Get(p).m_edges)
	{
		if (!m_edges.HasIndex(e))
		{
			isValid = false;
			LOG_ERROR("Edge %d doesn't exist for point %d.", e, p);
		}
		else
		{
			if (GetPoint(GetSource(e)).GetIndex() != p)
			{
				LOG_ERROR("Edge %d referenced by point %d is not an outgoing edge for this point.", e, p);
				isValid = false;
			}
		}
	}
	return isValid;
}

bool SplitPointTopology::ValidateSplitPoint(const Index_t &s)
{
	if (!m_splitPoints.HasIndex(s))
	{
		LOG_ERROR("Split Point %d doesn't exist.", s);
		return false;
	}

	bool isValid = true;
	const SplitPoint &splitPoint = m_splitPoints.Get(s);

	// Check that the incoming and outgoing edges exist
	if (!m_edges.HasIndex(splitPoint.m_incomingEdge))
	{
		LOG_ERROR("Incoming Edge %d doesn't exist for Split Point %d", splitPoint.m_incomingEdge, s);
		isValid = false;
	}
	if (!m_edges.HasIndex(splitPoint.m_outgoingEdge))
	{
		LOG_ERROR("Outgoing Edge %d doesn't exist for Split Point %d", splitPoint.m_outgoingEdge, s);
		isValid = false;
	}

	// Check that the edges reference the split point
	Edge incomingEdge = m_edges.Get(splitPoint.m_incomingEdge);
	Edge outgoingEdge = m_edges.Get(splitPoint.m_outgoingEdge);
	if (incomingEdge.m_destination != s)
	{
		LOG_ERROR("Incoming Edge %d of Split Point %d references Split Point %d", splitPoint.m_incomingEdge, s,
		          incomingEdge.m_destination);
		isValid = false;
	}
	if (outgoingEdge.m_source != s)
	{
		LOG_ERROR("Outgoing Edge %d of Split Point %d references Split Point %d", splitPoint.m_outgoingEdge, s,
		          outgoingEdge.m_source);
		isValid = false;
	}

	// Check that the face exists
	if (!m_faces.HasIndex(splitPoint.m_face))
	{
		LOG_ERROR("Face %d for Split Point %d doesn't exist", splitPoint.m_face, s);
		isValid = false;
	}

	// Check that the point exists
	if (!m_points.HasIndex(splitPoint.m_point))
	{
		LOG_ERROR("Point %d for Split Point %d doesn't exist", splitPoint.m_point, s);
		isValid = false;
	}

	return isValid;
}

bool SplitPointTopology::ValidateEdge(const Index_t &e)
{
	if (!m_edges.HasIndex(e))
	{
		LOG_ERROR("Edge %d doesn't exist", e);
		return false;
	}

	bool isValid = true;
	Edge &edge = m_edges.Get(e);

	// Check that the split points exist.
	if (!m_splitPoints.HasIndex(edge.m_source))
	{
		LOG_ERROR("Source Split Point %d for Edge %d doesn't exist", edge.m_source, e);
		isValid = false;
	}
	if (!m_splitPoints.HasIndex(edge.m_destination))
	{
		LOG_ERROR("Destination Split Point %d for Edge %d doesn't exist", edge.m_destination, e);
		isValid = false;
	}

	// Check that the split points reference this edge.
	SplitPoint &source = m_splitPoints.Get(edge.m_source);
	SplitPoint &destination = m_splitPoints.Get(edge.m_destination);
	if (source.m_outgoingEdge != e)
	{
		LOG_ERROR("Source Split Point %d doesn't reference Edge %d (references %d)", edge.m_source, e,
		          source.m_outgoingEdge);
		isValid = false;
	}
	if (destination.m_incomingEdge != e)
	{
		LOG_ERROR("Destination Split Point %d doesn't reference Edge %d (references %d)", edge.m_destination, e,
		          destination.m_incomingEdge);
		isValid = false;
	}

	return isValid;
}

bool SplitPointTopology::ValidateFace(const Index_t &f)
{
	if (!m_faces.HasIndex(f))
	{
		LOG_ERROR("Face %d doesn't exist", f);
		return false;
	}

	bool isValid = true;
	Face &face = m_faces.Get(f);

	// Check that the split point exists.
	if (!m_splitPoints.HasIndex(face.m_splitPoint))
	{
		LOG_ERROR("Split Point %d for Face %d doesn't exist", face.m_splitPoint, f);
	}

	Index_t currentSplitPointIndex = GetNextSplitPoint(SplitPointHandle(face.m_splitPoint));
	uint32_t splitPointCount = 1;

	while (currentSplitPointIndex != face.m_splitPoint)
	{
		const SplitPoint &splitPoint = m_splitPoints.Get(currentSplitPointIndex);

		// Check that all split points point to this face.
		if (splitPoint.m_face != f)
		{
			LOG_ERROR("Split Point %d doesn't reference Face %d (references %d)", currentSplitPointIndex, f,
			          splitPoint.m_face);
			isValid = false;
		}

		++splitPointCount;
		currentSplitPointIndex = GetNextSplitPoint(SplitPointHandle(currentSplitPointIndex));

		if (splitPointCount >= 100)
		{
			LOG_ERROR("Possible infinite loop in Face %d", f);
			isValid = false;
			break;
		}
	}

	// Check that there are at least 3 points in this face.
	if (splitPointCount < 3)
	{
		LOG_ERROR("Face %d has less than 3 Split Points. It has %d Split Points.", f, splitPointCount);
		isValid = false;
	}

	return isValid;
}

void SplitPointTopology::DumpToStream(std::ostream &outStream)
{
	outStream << "IsValid: " << (IsValid() ? "true" : "false") << std::endl;
	outStream << "Points:" << std::endl;
	for (const auto &p : m_points)
	{
		outStream << " " << p.first << ":" << std::endl;
		for (const auto &e : p.second.m_edges)
		{
			outStream << "  Edge: " << e << std::endl;
		}
	}

	outStream << "SplitPoints:" << std::endl;
	for (const auto &sp : m_splitPoints)
	{
		outStream << " " << sp.first << ":" << std::endl;
		outStream << "  Point: " << sp.second.m_point << std::endl;
		outStream << "  Face: " << sp.second.m_face << std::endl;
		outStream << "  Incoming Edge: " << sp.second.m_incomingEdge << std::endl;
		outStream << "  Outgoing Edge: " << sp.second.m_outgoingEdge << std::endl;
	}

	outStream << "Edges:" << std::endl;
	for (const auto &e : m_edges)
	{
		outStream << " " << e.first << ":" << std::endl;
		outStream << "  Source SplitPoint: " << e.second.m_source << std::endl;
		outStream << "  Destination SplitPoint: " << e.second.m_destination << std::endl;
	}

	outStream << "Faces:" << std::endl;
	for (const auto &f : m_faces)
	{
		outStream << " " << f.first << ":" << std::endl;
		outStream << "  SplitPoint: " << f.second.m_splitPoint << std::endl;
	}
}
}  // namespace selector
