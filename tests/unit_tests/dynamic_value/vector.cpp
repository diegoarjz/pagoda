#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/set_value_from.h>
#include <pagoda/dynamic/type_info.h>
#include <pagoda/dynamic/vector3.h>

#include <pagoda/math/vec_base.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;
using namespace pagoda::math;
using namespace pagoda::dynamic;

TEST(Vector3, test_constructions)
{
	EXPECT_TRUE((Vec3F{0, 0, 0}) == static_cast<Vec3F>(Vector3()));
	EXPECT_TRUE((Vec3F{1, 2, 3}) == static_cast<Vec3F>(Vector3(Vec3F{1, 2, 3})));
}

TEST(Vector3, test_get_value_as)
{
	Vector3 v(Vec3F{1, 2, 3});
	EXPECT_TRUE((Vec3F{1, 2, 3}) == get_value_as<Vec3F>(v));
}

TEST(Vector3, test_set_value_from)
{
	Vector3 v;
	set_value_from<Vec3F>(v, Vec3F{3, 2, 1});
	EXPECT_TRUE((Vec3F{3, 2, 1}) == static_cast<Vec3F>(v));
}

TEST(Vector3, test_members)
{
	Vector3 v(Vec3F{1, 2, 3});

	uint32_t i = 1;
	for (auto name : {"GetX", "GetY", "GetZ"}) {
		auto m = v.GetMember(name);
		ASSERT_NE(m, nullptr);
		auto f = std::dynamic_pointer_cast<Function>(m);
		ASSERT_NE(f, nullptr);

		auto r = std::dynamic_pointer_cast<FloatValue>(f->Call({}));
		ASSERT_NE(r, nullptr);

		EXPECT_EQ(static_cast<float>(*r), i++);
	}
}
