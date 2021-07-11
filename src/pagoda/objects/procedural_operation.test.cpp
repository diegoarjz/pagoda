#include <pagoda/objects/procedural_operation.h>
#include "pagoda/dynamic/float_value.h"

#include <gtest/gtest.h>

#include <memory>
#include <unordered_set>

using namespace pagoda;
using namespace pagoda::objects;

class MockOperation : public ProceduralOperation
{
	public:
	MockOperation() : ProceduralOperation(nullptr)
	{
		CreateInputInterface("in");
		CreateInputInterface("in2");
		CreateOutputInterface("out");
		CreateOutputInterface("out2");
	}
	~MockOperation() override {}

	void SetParameters(graph::ExecutionArgumentCallback* cb) override
	{
		RegisterMember("abc", std::make_shared<dynamic::FloatValue>(0.0f));
	}

	const std::string& GetOperationName() const override
	{
		static const std::string sName{"MockOperation"};
		return sName;
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

TEST_F(ProceduralOperationTest, can_iterate_over_interfaces)
{
	std::unordered_set<std::string> in;
	std::unordered_set<std::string> out;

	auto op = std::make_shared<MockOperation>();
	op->ForEachInputInterface([&in](const std::string& i) { in.insert(i); });
	op->ForEachOutputInterface([&out](const std::string& i) { out.insert(i); });

	EXPECT_EQ(in.size(), 2u);
	EXPECT_EQ(out.size(), 2u);

	EXPECT_NE(in.find("in"), in.end());
	EXPECT_NE(in.find("in2"), in.end());
	EXPECT_NE(out.find("out"), out.end());
	EXPECT_NE(out.find("out2"), out.end());
}
