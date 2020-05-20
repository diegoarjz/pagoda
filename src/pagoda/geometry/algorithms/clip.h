#ifndef PAGODA_GEOMETRY_OPERATIONS_CLIP_H_
#define PAGODA_GEOMETRY_OPERATIONS_CLIP_H_

#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/math/intersection.h>
#include <pagoda/math/line_3d.h>
#include <pagoda/math/plane.h>
#include <pagoda/math/vec_base.h>

namespace pagoda::geometry::algorithms
{
template<class G>
class Clip
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;
	using Index_t = typename Geometry::Index_t;

public:
	Clip(const math::Plane<float> &plane) : m_plane(plane) {}

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

		core::GeometryBuilderT<Geometry> builder(back);
		std::map<Index_t, Index_t> pointsToBuilderIndex;

		std::set<Index_t> facesToDelete;
		for (auto fIter = front->FacesBegin(); fIter != front->FacesEnd(); ++fIter)
		{
			if (m_faceSide[*fIter] == math::Plane<float>::PlaneSide::Back)
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
	void ClipFace(GeometryPtr geometry, const core::SplitPointTopology::FaceHandle &face)
	{
		START_PROFILE;

		LOG_TRACE(GeometryOperations, "Clipping Face " << face);

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
				LOG_TRACE(GeometryOperations, "Into face " << f);
			}
#endif

			if (faceSide == math::Plane<float>::PlaneSide::Front)
			{
				m_faceSide[face] = math::Plane<float>::PlaneSide::Back;
			}
			else if (faceSide == math::Plane<float>::PlaneSide::Back)
			{
				m_faceSide[face] = math::Plane<float>::PlaneSide::Front;
			}
			else  // Contained
			{
				m_faceSide[face] = math::Plane<float>::PlaneSide::Front;
			}
		}
	}

	typename math::Plane<float>::PlaneSide CheckFaceSide(GeometryPtr geometry, const Index_t &face)
	{
		typename math::Plane<float>::PlaneSide planeSide;
		for (auto fpIter = geometry->FacePointCirculatorBegin(face); fpIter; ++fpIter)
		{
			auto pos = geometry->GetPosition(*fpIter);
			planeSide = m_plane.GetPlaneSide(pos);
			if (planeSide != math::Plane<float>::PlaneSide::Contained)
			{
				break;
			}
		}
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
				auto sourcePos = geometry->GetPosition(sourcePoint);
				auto destPos = geometry->GetPosition(destPoint);

				auto line = math::Line3D<float>::FromTwoPoints(sourcePos, destPos);
				auto edgeIntersection = math::intersection(m_plane, line);

				auto edgesBetweenPoints = geometry->GetEdges(sourcePoint, destPoint);
				for (const auto &e : edgesBetweenPoints)
				{
					edgesToSplit[e] = edgeIntersection.m_intersection;
				}
			}
			else if (m_pointsSide[sourcePoint] == math::Plane<float>::PlaneSide::Back)
			{
				// It's a face behind the plane
				m_faceSide[geometry->GetFace(*eIter)] = math::Plane<float>::PlaneSide::Back;
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

		return p1Side == p2Side || p1Side == math::Plane<float>::PlaneSide::Contained ||
		       p2Side == math::Plane<float>::PlaneSide::Contained;
	}

	std::set<Index_t> m_newEdges;
	std::map<Index_t, math::Plane<float>::PlaneSide> m_pointsSide;
	std::map<Index_t, math::Plane<float>::PlaneSide> m_faceSide;
	math::Plane<float> m_plane;
};
}  // namespace pagoda::geometry::algorithms

#endif
