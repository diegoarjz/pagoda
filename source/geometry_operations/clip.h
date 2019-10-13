#ifndef SELECTOR_GEOMETRY_OPERATIONS_CLIP_H_
#define SELECTOR_GEOMETRY_OPERATIONS_CLIP_H_

#include "math_lib/intersection.h"
#include "math_lib/line_3d.h"
#include "math_lib/plane.h"

#include "geometry_core/geometry_builder.h"

namespace selector
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
		LOG_TRACE(GeometryOperations, "Clip with plane %s.", to_string(m_plane).c_str());

		*front = *geometryIn;

		CheckPointsSide(front);
		SplitEdges(front);

		auto facesEnd = front->FacesEnd();
		for (auto fIter = front->FacesBegin(); fIter != facesEnd; ++fIter)
		{
			ClipFace(front, *fIter);
		}

		GeometryBuilderT<Geometry> builder(back);
		std::unordered_map<Index_t, Index_t> pointsToBuilderIndex;

		std::unordered_set<Index_t> facesToDelete;
		for (auto fIter = front->FacesBegin(); fIter != facesEnd; ++fIter)
		{
			if (m_faceSide[*fIter] == Plane<float>::PlaneSide::Back)
			{
				LOG_TRACE(GeometryOperations, "Face %d is behind the plane", *fIter);
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
	void ClipFace(GeometryPtr geometry, Index_t face)
	{
		START_PROFILE;

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
			LOG_TRACE(GeometryOperations, "Splitting face %d from edge %d to edge %d", face, std::get<0>(e),
			          std::get<1>(e));
			auto newFace = geometry->SplitFace(face, std::get<0>(e), std::get<1>(e));
			if (CheckFaceSide(geometry, newFace) == Plane<float>::PlaneSide::Front)
			{
				m_faceSide[newFace] = Plane<float>::PlaneSide::Front;
				m_faceSide[face] = Plane<float>::PlaneSide::Back;
			}
			else
			{
				m_faceSide[newFace] = Plane<float>::PlaneSide::Back;
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
		LOG_TRACE(GeometryOperations, "Face %d is %s", face, to_string<float>(planeSide).c_str());
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
			LOG_TRACE(GeometryOperations, "Point %d (%f, %f, %f) is %s Plane", *pIter, pos[0], pos[1], pos[2],
			          to_string<float>(planeSide).c_str());
		}
	}

	void SplitEdges(GeometryPtr geometry)
	{
		START_PROFILE;

		std::unordered_map<Index_t, typename Geometry::PositionType> edgesToSplit;

		auto edgesEnd = geometry->EdgesEnd();
		for (auto eIter = geometry->EdgesBegin(); eIter != edgesEnd; ++eIter)
		{
			auto sourcePoint = geometry->GetPoint(geometry->GetSource(*eIter));
			auto destPoint = geometry->GetPoint(geometry->GetDestination(*eIter));

			if (!SameSide(sourcePoint, destPoint))
			{
				LOG_TRACE(GeometryOperations, "Points %d (%s) and %d (%s) are on different sides", sourcePoint,
				          to_string<float>(m_pointsSide[sourcePoint]).c_str(), destPoint,
				          to_string<float>(m_pointsSide[destPoint]).c_str());
				auto sourcePos = geometry->GetPosition(sourcePoint);
				auto destPos = geometry->GetPosition(destPoint);

				LOG_TRACE(GeometryOperations, " Source Position: %s", to_string(sourcePos).c_str());
				LOG_TRACE(GeometryOperations, " Destination Position: %s", to_string(destPos).c_str());

				auto line = Line3D<float>::FromTwoPoints(sourcePos, destPos);
				auto edgeIntersection = intersection(m_plane, line);

				LOG_TRACE(GeometryOperations, " Intersection with plane: %s",
				          to_string(edgeIntersection.m_intersection).c_str());

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
			LOG_TRACE(GeometryOperations, "Edge %d is a new edge", newEdge);
		}
	}

	bool SameSide(const Index_t &p1, const Index_t &p2) { return m_pointsSide[p1] == m_pointsSide[p2]; }

	std::unordered_set<Index_t> m_newEdges;
	std::unordered_map<Index_t, Plane<float>::PlaneSide> m_pointsSide;
	std::unordered_map<Index_t, Plane<float>::PlaneSide> m_faceSide;
	Plane<float> m_plane;
};
}  // namespace selector

#endif
