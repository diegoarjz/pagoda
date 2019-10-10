#include <procedural_objects/geometry_component.h>
#include <procedural_objects/geometry_system.h>
#include <procedural_objects/procedural_object.h>
#include <procedural_objects/procedural_operation.h>

#include <gtest/gtest.h>

using namespace selector;

class ProceduralOperationObjectInterfaceTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		geometry_component = std::make_shared<GeometryComponent>();

		procedural_object = std::make_shared<ProceduralObject>();
	}

	virtual void TearDown() {}

	std::shared_ptr<GeometryComponent> geometry_component;
	std::shared_ptr<ProceduralObject> procedural_object;
};

TEST_F(ProceduralOperationObjectInterfaceTest, test_add_object)
{
	ProceduralOperationObjectInterface interface(InterfaceName("", 0));

	EXPECT_TRUE(interface.AddProceduralObject(procedural_object));
	EXPECT_EQ(interface.GetFrontProceduralObject(), procedural_object);
	// Test the object isn't removed
	EXPECT_EQ(interface.GetFrontProceduralObject(), procedural_object);
}

TEST_F(ProceduralOperationObjectInterfaceTest, test_add_and_pop_object)
{
	ProceduralOperationObjectInterface interface(InterfaceName("", 0));

	EXPECT_TRUE(interface.AddProceduralObject(procedural_object));
	EXPECT_EQ(interface.GetAndPopProceduralObject(), procedural_object);
	// Test the object is removed
	EXPECT_EQ(interface.GetFrontProceduralObject(), nullptr);
}
