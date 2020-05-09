#pragma once

#include "geometry_core/geometry_builder.h"
#include "math_lib/matrix_base.h"

#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_mat_operations.hpp>

namespace pagoda
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
			Vec4F finalPos = m_matrix * XYZ1(pos);
			pos = XYZ(finalPos) / W(finalPos);
			LOG_TRACE(GeometryOperations, "Result: " << pos);
		}
	}

private:
	Mat4x4F m_matrix;
};
}  // namespace pagoda
