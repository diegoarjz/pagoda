#include <pagoda/geometry_core/geometry.h>
#include <pagoda/geometry_core/geometry_exporter.h>
#include <pagoda/geometry_operations/create_rect.h>
#include <pagoda/math_lib/vec_base.h>

#include "../test_utils.h"

#include <gtest/gtest.h>

#include <memory>

using namespace pagoda;

using GeometryType = GeometryBase<>;
using GeometryPtr = std::shared_ptr<GeometryType>;

class CreateRectTest : public PagodaTestFixture<::testing::Test>
{
public:
	void SetUp() {}

	void TearDown() {}
};

TEST_F(CreateRectTest, create_rect)
{
	CreateRect<GeometryType> create(10, 5);
	GeometryPtr out = std::make_shared<GeometryType>();
	create.Execute(out);

	Vec3F expected[] = {{-5, 2.5, 0}, {-5, -2.5, 0}, {5, -2.5, 0}, {5, 2.5, 0}};

	ASSERT_TRUE(out->IsValid());
	EXPECT_EQ(out->GetFaceCount(), 1);
	EXPECT_EQ(out->GetPointCount(), 4);
	EXPECT_EQ(out->GetSplitPointCount(), 4);
	EXPECT_EQ(out->GetEdgeCount(), 4);

	auto i = 0u;
	for (auto fIter = out->FacesBegin(); fIter != out->FacesEnd(); ++fIter)
	{
		for (auto fvCirc = out->FacePointCirculatorBegin(*fIter); fvCirc; ++fvCirc)
		{
			EXPECT_TRUE(out->GetPosition(*fvCirc) == expected[i]);
			++i;
		}
	}

	ObjExporter<GeometryType> exporter(out);
	std::stringstream ss;
	exporter.Export(ss);
	MatchFile match(GetCurrentTestFileResultsDirectory() /= "geometry.obj", GetShouldWriteFiles());
	match.Match(ss.str());
}
