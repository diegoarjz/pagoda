#pragma once

#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/math/matrix_base.h>

#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_mat_operations.hpp>

namespace pagoda::geometry::algorithms
{
template<class G>
class MatrixTransform
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;

public:
	MatrixTransform(const math::Mat4x4F& matrix) : m_matrix(matrix) {}

	void Execute(GeometryPtr geometryIn, GeometryPtr geometryOut)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "Transforming with matrix");

		*geometryOut = *geometryIn;

		for (auto iter = geometryOut->PointsBegin(); iter != geometryOut->PointsEnd(); ++iter)
		{
			auto& pos = geometryOut->GetPosition(*iter);
			math::Vec4F finalPos = m_matrix * XYZ1(pos);
			pos = XYZ(finalPos) / W(finalPos);
		}
	}

private:
	math::Mat4x4F m_matrix;
};
}  // namespace pagoda::geometry::algorithms
