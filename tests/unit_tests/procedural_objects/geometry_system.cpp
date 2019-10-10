#include "../parameter/mock_objects.h"
#include "mock_objects.h"

#include <parameter/context.h>
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
		geometry = std::make_shared<Geometry>();
	}

	void TearDown()
	{
		if (component != nullptr)
		{
			//geometry_system->KillProceduralComponent(component);
		}
	}

	std::shared_ptr<GeometrySystem> geometry_system;
	std::shared_ptr<GeometryComponent> component;
	GeometryPtr geometry;
};

TEST_F(GeometrySystemTest, test_create_component)
{
    auto proceduralObject = std::make_shared<ProceduralObject>();
	auto procedural_component = geometry_system->CreateComponent(proceduralObject);
	component = std::dynamic_pointer_cast<GeometryComponent>(procedural_component);

	EXPECT_EQ(procedural_component->GetType(), GeometryComponent::GetComponentSystemName());
	EXPECT_NE(component, nullptr);

	auto returned_component = geometry_system->GetComponent(proceduralObject);
	EXPECT_EQ(returned_component, std::dynamic_pointer_cast<GeometryComponent>(component));
}

TEST_F(GeometrySystemTest, test_kill_component)
{
    auto proceduralObject = std::make_shared<ProceduralObject>();
	auto component = geometry_system->CreateComponent(proceduralObject);

	geometry_system->KillProceduralComponent(proceduralObject);
	auto returned_component = geometry_system->GetComponent(proceduralObject);
	EXPECT_EQ(returned_component, nullptr);
}
