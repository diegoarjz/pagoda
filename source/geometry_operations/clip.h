#ifndef PAGODA_GEOMETRY_OPERATIONS_CLIP_H_
#define PAGODA_GEOMETRY_OPERATIONS_CLIP_H_

#include "math_lib/intersection.h"
#include "math_lib/line_3d.h"
#include "math_lib/plane.h"

#include "geometry_core/geometry_builder.h"

namespace pagoda
{
template<class G>
class Clip
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;
	using Index_t = typename Geometry::Index_t;

public:
	Clip(const Plane<float> &plane) : m_plane(plane) {}

	void Execute(GeometryPtr geometryIn, GeometryPtr front, GeometryPtr back)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "Clip with plane " << to_string(m_plane));

		*front = *geometryIn;

		CheckPointsSide(front);
		SplitEdges(front);

		auto facesEnd = front->FacesEnd();
		for (auto fIter = front->FacesBegin(); fIter != facesEnd; ++fIter)
		{
			ClipFace(front, *fIter);
		}

		GeometryBuilderT<Geometry> builder(back);
		std::map<Index_t, Index_t> pointsToBuilderIndex;

		std::set<Index_t> facesToDelete;
		for (auto fIter = front->FacesBegin(); fIter != front->FacesEnd(); ++fIter)
		{
			if (m_faceSide[*fIter] == Plane<float>::PlaneSide::Back)
			{
				LOG_TRACE(GeometryOperations, "Face " << *fIter << " is behind the plane");
				facesToDelete.insert(*fIter);

				auto faceBuilder = builder.StartFace();
				for (auto fpCirc = front->FacePointCirculatorBegin(*fIter); fpCirc; ++fpCirc)
				{
					auto mapIter = pointsToBuilderIndex.find(*fpCirc);
					if (mapIter == std::end(pointsToBuilderIndex))
					{
						mapIter =
						    pointsToBuilderIndex.emplace(*fpCirc, builder.AddPoint(front->GetPosition(*fpCirc))).first;
					}

					faceBuilder.AddIndex(mapIter->second);
				}
				faceBuilder.CloseFace();
			}
		}
		for (const auto &f : facesToDelete)
		{
			front->DeleteFace(f);
		}
	}

private:
	void ClipFace(GeometryPtr geometry, const SplitPointTopology::FaceHandle &face)
	{
		START_PROFILE;

#ifdef DEBUG
		LOG_TRACE(GeometryOperations, "Clipping Face " << face << " with vertices");
		for (auto fpIter = geometry->FacePointCirculatorBegin(face); fpIter; ++fpIter)
		{
			LOG_TRACE(GeometryOperations, " " << geometry->GetPosition(*fpIter));
		}
#endif

		std::vector<std::pair<Index_t, Index_t>> splitFaceEdges;
		std::pair<Index_t, Index_t> currentEdgeToSplit;
		uint16_t state = 0;

		for (auto feIter = geometry->FaceEdgeCirculatorBegin(face); feIter; ++feIter)
		{
			if (m_newEdges.find(*feIter) != std::end(m_newEdges))
			{
				if (state == 0)
				{
					std::get<0>(currentEdgeToSplit) = *feIter;
				}
				else
				{
					std::get<1>(currentEdgeToSplit) = geometry->GetPrevEdge(*feIter);
					splitFaceEdges.push_back(currentEdgeToSplit);
				}

				state = (state + 1) % 2;
			}
		}

		for (const auto &e : splitFaceEdges)
		{
			LOG_TRACE(GeometryOperations,
			          "Splitting face " << face << " from edge " << std::get<0>(e) << " to edge " << std::get<1>(e));
			auto newFace = geometry->SplitFace(face, std::get<0>(e), std::get<1>(e));
			auto faceSide = CheckFaceSide(geometry, newFace);
			m_faceSide[newFace] = faceSide;

#ifdef DEBUG
			for (const auto &f : {face, newFace})
			{
				LOG_TRACE(GeometryOperations, "Into face " << f << " with vertices");
				for (auto fpIter = geometry->FacePointCirculatorBegin(f); fpIter; ++fpIter)
				{
					LOG_TRACE(GeometryOperations, " " << geometry->GetPosition(*fpIter));
				}
			}
#endif

			if (faceSide == Plane<float>::PlaneSide::Front)
			{
				m_faceSide[face] = Plane<float>::PlaneSide::Back;
			}
			else if (faceSide == Plane<float>::PlaneSide::Back)
			{
				m_faceSide[face] = Plane<float>::PlaneSide::Front;
			}
			else  // Contained
			{
				m_faceSide[face] = Plane<float>::PlaneSide::Front;
			}
		}
	}

	typename Plane<float>::PlaneSide CheckFaceSide(GeometryPtr geometry, const Index_t &face)
	{
		typename Plane<float>::PlaneSide planeSide;
		for (auto fpIter = geometry->FacePointCirculatorBegin(face); fpIter; ++fpIter)
		{
			auto pos = geometry->GetPosition(*fpIter);
			planeSide = m_plane.GetPlaneSide(pos);
			if (planeSide != Plane<float>::PlaneSide::Contained)
			{
				break;
			}
		}
		LOG_TRACE(GeometryOperations, "Face " << face << " is " << to_string<float>(planeSide));
		return planeSide;
	}

	void CheckPointsSide(GeometryPtr geometry)
	{
		START_PROFILE;

		auto pointsEnd = geometry->PointsEnd();
		for (auto pIter = geometry->PointsBegin(); pIter != pointsEnd; ++pIter)
		{
			auto pos = geometry->GetPosition(*pIter);
			auto planeSide = m_plane.GetPlaneSide(pos);
			m_pointsSide[*pIter] = planeSide;
			LOG_TRACE(GeometryOperations, "Point " << *pIter << " " << pos << " is " << to_string<float>(planeSide)
			                                       << " Plane" << to_string<float>(planeSide).c_str());
		}
	}

	void SplitEdges(GeometryPtr geometry)
	{
		START_PROFILE;

		std::map<Index_t, typename Geometry::PositionType> edgesToSplit;

		auto edgesEnd = geometry->EdgesEnd();
		for (auto eIter = geometry->EdgesBegin(); eIter != edgesEnd; ++eIter)
		{
			auto sourcePoint = geometry->GetPoint(geometry->GetSource(*eIter));
			auto destPoint = geometry->GetPoint(geometry->GetDestination(*eIter));

			if (!SameSide(sourcePoint, destPoint))
			{
				LOG_TRACE(GeometryOperations,
				          "Points " << sourcePoint << " (" << to_string<float>(m_pointsSide[sourcePoint]) << " ) and "
				                    << destPoint << " (" << to_string<float>(m_pointsSide[destPoint])
				                    << ") are on different sides");
				auto sourcePos = geometry->GetPosition(sourcePoint);
				auto destPos = geometry->GetPosition(destPoint);

				LOG_TRACE(GeometryOperations, " Source Position: " << to_string(sourcePos));
				LOG_TRACE(GeometryOperations, " Destination Position: " << to_string(destPos));

				auto line = Line3D<float>::FromTwoPoints(sourcePos, destPos);
				auto edgeIntersection = intersection(m_plane, line);

				LOG_TRACE(GeometryOperations,
				          " Intersection with plane: " << to_string(edgeIntersection.m_intersection));

				auto edgesBetweenPoints = geometry->GetEdges(sourcePoint, destPoint);
				for (const auto &e : edgesBetweenPoints)
				{
					edgesToSplit[e] = edgeIntersection.m_intersection;
				}
			}
			else if (m_pointsSide[sourcePoint] == Plane<float>::PlaneSide::Back)
			{
				// It's a face behind the plane
				m_faceSide[geometry->GetFace(*eIter)] = Plane<float>::PlaneSide::Back;
			}
		}

		for (const auto &e : edgesToSplit)
		{
			auto splitPoint = geometry->SplitEdge(e.first);
			auto newEdge = geometry->GetOutEdge(splitPoint);
			m_newEdges.insert(newEdge);
			auto newPoint = geometry->GetPoint(splitPoint);
			geometry->SetPosition(newPoint, e.second);
			LOG_TRACE(GeometryOperations, "Edge " << newEdge << " is a new edge");
		}
	}

	bool SameSide(const Index_t &p1, const Index_t &p2)
	{
		auto p1Side = m_pointsSide[p1];
		auto p2Side = m_pointsSide[p2];

		return p1Side == p2Side || p1Side == Plane<float>::PlaneSide::Contained ||
		       p2Side == Plane<float>::PlaneSide::Contained;
	}

	std::set<Index_t> m_newEdges;
	std::map<Index_t, Plane<float>::PlaneSide> m_pointsSide;
	std::map<Index_t, Plane<float>::PlaneSide> m_faceSide;
	Plane<float> m_plane;
};
}  // namespace pagoda

#endif
