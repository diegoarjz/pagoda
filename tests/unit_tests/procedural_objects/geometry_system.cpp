#include "mock_objects.h"

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>
#include <pagoda/geometry/operations/create_rect.h>
#include <pagoda/geometry/operations/extrude_geometry.h>

#include <pagoda/objects/hierarchical_component.h>
#include <pagoda/objects/hierarchical_system.h>
#include <pagoda/objects/procedural_object.h>
#include <pagoda/pagoda.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace pagoda;

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
		if (component != nullptr) {
			// geometry_system->KillProceduralComponent(component);
		}
	}

	std::shared_ptr<GeometrySystem> geometry_system;
	std::shared_ptr<GeometryComponent> component;
	GeometryPtr geometry;
  Pagoda m_pagoda;
};

TEST_F(GeometrySystemTest, test_create_component)
{
	auto proceduralObject = std::make_shared<ProceduralObject>(m_pagoda.GetProceduralObjectSystem());
	auto procedural_component = geometry_system->CreateComponent(proceduralObject);
	component = std::dynamic_pointer_cast<GeometryComponent>(procedural_component);

	EXPECT_EQ(procedural_component->GetType(), GeometryComponent::GetComponentSystemName());
	EXPECT_NE(component, nullptr);

	auto returned_component = geometry_system->GetComponent(proceduralObject);
	EXPECT_EQ(returned_component, std::dynamic_pointer_cast<GeometryComponent>(component));
}

TEST_F(GeometrySystemTest, test_kill_component)
{
	auto proceduralObject = std::make_shared<ProceduralObject>(m_pagoda.GetProceduralObjectSystem());
	auto component = geometry_system->CreateComponent(proceduralObject);

	geometry_system->KillProceduralComponent(proceduralObject);
	auto returned_component = geometry_system->GetComponent(proceduralObject);
	EXPECT_EQ(returned_component, nullptr);
}
