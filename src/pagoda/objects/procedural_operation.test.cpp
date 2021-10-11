#include "pagoda/dynamic/float_value.h"
#include "pagoda/objects/interface_callback.h"
#include "pagoda/objects/procedural_operation.h"

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
	~MockOperation() override
	{
	}

	void SetParameters(graph::ExecutionArgumentCallback* cb) override
	{
		RegisterMember("abc", std::make_shared<dynamic::FloatValue>(0.0f));
	}

	const std::string& GetOperationName() const override
	{
		static const std::string sName{"MockOperation"};
		return sName;
	}

	void Interfaces(InterfaceCallback* cb) override
	{
		cb->InputInterface(m_in, "in", "In", Interface::Arity::Many);
		cb->InputInterface(m_in2, "in2", "In2", Interface::Arity::Many);
		cb->OutputInterface(m_out, "out", "Out", Interface::Arity::Many);
		cb->OutputInterface(m_out2, "out2", "Out2", Interface::Arity::Many);
	}

	bool m_called{false};

	protected:
	void DoWork() override
	{
		m_called = true;
	}

	objects::InterfacePtr m_in;
	objects::InterfacePtr m_in2;
	objects::InterfacePtr m_out;
	objects::InterfacePtr m_out2;
};

class ProceduralOperationTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
	}

	void TearDown()
	{
	}
};

TEST_F(ProceduralOperationTest, calling_execute_should_call_do_work)
{
	MockOperation op;
	op.Execute();
	EXPECT_TRUE(op.m_called);
}

