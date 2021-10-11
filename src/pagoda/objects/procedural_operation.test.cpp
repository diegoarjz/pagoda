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

  void Interfaces(InterfaceCallback *cb) override
  {
    cb->InputInterface(m_in, "in", "In", Interface::Arity::Many);
    cb->InputInterface(m_in2, "in2", "In2", Interface::Arity::Many);
    cb->OutputInterface(m_out, "out", "Out", Interface::Arity::Many);
    cb->OutputInterface(m_out2, "out2", "Out2", Interface::Arity::Many);
  }

	protected:
	void DoWork() override
	{
		//
	}

  objects::InterfacePtr m_in;
  objects::InterfacePtr m_in2;
  objects::InterfacePtr m_out;
  objects::InterfacePtr m_out2;
};

class ProceduralOperationTest : public ::testing::Test
{
	protected:
	void SetUp() {}

	void TearDown() {}
};

/*
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
*/
