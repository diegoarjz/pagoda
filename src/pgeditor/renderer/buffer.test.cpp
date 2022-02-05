#include "buffer.h"

#include <pagoda/math/vec_base.h>

#include <gtest/gtest.h>

#include <vector>

using namespace pgeditor::renderer;
using namespace pagoda::math;
using namespace pagoda::common;

TEST(Buffer, test_default_creation)
{
	Buffer b;
	EXPECT_EQ(b.GetBufferSize(), 0);
	EXPECT_EQ(b.GetBytesPerComponent(), 0);
	EXPECT_EQ(b.GetComponentsPerElement(), 0);
	EXPECT_EQ(b.GetNumElements(), 0);
	EXPECT_EQ(b.GetFormat(), Format::Invalid);
	EXPECT_EQ(b.GetData(), nullptr);
}

TEST(Buffer, test_setting_data)
{
	Buffer b;
	std::vector<Vec3F> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

	b.SetData(v);
	EXPECT_NE(b.GetData(), nullptr);
	EXPECT_EQ(b.GetBufferSize(), 3 * 3 * sizeof(float));
	EXPECT_EQ(b.GetBytesPerComponent(), sizeof(float));
	EXPECT_EQ(b.GetComponentsPerElement(), 3);
	EXPECT_EQ(b.GetNumElements(), 3);
	EXPECT_EQ(b.GetFormat(), Format::F32);
}

TEST(Buffer, test_getting_elements)
{
	Buffer b;
	std::vector<Vec3F> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	b.SetData(v);

	EXPECT_TRUE(*b.GetAt<Vec3F>(0) == (Vec3F{1, 2, 3}));
	EXPECT_TRUE(*b.GetAt<Vec3F>(1) == (Vec3F{4, 5, 6}));
	EXPECT_TRUE(*b.GetAt<Vec3F>(2) == (Vec3F{7, 8, 9}));

	EXPECT_TRUE(b.GetArrayAs<Vec3F>()[0] == (Vec3F{1, 2, 3}));
	EXPECT_TRUE(b.GetArrayAs<Vec3F>()[1] == (Vec3F{4, 5, 6}));
	EXPECT_TRUE(b.GetArrayAs<Vec3F>()[2] == (Vec3F{7, 8, 9}));
}

TEST(Buffer, test_out_of_bounds)
{
	Buffer b;

	std::vector<Vec3F> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	b.SetData(v);

	EXPECT_EQ(b.GetAt<Vec3F>(4), nullptr);
}

TEST(Buffer, test_different_types)
{
	Buffer b;

	std::vector<float> v{1, 2, 3, 4};
	b.SetData(v);

	EXPECT_EQ(b.GetAt<Vec3F>(0), nullptr);
}

TEST(Buffer, test_to_vector)
{
	Buffer b;
	std::vector<Vec3F> v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	b.SetData(v);
	std::vector<Vec3F> v2 = b.GetAsVector<Vec3F>();

	EXPECT_EQ(v2.size(), 3);
	for (auto i = 0; i < v.size(); ++i) {
		EXPECT_TRUE(v[i] == v2[i]);
	}
}

