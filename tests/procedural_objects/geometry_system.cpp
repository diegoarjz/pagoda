#include "../parameter/mock_objects.h"
#include "mock_objects.h"

#include <parameter/context.h>
#include <parameter/float_parameter.h>
#include <procedural_objects/create_rect.h>
#include <procedural_objects/extrude_geometry.h>
#include <procedural_objects/geometry_component.h>
#include <procedural_objects/geometry_system.h>
#include <procedural_objects/hierarchical_component.h>
#include <procedural_objects/hierarchical_system.h>
#include <procedural_objects/procedural_object.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace selector;

class GeometrySystemTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		geometry_system = std::make_shared<GeometrySystem>();
		geometry = std::make_shared<Geometry>(5, 5, 5);
	}

	void TearDown()
	{
		if (component != nullptr)
		{
			geometry_system->KillProceduralComponent(component);
		}
	}

	std::shared_ptr<GeometrySystem> geometry_system;
	std::shared_ptr<GeometryComponent> component;
	GeometryPtr geometry;
};

TEST_F(GeometrySystemTest, test_create_component)
{
	auto procedural_component = geometry_system->CreateComponent();
	component = std::dynamic_pointer_cast<GeometryComponent>(procedural_component);

	EXPECT_EQ(procedural_component->Type(), ComponentType::Geometry);
	EXPECT_NE(component, nullptr);

	auto components = geometry_system->GetComponents();
	EXPECT_EQ(components.size(), 1);
	auto c = *components.begin();
	EXPECT_EQ(c, std::dynamic_pointer_cast<GeometryComponent>(component));
}

TEST_F(GeometrySystemTest, test_create_component_with_geometry)
{
	component = geometry_system->CreateComponent(geometry);

	EXPECT_EQ(component->Type(), ComponentType::Geometry);
	EXPECT_EQ(component->GetGeometry(), geometry);

	auto components = geometry_system->GetComponents();
	EXPECT_EQ(components.size(), 1);
	EXPECT_EQ(*components.begin(), component);
}

TEST_F(GeometrySystemTest, test_kill_component)
{
	auto component = geometry_system->CreateComponent();

	geometry_system->KillProceduralComponent(component);
	auto components = geometry_system->GetComponents();
	EXPECT_EQ(components.size(), 0);
}

class CreateRectTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		context = std::make_shared<ContextMock>(ParameterIdentifier::CreateIdentifier("context").second);
		geometry_system_mock = std::make_shared<GeometrySystemMock>();
		procedural_object_system = std::make_shared<ProceduralObjectSystemMock>();

		exection_context = std::make_shared<OperationExecutionContext>();
		exection_context->geometry_system = geometry_system_mock;
		exection_context->procedural_object_system = procedural_object_system;
		exection_context->parameter_context = context;

		create_rect = std::make_shared<CreateRectGeometry>();

		width_par = std::make_shared<Parameter<FloatParameter>>(ParameterIdentifier::CreateIdentifier("width").second);
		height_par =
		    std::make_shared<Parameter<FloatParameter>>(ParameterIdentifier::CreateIdentifier("height").second);

		out_mask.set(static_cast<uint32_t>(ComponentType::Geometry));
		out_mask.set(static_cast<uint32_t>(ComponentType::Hierarchical));
	}

	void TearDown() {}

	std::shared_ptr<CreateRectGeometry> create_rect;
	std::shared_ptr<ContextMock> context;
	std::shared_ptr<ProceduralObjectSystemMock> procedural_object_system;
	std::shared_ptr<GeometrySystemMock> geometry_system_mock;
	std::shared_ptr<OperationExecutionContext> exection_context;
	std::shared_ptr<Parameter<FloatParameter>> width_par;
	std::shared_ptr<Parameter<FloatParameter>> height_par;
	ProceduralObjectMask out_mask;

	void SetExecutionContext()
	{
		// clang-format off
        EXPECT_CALL(*context, GetParameter("width"))
            .Times(1)
            .WillOnce(::testing::Return(width_par));
        EXPECT_CALL(*context, GetParameter("height"))
            .Times(1)
            .WillOnce(::testing::Return(height_par));
		// clang-format on

		create_rect->SetExecutionContext(exection_context);
	}
};
