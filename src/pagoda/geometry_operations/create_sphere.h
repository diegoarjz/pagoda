#pragma once

#include <pagoda/common/debug/logger.h>
#include <pagoda/math_lib/math_utils.h>

#include <boost/qvm/vec_operations.hpp>

namespace pagoda
{
template<class G>
class CreateSphere
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;

public:
	CreateSphere(const float &radius, uint32_t slices, uint32_t stacks)
	    : m_radius(radius), m_slices(slices), m_stacks(stacks)
	{
	}

	void Execute(GeometryPtr geometryOut)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations,
		          "CreateSphere. Radius: " << m_radius << " Slices: " << m_slices << " Stacks: " << m_stacks);

		Vec3F top = {0, 0, m_radius};
		Vec3F bottom{0, 0, -m_radius};

		LOG_TRACE(GeometryOperations, "Top: " << top);
		LOG_TRACE(GeometryOperations, "Bottom: " << bottom);

		float sliceIncrementAngle = MathUtils<float>::two_pi / m_slices;
		float stackIncrementAngle = MathUtils<float>::pi / static_cast<float>(2 + (m_stacks - 1));

		LOG_TRACE(GeometryOperations, "Slice Increment Angle: " << sliceIncrementAngle);
		LOG_TRACE(GeometryOperations, "Stack Increment Angle: " << stackIncrementAngle);

		float omega = 0;
		float theta = stackIncrementAngle;

		std::vector<typename Geometry::Index_t> points;
		points.reserve(2 + m_slices * m_stacks);

		GeometryBuilderT<Geometry> builder(geometryOut);

		for (auto st = 0u; st < m_stacks; ++st, theta += stackIncrementAngle)
		{
			for (auto sl = 0u; sl < m_slices; ++sl, omega += sliceIncrementAngle)
			{
				auto sinTheta = std::sin(theta);
				points.push_back(builder.AddPoint(
				    m_radius * Vec3F{sinTheta * std::cos(omega), sinTheta * std::sin(omega), std::cos(theta)}));
			}
		}
		auto topIndex = builder.AddPoint(top);
		auto bottomIndex = builder.AddPoint(bottom);

		for (auto st = 0u; st < m_stacks - 1; ++st)
		{
			for (auto sl = 0u; sl < m_slices; ++sl)
			{
				auto p1 = sl + st * m_slices;
				auto p2 = sl + (st + 1) * m_slices;
				auto p3 = (sl + 1) % m_slices + (st + 1) * m_slices;
				auto p4 = (sl + 1) % m_slices + st * m_slices;

				auto face = builder.StartFace(4);
				face.AddIndex(points[p1]);
				face.AddIndex(points[p2]);
				face.AddIndex(points[p3]);
				face.AddIndex(points[p4]);
				face.CloseFace();
			}
		}

		for (auto sl = 0u; sl < m_slices; ++sl)
		{
			auto topFace = builder.StartFace(3);
			topFace.AddIndex(topIndex);
			topFace.AddIndex(points[sl]);
			topFace.AddIndex(points[(sl + 1) % m_slices]);
			topFace.CloseFace();

			auto bottomFace = builder.StartFace(3);
			bottomFace.AddIndex(points[points.size() - (m_slices - sl)]);
			bottomFace.AddIndex(bottomIndex);
			bottomFace.AddIndex(points[points.size() - (m_slices - (sl + 1) % m_slices)]);
			bottomFace.CloseFace();
		}
	}

private:
	float m_radius;
	uint32_t m_slices;
	uint32_t m_stacks;
};
}  // namespace pagoda
