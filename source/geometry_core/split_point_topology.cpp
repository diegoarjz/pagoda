#include "split_point_topology.h"

#include "common/logger.h"

namespace selector
{
SplitPointTopology::SplitPointTopology() {}

SplitPointTopology::~SplitPointTopology() {}

/*
 * Operations
 */
SplitPointTopology::PointHandle SplitPointTopology::GetPoint(const SplitPointHandle &s) const
{
	return PointHandle(m_splitPoints.Get(s.GetIndex()).m_point);
}

SplitPointTopology::EdgeHandle SplitPointTopology::GetInEdge(const SplitPointHandle &s) const
{
	return EdgeHandle(m_splitPoints.Get(s.GetIndex()).m_incomingEdge);
}

SplitPointTopology::EdgeHandle SplitPointTopology::GetOutEdge(const SplitPointHandle &s) const
{
	return EdgeHandle(m_splitPoints.Get(s.GetIndex()).m_outgoingEdge);
}

SplitPointTopology::FaceHandle SplitPointTopology::GetFace(const SplitPointHandle &s) const
{
	return FaceHandle(m_splitPoints.Get(s.GetIndex()).m_face);
}

SplitPointTopology::SplitPointHandle SplitPointTopology::GetSource(const EdgeHandle &e) const
{
	return SplitPointHandle(m_edges.Get(e.GetIndex()).m_source);
}

SplitPointTopology::SplitPointHandle SplitPointTopology::GetDestination(const EdgeHandle &e) const
{
	return SplitPointHandle(m_edges.Get(e.GetIndex()).m_destination);
}

SplitPointTopology::FaceHandle SplitPointTopology::GetFace(const EdgeHandle &e) const
{
	return FaceHandle(m_splitPoints.Get(GetSource(e).GetIndex()).m_face);
}

SplitPointTopology::SplitPointHandle SplitPointTopology::GetSplitPoint(const FaceHandle &f) const
{
	return SplitPointHandle(m_faces.Get(f.GetIndex()).m_splitPoint);
}

SplitPointTopology::EdgeHandle SplitPointTopology::GetEdge(const FaceHandle &f) const
{
	return EdgeHandle(GetOutEdge(GetSplitPoint(FaceHandle(f.GetIndex()))).GetIndex());
}

/*
 * Navigating the topology
 */
SplitPointTopology::EdgeHandle SplitPointTopology::GetNextEdge(const EdgeHandle &e) const
{
	auto &edge = m_edges.Get(e.GetIndex());
	auto &nextSplitPoint = m_splitPoints.Get(edge.m_destination);
	return EdgeHandle(nextSplitPoint.m_outgoingEdge);
}

SplitPointTopology::EdgeHandle SplitPointTopology::GetPrevEdge(const EdgeHandle &e) const
{
	auto &edge = m_edges.Get(e.GetIndex());
	auto &splitPoint = m_splitPoints.Get(edge.m_source);
	return EdgeHandle(splitPoint.m_incomingEdge);
}

SplitPointTopology::PointHandle SplitPointTopology::GetNextPoint(const PointHandle &p, const EdgeHandle &e) const
{
	auto &edge = m_edges.Get(e.GetIndex());
	auto &nextSplitPoint = m_splitPoints.Get(edge.m_destination);
	return PointHandle(nextSplitPoint.m_point);
}

SplitPointTopology::PointHandle SplitPointTopology::GetPrevPoint(const PointHandle &p, const EdgeHandle &e) const
{
	auto &edge = m_edges.Get(e.GetIndex());
	auto &splitPoint = m_splitPoints.Get(edge.m_source);
	return PointHandle(splitPoint.m_point);
}

SplitPointTopology::SplitPointHandle SplitPointTopology::GetNextSplitPoint(const SplitPointHandle &s) const
{
	auto &splitPoint = m_splitPoints.Get(s.GetIndex());
	auto &edge = m_edges.Get(splitPoint.m_outgoingEdge);
	return SplitPointHandle(edge.m_destination);
}

SplitPointTopology::SplitPointHandle SplitPointTopology::GetPrevSplitPoint(const SplitPointHandle &s) const
{
	auto &splitPoint = m_splitPoints.Get(s.GetIndex());
	auto &prevEdge = m_edges.Get(splitPoint.m_incomingEdge);
	return SplitPointHandle(prevEdge.m_source);
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
	auto p = m_points.Get(p0.GetIndex());
	IndexPointPairArray_t<3> points = {PointContainer_t::IndexValuePair_t{p0.GetIndex(), m_points.Get(p0.GetIndex())},
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
	IndexPointPairArray_t<3> points = {PointContainer_t::IndexValuePair_t{p0.GetIndex(), m_points.Get(p0.GetIndex())},
	                                   PointContainer_t::IndexValuePair_t{p1.GetIndex(), m_points.Get(p1.GetIndex())},
	                                   m_points.CreateAndGet()};

	// SplitPoints
	IndexSplitPointPairArray_t<3> splitPoints = {m_splitPoints.CreateAndGet(), m_splitPoints.CreateAndGet(),
	                                             m_splitPoints.CreateAndGet()};

	// Edges
	IndexEdgePairArray_t<3> edges = {m_edges.CreateAndGet(), m_edges.CreateAndGet(), m_edges.CreateAndGet()};

	return CreateFace(points, splitPoints, edges);
}

SplitPointTopology::CreateFaceResult SplitPointTopology::CreateFace(const PointHandle &p0, const PointHandle &p1,
                                                                    const PointHandle &p2)
{
	// Points
	IndexPointPairArray_t<3> points = {PointContainer_t::IndexValuePair_t{p0.GetIndex(), m_points.Get(p0.GetIndex())},
	                                   PointContainer_t::IndexValuePair_t{p1.GetIndex(), m_points.Get(p1.GetIndex())},
	                                   PointContainer_t::IndexValuePair_t{p2.GetIndex(), m_points.Get(p2.GetIndex())}};

	// SplitPoints
	IndexSplitPointPairArray_t<3> splitPoints = {m_splitPoints.CreateAndGet(), m_splitPoints.CreateAndGet(),
	                                             m_splitPoints.CreateAndGet()};

	// Edges
	IndexEdgePairArray_t<3> edges = {m_edges.CreateAndGet(), m_edges.CreateAndGet(), m_edges.CreateAndGet()};

	return CreateFace(points, splitPoints, edges);
}

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
		// auto &destinationSplitPoint = std::get<1>(splitPoints[(i + 1) % 3]);

		point.m_edges.insert(std::get<0>(edges[i]));  // TODO: check if the point was inserted

		sourceSplitPoint.m_point = std::get<0>(points[i]);
		sourceSplitPoint.m_face = std::get<0>(face);
		sourceSplitPoint.m_outgoingEdge = std::get<0>(edges[i]);
		sourceSplitPoint.m_incomingEdge = std::get<0>(edges[(i - 1) % 3]);

		edge.m_source = std::get<0>(splitPoints[i]);
		edge.m_destination = std::get<0>(splitPoints[(i + 1) % 3]);
	}

	// TODO: Validate face
	// ValidateFace(std::get<0>(face));

	CreateFaceResult result(FaceHandle(std::get<0>(face)),
	                        {PointHandle(std::get<0>(points[0])), PointHandle(std::get<0>(points[1])),
	                         PointHandle(std::get<0>(points[2]))});

	return result;
}

SplitPointTopology::SplitPointHandle SplitPointTopology::AddPointToFace(const FaceHandle &f, const EdgeHandle &e)
{
	PointContainer_t::IndexValuePair_t newPoint = m_points.CreateAndGet();
	return AddPointToFace(f, e, PointHandle(std::get<0>(newPoint)));
}

SplitPointTopology::SplitPointHandle SplitPointTopology::AddPointToFace(const FaceHandle &f, const EdgeHandle &e,
                                                                        const PointHandle &p)
{
	Edge &edge = m_edges.Get(e.GetIndex());
	EdgeContainer_t::IndexValuePair_t newEdge = m_edges.CreateAndGet();
	SplitPointContainer_t::IndexValuePair_t newSplitPoint = m_splitPoints.CreateAndGet();
	SplitPoint &nextSplitPoint = m_splitPoints.Get(edge.m_destination);

	m_points.Get(p.GetIndex()).m_edges.insert(std::get<0>(newEdge));  // TODO: Check if the edge was inserted

	std::get<1>(newSplitPoint).m_point = p.GetIndex();
	std::get<1>(newSplitPoint).m_face = f.GetIndex();
	std::get<1>(newSplitPoint).m_incomingEdge = e.GetIndex();
	std::get<1>(newSplitPoint).m_outgoingEdge = std::get<0>(newEdge);

	std::get<1>(newEdge).m_source = std::get<0>(newSplitPoint);
	std::get<1>(newEdge).m_destination = edge.m_destination;

	nextSplitPoint.m_incomingEdge = std::get<0>(newEdge);
	edge.m_destination = std::get<0>(newSplitPoint);

	// TODO: ValidateFace(f.GetIndex());

	return SplitPointHandle(std::get<0>(newSplitPoint));
}

/*
 * Modifying the topology
 */
SplitPointTopology::SplitPointHandle SplitPointTopology::SplitEdge(const EdgeHandle &e)
{
	PointContainer_t::IndexValuePair_t newPoint = m_points.CreateAndGet();
	return SplitEdge(e, PointHandle(std::get<0>(newPoint)));
}

SplitPointTopology::SplitPointHandle SplitPointTopology::SplitEdge(const EdgeHandle &e, const PointHandle &p)
{
	Point &point = m_points.Get(p.GetIndex());
	SplitPointContainer_t::IndexValuePair_t newSplitPoint = m_splitPoints.CreateAndGet();
	Edge &edge = m_edges.Get(e.GetIndex());
	// SplitPoint &nextSplitPoint = m_splitPoints.Get(edge.m_destination);
	EdgeContainer_t::IndexValuePair_t newEdge = m_edges.CreateAndGet();

	point.m_edges.insert(std::get<0>(newEdge));  // TODO: check if the edge was inserted.

	std::get<1>(newSplitPoint).m_point = p.GetIndex();
	std::get<1>(newSplitPoint).m_incomingEdge = e.GetIndex();
	std::get<1>(newSplitPoint).m_outgoingEdge = std::get<0>(newEdge);
	std::get<1>(newSplitPoint).m_face = GetFace(e).GetIndex();

	std::get<1>(newEdge).m_source = std::get<0>(newSplitPoint);
	std::get<1>(newEdge).m_destination = edge.m_destination;

	edge.m_destination = std::get<0>(newSplitPoint);

	std::get<1>(newSplitPoint).m_incomingEdge = std::get<0>(newEdge);

	// TODO: ValidateEdge(e.GetIndex());
	// TODO: ValidateEdge(std::get<0>(newEdge));

	return SplitPointHandle(std::get<0>(newSplitPoint));
}

SplitPointTopology::SplitPointHandle SplitPointTopology::CollapseEdge(const EdgeHandle &e)
{
	Edge &edge = m_edges.Get(e.GetIndex());
	SplitPoint &prevSplitPoint = m_splitPoints.Get(edge.m_source);
	SplitPoint &nextSplitPoint = m_splitPoints.Get(edge.m_destination);

	Edge &prevEdge = m_edges.Get(GetPrevEdge(e).GetIndex());
	Edge &nextEdge = m_edges.Get(GetNextEdge(e).GetIndex());

	prevEdge.m_destination = nextEdge.m_source;
	nextSplitPoint.m_incomingEdge = GetPrevEdge(e).GetIndex();

	Point &prevPoint = m_points.Get(prevSplitPoint.m_point);
	prevPoint.m_edges.erase(e.GetIndex());

	/*
	if (prevPoint.m_splitPoints.size() == 0)
	{
	    // TODO: delete point
	}
	*/
	// TODO: delete split point
	// TODO: delete edge
	// TODO: Delete previous edges, split points and points
	// TODO: ValidateFace();

	return SplitPointHandle(edge.m_destination);
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
	Face &face = m_faces.Get(f.GetIndex());
	// SplitPoint &splitPoint = m_splitPoints.Get(face.m_splitPoint);

	Index_t currentSplitPointIndex = GetNextSplitPoint(SplitPointHandle(face.m_splitPoint)).GetIndex();
	uint32_t splitPointCount = 0;

	while (currentSplitPointIndex != face.m_splitPoint)
	{
		SplitPoint &splitPoint = m_splitPoints.Get(currentSplitPointIndex);
		Point &point = m_points.Get(splitPoint.m_point);
		point.m_edges.erase(currentSplitPointIndex);

		if (point.m_edges.size() == 0)
		{
			m_points.Delete(splitPoint.m_point);
		}
		m_splitPoints.Delete(currentSplitPointIndex);
		m_edges.Delete(splitPoint.m_outgoingEdge);

		++splitPointCount;
		currentSplitPointIndex = GetNextSplitPoint(SplitPointHandle(currentSplitPointIndex)).GetIndex();
	}

	m_faces.Delete(f.GetIndex());

	IsValid();
}

void SplitPointTopology::DeletePoint(const PointHandle &p)
{
	Point &point = m_points.Get(p.GetIndex());

	for (auto &edge : point.m_edges)
	{
		DeleteEdge(EdgeHandle(edge));
	}

	IsValid();
}

void SplitPointTopology::DeleteEdge(const EdgeHandle &e)
{
	Edge &edge = m_edges.Get(e.GetIndex());
	SplitPoint &splitPoint = m_splitPoints.Get(edge.m_source);

	DeleteFace(FaceHandle(splitPoint.m_face));

	IsValid();
}

void SplitPointTopology::DeleteSplitPoint(const SplitPointHandle &s)
{
	SplitPoint &splitPoint = m_splitPoints.Get(s.GetIndex());

	DeleteFace(FaceHandle(splitPoint.m_face));

	IsValid();
}

/*
 * Querying the topology.
 */
SplitPointTopology::EdgeHandleSet SplitPointTopology::GetEdges(const PointHandle &p0, const PointHandle &p1)
{
	EdgeHandleSet edges;
	const Point &point0 = m_points.Get(p0.GetIndex());
	const Point &point1 = m_points.Get(p1.GetIndex());

	// TODO: Check how to do a set union
	std::unordered_set<Index_t> edgesToCheck = point0.m_edges;
	edgesToCheck.insert(std::begin(point1.m_edges), std::end(point1.m_edges));
	//(point1.m_edges);

	for (const auto &edgeIndex : edgesToCheck)
	{
		const Index_t sourcePointIndex = GetPoint(GetSource(EdgeHandle(edgeIndex))).GetIndex();
		const Index_t destinationPointIndex = GetPoint(GetDestination(EdgeHandle(edgeIndex))).GetIndex();

		if ((sourcePointIndex == p0.GetIndex() && destinationPointIndex == p1.GetIndex()) ||
		    (sourcePointIndex == p1.GetIndex() && destinationPointIndex == p0.GetIndex()))
		{
			edges.insert(EdgeHandle(edgeIndex));
		}
	}

	return edges;
}

SplitPointTopology::FaceHandleSet SplitPointTopology::GetFaces(const PointHandle &p)
{
	FaceHandleSet faces;

	const Point &point = m_points.Get(p.GetIndex());
	for (const auto &edge : point.m_edges)
	{
		faces.insert(GetFace(EdgeHandle(edge)));
	}

	/*
	PointHandle currentPoint = GetNextPoint(p);

	while (currentPoint != p)
	{
	    const SplitPoint &s = m_splitPoints.Get(GetSplitPoint(currentPoint.GetIndex())).GetIndex();
	    faces.insert(s.m_face);
	    currentPoint = GeteNextPoint(currentPoint);
	}
	*/

	return faces;
}

SplitPointTopology::FaceHandleSet SplitPointTopology::GetFaces(const PointHandle &p0, const PointHandle &p1)
{
	FaceHandleSet faces;
	EdgeHandleSet edges = GetEdges(p0, p1);

	for (const auto &e : edges)
	{
		const auto &sourceSplitPoint = m_splitPoints.Get(GetSource(e).GetIndex());
		faces.insert(FaceHandle(sourceSplitPoint.m_face));
	}

	return faces;
}

SplitPointTopology::EdgeHandleSet SplitPointTopology::GetSharedEdges(const FaceHandle &f0, const FaceHandle &f1)
{
	EdgeHandleSet edges;

	return edges;
}

bool SplitPointTopology::IsValid()
{
	bool hasErrors = false;
	for (const auto &p : m_points)
	{
		hasErrors = hasErrors || ValidatePoint(p.first);
	}

	for (const auto &s : m_splitPoints)
	{
		hasErrors = hasErrors || ValidateSplitPoint(s.first);
	}

	for (const auto &e : m_edges)
	{
		hasErrors = hasErrors || ValidateEdge(e.first);
	}

	for (const auto &f : m_faces)
	{
		hasErrors = hasErrors || ValidateFace(f.first);
	}

	CRITICAL_ASSERT_MSG(hasErrors, "Invalid topology");
	return !hasErrors;
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

	Index_t currentSplitPointIndex = GetNextSplitPoint(SplitPointHandle(face.m_splitPoint)).GetIndex();
	uint32_t splitPointCount = 0;

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
		currentSplitPointIndex = GetNextSplitPoint(SplitPointHandle(currentSplitPointIndex)).GetIndex();

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
		LOG_ERROR("Face %d has less than 3 Split Points", f);
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
}
