#include <parameter/context.h>
#include <parameter/float_parameter.h>
#include <parameter/parameter.h>
#include <parameter/parameter_identifier.h>
#include <parameter/parameter_resolver.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(TestParameterPaths, valid)
{
	std::vector<std::string> values{"a",
	                                "an_identifier",
	                                "/absolute/path",
	                                "../relative_path_up",
	                                "relative/path",
	                                "./relative",
	                                "./relative/path",
	                                "/something/./../weird",
	                                "./something/./../weird",
	                                "something/../weird",
	                                "something/./../weird"};

	for (auto s : values)
	{
		EXPECT_TRUE(ParameterPath::IsValidPath(s));
	}
}

TEST(TestParameterPaths, invalid)
{
	std::vector<std::string> values{"21",
	                                "..an_identifier",
	                                "..a_context/an_identifier",
	                                "ending/in/context/",
	                                "?invalid_identifiers",
	                                "//invalid",
	                                "//something//../weird",
	                                "..../invalid",
	                                "/..../invalid",
	                                "/.../.../invalid",
	                                "invalid/.../invalid",
	                                "/",
	                                ".",
	                                "..",
	                                ""};
	for (auto s : values)
	{
		EXPECT_FALSE(ParameterPath::IsValidPath(s));
	}
}

TEST(ParameterPath, test_create_single_component)
{
	ParameterPath p = ParameterPath::CreatePath("name");

	ASSERT_EQ(p.GetComponents().size(), 1);
	EXPECT_EQ(p.GetComponents().back().m_componentType, ParameterPath::PathComponent::Type::Name);
	EXPECT_EQ(p.GetComponents().back().m_name, "name");
}

TEST(ParameterPath, test_create_absolute)
{
	ParameterPath p = ParameterPath::CreatePath("/name");

	ASSERT_EQ(p.GetComponents().size(), 2);
	EXPECT_EQ(p.GetComponents().front().m_componentType, ParameterPath::PathComponent::Type::Root);

	EXPECT_EQ(p.GetComponents().back().m_componentType, ParameterPath::PathComponent::Type::Name);
	EXPECT_EQ(p.GetComponents().back().m_name, "name");
}

TEST(ParameterPath, test_create_relative_one_up)
{
	ParameterPath p = ParameterPath::CreatePath("../name");

	ASSERT_EQ(p.GetComponents().size(), 2);
	EXPECT_EQ(p.GetComponents().front().m_componentType, ParameterPath::PathComponent::Type::Parent);

	EXPECT_EQ(p.GetComponents().back().m_componentType, ParameterPath::PathComponent::Type::Name);
	EXPECT_EQ(p.GetComponents().back().m_name, "name");
}

TEST(ParameterPath, test_create_relative_current)
{
	ParameterPath p = ParameterPath::CreatePath("./name");

	ASSERT_EQ(p.GetComponents().size(), 2);
	EXPECT_EQ(p.GetComponents().front().m_componentType, ParameterPath::PathComponent::Type::Current);

	EXPECT_EQ(p.GetComponents().back().m_componentType, ParameterPath::PathComponent::Type::Name);
	EXPECT_EQ(p.GetComponents().back().m_name, "name");
}

TEST(ParameterPath, test_create_relative_multiple)
{
	ParameterPath p = ParameterPath::CreatePath("./name/name2");

	ParameterPath::PathComponent::Type types[] = {ParameterPath::PathComponent::Type::Current,
	                                              ParameterPath::PathComponent::Type::Name,
	                                              ParameterPath::PathComponent::Type::Name};
	std::string names[] = {"", "name", "name2"};

	ASSERT_EQ(p.GetComponents().size(), 3);
	for (auto i = 0; i < 3; ++i)
	{
		EXPECT_EQ(p.GetComponents()[i].m_componentType, types[i]);
		EXPECT_EQ(p.GetComponents()[i].m_name, names[i]);
	}
}

class ParameterResolverTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		rootContext = std::make_shared<Context>("root");
		A = std::make_shared<Context>("A");
		A->SetParent(rootContext);
		B = std::make_shared<Context>("B");
		B->SetParent(rootContext);

		resolver = std::make_shared<ParameterResolver>(rootContext);

		parameter1 = rootContext->CreateParameter<FloatParameter>("floatParameter1", 1.0);
		parameterA = A->CreateParameter<FloatParameter>("floatParameterA", 1.0);
		parameterB = B->CreateParameter<FloatParameter>("floatParameterB", 1.0);
	}

	void TearDown() {}

	std::shared_ptr<Context> rootContext;
	std::shared_ptr<ParameterBase> parameter1;
	std::shared_ptr<Context> A;
	std::shared_ptr<Context> B;
	std::shared_ptr<ParameterBase> parameterA;
	std::shared_ptr<ParameterBase> parameterB;

	std::shared_ptr<ParameterResolver> resolver;
};  // class ParameterResolverTest

TEST_F(ParameterResolverTest, test_get_directly_from_context)
{
	auto par = resolver->Resolve("floatParameter1", rootContext);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameter1);
}

TEST_F(ParameterResolverTest, test_get_from_context_with_relative_path)
{
	auto par = resolver->Resolve("./floatParameter1", rootContext);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameter1);
}

TEST_F(ParameterResolverTest, test_navigate_down_one_context)
{
	auto par = resolver->Resolve("A/floatParameterA", rootContext);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameterA);
}

TEST_F(ParameterResolverTest, test_absolute_path)
{
	auto par = resolver->Resolve("/A/floatParameterA", rootContext);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameterA);
}

TEST_F(ParameterResolverTest, test_absolute_path_parameter_in_root)
{
	auto par = resolver->Resolve("/floatParameter1", rootContext);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameter1);
}

TEST_F(ParameterResolverTest, test_navigate_up_one_context)
{
	auto par = resolver->Resolve("../floatParameter1", A);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameter1);
}

TEST_F(ParameterResolverTest, test_navigate_up_and_down_one_context)
{
	auto par = resolver->Resolve("../A/floatParameterA", A);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameterA);
}

TEST_F(ParameterResolverTest, test_navigate_up_and_down_to_other_context)
{
	auto par = resolver->Resolve("../B/floatParameterB", B);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameterB);
}

TEST_F(ParameterResolverTest, test_navigate_up_from_root)
{
	auto par = resolver->Resolve("../something", rootContext);

	ASSERT_EQ(par, nullptr);
}

TEST_F(ParameterResolverTest, test_navigate_down_non_existing_context)
{
	auto par = resolver->Resolve("/C/something", rootContext);
	ASSERT_EQ(par, nullptr);
}

TEST_F(ParameterResolverTest, test_non_existing_parameter)
{
	auto par = resolver->Resolve("something", rootContext);
	ASSERT_EQ(par, nullptr);
}

TEST_F(ParameterResolverTest, test_absolute_from_non_root_context)
{
	auto par = resolver->Resolve("/floatParameter1", B);
	ASSERT_EQ(par, parameter1);
}

// Resolve with ParameterPath
TEST_F(ParameterResolverTest, test_get_directly_from_context_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("floatParameter1"), rootContext);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameter1);
}

TEST_F(ParameterResolverTest, test_get_from_context_with_relative_path_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("./floatParameter1"), rootContext);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameter1);
}

TEST_F(ParameterResolverTest, test_navigate_down_one_context_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("A/floatParameterA"), rootContext);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameterA);
}

TEST_F(ParameterResolverTest, test_absolute_path_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("/A/floatParameterA"), rootContext);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameterA);
}

TEST_F(ParameterResolverTest, test_absolute_path_parameter_in_root_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("/floatParameter1"), rootContext);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameter1);
}

TEST_F(ParameterResolverTest, test_navigate_up_one_context_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("../floatParameter1"), A);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameter1);
}

TEST_F(ParameterResolverTest, test_navigate_up_and_down_one_context_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("../A/floatParameterA"), A);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameterA);
}

TEST_F(ParameterResolverTest, test_navigate_up_and_down_to_other_context_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("../B/floatParameterB"), B);

	ASSERT_NE(par, nullptr);
	ASSERT_EQ(par, parameterB);
}

TEST_F(ParameterResolverTest, test_navigate_up_from_root_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("../something"), rootContext);

	ASSERT_EQ(par, nullptr);
}

TEST_F(ParameterResolverTest, test_navigate_down_non_existing_context_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("/C/something"), rootContext);
	ASSERT_EQ(par, nullptr);
}

TEST_F(ParameterResolverTest, test_non_existing_parameter_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("something"), rootContext);
	ASSERT_EQ(par, nullptr);
}

TEST_F(ParameterResolverTest, test_absolute_from_non_root_context_parameter_path)
{
	auto par = resolver->Resolve(ParameterPath::CreatePath("/floatParameter1"), B);
	ASSERT_EQ(par, parameter1);
}
