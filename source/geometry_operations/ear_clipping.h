#pragma once

#include <geometry_core/geometry_builder.h>

#include <common/logger.h>

#include <math_lib/math_utils.h>

#include <vector>

namespace pagoda
{
template<class G>
class EarClipping
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;
	using Index_t = typename Geometry::Index_t;

	struct VertexLinkedListNode
	{
		VertexLinkedListNode(const Index_t splitPoint, const Vec3F &position)
		    : m_splitPoint(splitPoint), m_position(position)
		{
		}

		Index_t m_splitPoint;
		Vec3F m_position;
		VertexLinkedListNode *m_next;
		VertexLinkedListNode *m_prev;
	};

	struct Triangle
	{
		Triangle(typename Geometry::PointHandle p0, typename Geometry::PointHandle p1,
		         typename Geometry::PointHandle p2)
		    : m_p0(p0), m_p1(p1), m_p2(p2)
		{
		}

		typename Geometry::PointHandle m_p0;
		typename Geometry::PointHandle m_p1;
		typename Geometry::PointHandle m_p2;
	};

	struct Sorter
	{
		bool operator()(const VertexLinkedListNode *lhs, const VertexLinkedListNode *rhs) const
		{
			return lhs->m_splitPoint < rhs->m_splitPoint;
		}
	};

public:
	EarClipping() {}

	void Execute(GeometryPtr geometryIn, GeometryPtr geometryOut)
	{
		START_PROFILE;

		std::vector<Triangle> triangles;

		for (auto faceIter = geometryIn->FacesBegin(); faceIter != geometryIn->FacesEnd(); ++faceIter)
		{
			std::vector<VertexLinkedListNode> vertexLinkedList;
			Vec3F faceNormal = geometryIn->GetFaceAttributes(*faceIter).m_normal;

			std::set<VertexLinkedListNode *, Sorter> reflexVertices;
			std::set<VertexLinkedListNode *, Sorter> convexVertices;
			std::set<VertexLinkedListNode *, Sorter> earTips;

			for (auto fspIter = geometryIn->FaceSplitPointCirculatorBegin(*faceIter); fspIter; ++fspIter)
			{
				Vec3F curr = geometryIn->GetPosition(geometryIn->GetPoint(*fspIter));
				vertexLinkedList.emplace_back(*fspIter, curr);
			}

			for (auto i = 0u; i < vertexLinkedList.size(); ++i)
			{
				auto prevIndex = i == 0 ? vertexLinkedList.size() - 1 : i - 1;
				auto nextIndex = (i + 1) % vertexLinkedList.size();
				vertexLinkedList[i].m_next = &(vertexLinkedList[nextIndex]);
				vertexLinkedList[i].m_prev = &(vertexLinkedList[prevIndex]);

				Vec3F prev = vertexLinkedList[i].m_prev->m_position;
				Vec3F curr = vertexLinkedList[i].m_position;
				Vec3F next = vertexLinkedList[i].m_next->m_position;

				auto dot = boost::qvm::dot(boost::qvm::cross((next - curr), (prev - curr)), faceNormal);
				if (dot < 0)
				{
					reflexVertices.insert(&(vertexLinkedList[i]));
				}
				else if (dot > 0)
				{
					convexVertices.insert(&(vertexLinkedList[i]));
				}
			}

			for (const auto &n : convexVertices)
			{
				if (isEar(*n, reflexVertices))
				{
					earTips.insert(n);
				}
			}

			uint32_t trianglesFound = 0;
			// in a polygon with n vertices there are always n-2 trianges.
			while (!earTips.empty() && trianglesFound < vertexLinkedList.size() - 2)
			{
				VertexLinkedListNode *tip = *earTips.begin();
				earTips.erase(tip);
				++trianglesFound;

				// remove the tip
				VertexLinkedListNode *next = tip->m_next;
				VertexLinkedListNode *prev = tip->m_prev;

				next->m_prev = prev;
				prev->m_next = next;
				tip->m_next = nullptr;
				tip->m_prev = nullptr;

				for (const auto &adj : {prev, next})
				{
					bool isConvex = false;

					Vec3F prevPos = adj->m_prev->m_position;
					Vec3F currPos = adj->m_position;
					Vec3F nextPos = adj->m_next->m_position;

					auto dot = boost::qvm::dot(boost::qvm::cross((nextPos - currPos), (prevPos - currPos)), faceNormal);
					if (dot < 0)
					{
						reflexVertices.insert(adj);
						convexVertices.erase(adj);
					}
					else if (dot > 0)
					{
						convexVertices.insert(adj);
						reflexVertices.erase(adj);
						isConvex = true;
					}

					if (isConvex)
					{
						if (isEar(*adj, reflexVertices))
						{
							earTips.insert(adj);
						}
						else
						{
							earTips.erase(adj);
						}
					}
				}

				auto p0 = geometryIn->GetPoint(prev->m_splitPoint);
				auto p1 = geometryIn->GetPoint(tip->m_splitPoint);
				auto p2 = geometryIn->GetPoint(next->m_splitPoint);

				triangles.emplace_back(p0, p1, p2);
			}
		}

		GeometryBuilderT<Geometry> builder(geometryOut);
		std::unordered_map<Index_t, Index_t> pointsMap;
		auto pointEndIter = geometryIn->PointsEnd();
		for (auto pointIter = geometryIn->PointsBegin(); pointIter != pointEndIter; ++pointIter)
		{
			auto vertPos = geometryIn->GetPosition(*pointIter);
			pointsMap[*pointIter] = builder.AddPoint(vertPos);
		}

		for (const auto &t : triangles)
		{
			auto faceBuilder = builder.StartFace(3);
			faceBuilder.AddIndex(pointsMap[t.m_p0]);
			faceBuilder.AddIndex(pointsMap[t.m_p1]);
			faceBuilder.AddIndex(pointsMap[t.m_p2]);
			faceBuilder.CloseFace();
		}
	}

private:
	bool isEar(VertexLinkedListNode &v, std::set<VertexLinkedListNode *, Sorter> reflexVertices)
	{
		Vec3F p0 = v.m_prev->m_position;
		Vec3F p1 = v.m_position;
		Vec3F p2 = v.m_next->m_position;

		for (const auto rv : reflexVertices)
		{
			if (rv->m_splitPoint == v.m_splitPoint || rv->m_splitPoint == v.m_prev->m_splitPoint ||
			    rv->m_splitPoint == v.m_next->m_splitPoint)
			{
				continue;
			}

			if (pointInsideTriangle(rv->m_position, p0, p1, p2))
			{
				return false;
			}
		}

		return true;
	}

	bool pointInsideTriangle(const Vec3F &P, const Vec3F &a, const Vec3F &b, const Vec3F &c)
	{
		Vec3F e0 = c - a;
		Vec3F e1 = b - a;
		Vec3F e2 = P - a;

		float dot00 = boost::qvm::dot(e0, e0);
		float dot01 = boost::qvm::dot(e0, e1);
		float dot02 = boost::qvm::dot(e0, e2);
		float dot11 = boost::qvm::dot(e1, e1);
		float dot12 = boost::qvm::dot(e1, e2);

		// Compute barycentric coordinates
		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		return (u >= 0) && (v >= 0) && (u + v < 1);
	}
};
}  // namespace pagoda
