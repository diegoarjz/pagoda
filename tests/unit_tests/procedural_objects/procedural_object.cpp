#include <pagoda/procedural_objects/geometry_system.h>

#include <pagoda/procedural_objects/geometry_component.h>
#include <pagoda/procedural_objects/hierarchical_component.h>
#include <pagoda/procedural_objects/hierarchical_system.h>
#include <pagoda/procedural_objects/procedural_object.h>
#include <pagoda/procedural_objects/procedural_object_system.h>

#include <gtest/gtest.h>

using namespace pagoda;

TEST(ProceduralObject, hierarchical_component_set_parent)
{
	auto proceduralObject = std::make_shared<ProceduralObject>();
	auto proceduralObject2 = std::make_shared<ProceduralObject>();
	auto hierarchical_system = std::make_shared<HierarchicalSystem>();
	auto parent =
	    std::dynamic_pointer_cast<HierarchicalComponent>(hierarchical_system->CreateComponent(proceduralObject));
	auto child =
	    std::dynamic_pointer_cast<HierarchicalComponent>(hierarchical_system->CreateComponent(proceduralObject2));

	hierarchical_system->SetParent(parent, child);
	EXPECT_EQ(child->GetParent(), parent);
	EXPECT_EQ(parent->ChildrenCount(), 1);
	EXPECT_EQ(parent->cbegin()->lock(), child);

	hierarchical_system->SetParent(nullptr, child);
	EXPECT_EQ(child->GetParent(), nullptr);
	EXPECT_EQ(parent->ChildrenCount(), 0);
	EXPECT_EQ(parent->cbegin(), parent->cend());
}

class ProceduralObjectSystemTest : public ::testing::Test
{
protected:
	void SetUp() { procedural_object_system = std::make_shared<ProceduralObjectSystem>(); }

	void TearDown()
	{
		if (procedural_object != nullptr)
		{
			procedural_object_system->KillProceduralObject(procedural_object);
		}
	}

	std::shared_ptr<ProceduralObjectSystem> procedural_object_system;
	std::shared_ptr<ProceduralObject> procedural_object;
};

TEST_F(ProceduralObjectSystemTest, create_procedural_object)
{
	procedural_object = procedural_object_system->CreateProceduralObject();
	auto objects = procedural_object_system->GetProceduralObjects();

	ASSERT_NE(procedural_object, nullptr);
	ASSERT_EQ(objects.size(), 1);
	ASSERT_EQ(*objects.begin(), procedural_object);
}

TEST_F(ProceduralObjectSystemTest, remove_procedural_object)
{
	auto object = procedural_object_system->CreateProceduralObject();
	procedural_object_system->KillProceduralObject(object);

	ASSERT_EQ(procedural_object_system->GetProceduralObjects().size(), 0);
}

TEST_F(ProceduralObjectSystemTest, remove_null_procedural_object)
{
	auto object = procedural_object_system->CreateProceduralObject();
	procedural_object = object;
	object = nullptr;
	procedural_object_system->KillProceduralObject(object);

	ASSERT_EQ(procedural_object_system->GetProceduralObjects().size(), 1);
}

TEST_F(ProceduralObjectSystemTest, remove_non_managed_procedural_object)
{
	procedural_object = procedural_object_system->CreateProceduralObject();
	auto object = std::make_shared<ProceduralObject>();
	procedural_object_system->KillProceduralObject(object);

	ASSERT_EQ(procedural_object_system->GetProceduralObjects().size(), 1);
}
