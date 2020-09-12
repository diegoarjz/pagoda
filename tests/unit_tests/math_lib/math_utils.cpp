#include <pagoda/math/degrees.h>
#include <pagoda/math/math_utils.h>
#include <pagoda/math/radians.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::math;

using mt = MathUtils<float>;

TEST(ConversionTests, RadiansToDegrees)
{
	std::vector<std::pair<float, float>> data = {
	  {0, 0}, {mt::half_pi, 90}, {mt::pi, 180}, {mt::pi + mt::half_pi, 270}, {mt::two_pi, 360}};
	for (auto d : data) {
		EXPECT_EQ(mt::radians_to_degrees(std::get<0>(d)), std::get<1>(d));
	}
}

TEST(ConversionTests, DegreesToRadians)
{
	std::vector<std::pair<float, float>> data = {
	  {0, 0}, {mt::half_pi, 90}, {mt::pi, 180}, {mt::pi + mt::half_pi, 270}, {mt::two_pi, 360}};
	for (auto d : data) {
		EXPECT_EQ(mt::degrees_to_radians(std::get<1>(d)), std::get<0>(d));
	}
}

TEST(DegreesTest, when_constructing_from_pod_should_set_value_as_degrees)
{
	Degrees<float> d(180);
	EXPECT_EQ(static_cast<float>(d), 180);
}

TEST(DegreesTest, when_constructing_from_radians_should_convert_to_degrees)
{
	Radians<float> r(mt::half_pi);
	Degrees<float> d(r);
	EXPECT_EQ(static_cast<float>(d), 90);
}

TEST(RadiansTest, when_constructing_from_pod_should_set_value_as_degrees)
{
	Radians<float> r(mt::half_pi);
	EXPECT_EQ(static_cast<float>(r), mt::half_pi);
}

TEST(RadiansTest, when_constructing_from_degrees_should_convert_to_degrees)
{
	Degrees<float> d(90);
	Radians<float> r(d);
	EXPECT_EQ(static_cast<float>(r), mt::half_pi);
}
