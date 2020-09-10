#include <pagoda/objects/procedural_operation.h>
#include <memory>
#include "pagoda/dynamic/float_value.h"

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::objects;

class MockOperation : public ProceduralOperation
{
public:
	MockOperation() : ProceduralOperation(nullptr)
	{
		CreateInputInterface("in");
		CreateOutputInterface("out");

		RegisterValues({{"abc", std::make_shared<dynamic::FloatValue>(0.0f)}});
	}

protected:
	void DoWork() override
	{
		//
	}
};

class ProceduralOperationTest : public ::testing::Test
{
protected:
	void SetUp() {}

	void TearDown() {}
};

TEST_F(ProceduralOperationTest, can_check_existence_of_interface)
{
	auto op = std::make_shared<MockOperation>();
	EXPECT_TRUE(op->HasInputInterface("in"));
	EXPECT_TRUE(op->HasOutputInterface("out"));
	EXPECT_FALSE(op->HasInputInterface("_in"));
	EXPECT_FALSE(op->HasOutputInterface("_out"));
}

TEST_F(ProceduralOperationTest, can_register_parameters)
{
	auto op = std::make_shared<MockOperation>();
	EXPECT_NE(op->GetMember("abc"), nullptr);
}
