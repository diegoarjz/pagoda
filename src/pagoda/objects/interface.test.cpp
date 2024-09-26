#include "pagoda/objects/interface.h"
#include "procedural_object.h"

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::objects;

TEST(InterfaceTest, getters_and_setters)
{
	std::vector<std::tuple<std::string, Interface::Type, Interface::Arity>> testValues = {
	  {"interface", Interface::Type::Input, Interface::Arity::One},
	  {"interface2", Interface::Type::Output, Interface::Arity::All},
	};

	for (const auto& v : testValues) {
		auto i = std::make_shared<Interface>(std::get<0>(v), std::get<1>(v), std::get<2>(v));
		EXPECT_EQ(i->GetName(), std::get<0>(v));
		EXPECT_EQ(i->GetType(), std::get<1>(v));
		EXPECT_EQ(i->GetArity(), std::get<2>(v));
	}
}

TEST(InterfaceTest, output_interfaces_only_accept_input_interfaces)
{
	auto out = std::make_shared<Interface>("out", Interface::Type::Output);
	auto in = std::make_shared<Interface>("in", Interface::Type::Input);

	EXPECT_TRUE(out->Accepts(in));
	EXPECT_TRUE(in->Accepts(out));
	EXPECT_FALSE(out->Accepts(out));
	EXPECT_FALSE(in->Accepts(in));
}

TEST(InterfaceTest, one_arity_input_interfaces_only_accept_one_arity_output_interfaces)
{
	auto oneIn = std::make_shared<Interface>("one", Interface::Type::Input, Interface::Arity::One);
	auto oneOut = std::make_shared<Interface>("one", Interface::Type::Output, Interface::Arity::One);
	auto many = std::make_shared<Interface>("many", Interface::Type::Output, Interface::Arity::Many);
	auto all = std::make_shared<Interface>("all", Interface::Type::Output, Interface::Arity::All);

	EXPECT_TRUE(oneIn->Accepts(oneOut));
	EXPECT_FALSE(oneIn->Accepts(many));
	EXPECT_FALSE(oneIn->Accepts(all));
}

TEST(InterfaceTest, many_arity_input_interfaces_accept_all_output_interfaces)
{
	auto manyIn = std::make_shared<Interface>("manyIn", Interface::Type::Input, Interface::Arity::Many);
	auto manyOut = std::make_shared<Interface>("manyOut", Interface::Type::Output, Interface::Arity::Many);
	auto one = std::make_shared<Interface>("one", Interface::Type::Output, Interface::Arity::One);
	auto all = std::make_shared<Interface>("all", Interface::Type::Output, Interface::Arity::All);

	EXPECT_TRUE(manyIn->Accepts(one));
	EXPECT_TRUE(manyIn->Accepts(manyOut));
	EXPECT_TRUE(manyIn->Accepts(all));
}

TEST(InterfaceTest, all_arity_input_interfaces_accept_all_output_interfaces)
{
	auto allIn = std::make_shared<Interface>("all", Interface::Type::Input, Interface::Arity::All);
	auto many = std::make_shared<Interface>("manyOut", Interface::Type::Output, Interface::Arity::Many);
	auto one = std::make_shared<Interface>("one", Interface::Type::Output, Interface::Arity::One);
	auto all = std::make_shared<Interface>("all", Interface::Type::Output, Interface::Arity::All);

	EXPECT_TRUE(allIn->Accepts(one));
	EXPECT_TRUE(allIn->Accepts(many));
	EXPECT_TRUE(allIn->Accepts(all));
}

TEST(InterfaceTest, one_arity_input_interface_cannot_accept_more_than_one_output_interface)
{
	auto oneIn = std::make_shared<Interface>("one", Interface::Type::Input, Interface::Arity::One);
	auto oneOut = std::make_shared<Interface>("one", Interface::Type::Output, Interface::Arity::One);
	auto oneOut2 = std::make_shared<Interface>("one2", Interface::Type::Output, Interface::Arity::One);

	EXPECT_TRUE(oneIn->Accepts(oneOut));
	EXPECT_TRUE(oneIn->Connect(oneOut));
	EXPECT_TRUE(oneIn->Accepts(oneOut));
	EXPECT_FALSE(oneIn->Accepts(oneOut2));
}

TEST(InterfaceTest, test_connections)
{
	auto allIn = std::make_shared<Interface>("in", Interface::Type::Input, Interface::Arity::All);
	auto out1 = std::make_shared<Interface>("out", Interface::Type::Output, Interface::Arity::Many);
	auto out2 = std::make_shared<Interface>("out", Interface::Type::Output, Interface::Arity::Many);

	allIn->Connect(out1);
	allIn->ConnectedInterfaces([&](InterfacePtr i) { EXPECT_EQ(i, out1); });
	out1->ConnectedInterfaces([&](InterfacePtr i) { EXPECT_EQ(i, allIn); });
	EXPECT_EQ(allIn->ConnectionCount(), 1);
	EXPECT_EQ(out1->ConnectionCount(), 1);

	allIn->Connect(out2);
	allIn->ConnectedInterfaces([&](InterfacePtr i) { EXPECT_TRUE(i == out1 || i == out2); });
	EXPECT_EQ(allIn->ConnectionCount(), 2);
	EXPECT_EQ(out2->ConnectionCount(), 1);

	allIn->Disconnect(out1);
	allIn->ConnectedInterfaces([&](InterfacePtr i) { EXPECT_EQ(i, out2); });
	EXPECT_EQ(allIn->ConnectionCount(), 1);
	EXPECT_EQ(out1->ConnectionCount(), 0);

	out2->Disconnect(allIn);
	EXPECT_EQ(allIn->ConnectionCount(), 0);
	EXPECT_EQ(out2->ConnectionCount(), 0);
}
