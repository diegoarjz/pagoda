#include <geometry_core/geometry.h>
#include <geometry_operations/create_rect.h>
#include <math_lib/vec_base.h>

#include <gtest/gtest.h>

#include <memory>

using namespace selector;

using GeometryType = GeometryBase<>;
using GeometryPtr = std::shared_ptr<GeometryType>;

TEST(CreateRectGeometryOperation, create_rect)
{
    /*
	CreateRect<GeometryType> create(10, 5);
	GeometryPtr out = std::make_shared<GeometryType>();
	create.Execute(out);

	Vec3F expected[] = {{-5, -2.5, 0}, {5, -2.5, 0}, {5, 2.5, 0}, {-5, 2.5, 0}};

	auto i = 0u;
	for (auto f_iter = out->FacesBegin(); f_iter != out->FacesEnd(); ++f_iter)
	{
		for (auto face_vert_circ = out->FaceVertexBegin(*f_iter); face_vert_circ.IsValid(); ++face_vert_circ)
		{
			auto point = out->GetVertexAttributes(*face_vert_circ).m_position;
			ASSERT_EQ(point, expected[i]);
			++i;
		}
	}
    */
}
