#pragma once

#include "geometry_core/geometry_builder.h"
#include "math_lib/matrix_base.h"

namespace selector
{
template<class G>
class MatrixTransform
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;

public:
	MatrixTransform(const Mat4x4F& matrix) : m_matrix(matrix) {}

	void Execute(GeometryPtr geometryIn, GeometryPtr geometryOut)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "Transforming with matrix");
		LOG_TRACE(GeometryOperations, m_matrix);

		*geometryOut = *geometryIn;

		for (auto iter = geometryOut->PointsBegin(); iter != geometryOut->PointsEnd(); ++iter)
		{
			auto& pos = geometryOut->GetPosition(*iter);
			LOG_TRACE(GeometryOperations, "Applying matrix to " << pos);
			Vec4F finalPos = m_matrix * Vec4F(pos.X(), pos.Y(), pos.Z(), 1.0f);
			pos = Vec3F(finalPos.X(), finalPos.Y(), finalPos.Z()) / finalPos.W();
			LOG_TRACE(GeometryOperations, "Result: " << pos);
		}
	}

private:
	Mat4x4F m_matrix;
};
}  // namespace selector
