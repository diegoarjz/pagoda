#ifndef SELECTOR_GEOMETRY_OPERATIONS_CREATE_RECT_H_
#define SELECTOR_GEOMETRY_OPERATIONS_CREATE_RECT_H_

#include <common/logger.h>
#include <geometry_core/geometry.h>
#include <geometry_core/geometry_builder.h>
#include <geometry_core/geometry_sizes.h>

namespace selector
{
template<class G>
class CreateRect
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;

public:
	CreateRect(const float &width, const float &height) : m_width(width), m_height(height)
	{
		DBG_ASSERT_MSG(width != 0 && height != 0, "Can't create a rect with zero width or height");
	}

	GeometrySizes ResultSize() const { return GeometrySizes(4, 4, 1); }

	void Execute(GeometryPtr geometryOut)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "CreateRect. Width: " << m_width << " Height: " << m_height);

		Vec3F center(0, 0, 0);
		Vec3F width_direction(1, 0, 0);
		Vec3F height_direction(0, 1, 0);
		auto half_width = 0.5 * m_width;
		auto half_height = 0.5 * m_height;

		GeometryBuilderT<Geometry> builder(geometryOut);

		auto face = builder.StartFace(4);

		face.AddIndex(builder.AddPoint(center - half_width * width_direction - half_height * height_direction));
		face.AddIndex(builder.AddPoint(center + half_width * width_direction - half_height * height_direction));
		face.AddIndex(builder.AddPoint(center + half_width * width_direction + half_height * height_direction));
		face.AddIndex(builder.AddPoint(center - half_width * width_direction + half_height * height_direction));
		face.CloseFace();
	}

private:
	float m_width;
	float m_height;
};  // class CreateRect
}  // namespace selector

#endif
