#include <procedural_objects/geometry_system.h>

#include <procedural_objects/geometry_component.h>
#include <procedural_objects/hierarchical_component.h>
#include <procedural_objects/hierarchical_system.h>
#include <procedural_objects/procedural_object.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(ProceduralObject, add_procedural_component)
{
	ProceduralObjectPtr o = std::make_shared<ProceduralObject>();
	std::shared_ptr<GeometryComponent> geom = std::make_shared<GeometryComponent>();
	ProceduralObjectMask expected_mask;
	expected_mask.set(static_cast<uint32_t>(GeometryComponent::GetType()));

	o->SetComponent(geom);

	EXPECT_EQ(o->GetComponent<GeometryComponent>(), geom);
	EXPECT_EQ(geom->GetParentObject(), o);
	EXPECT_EQ(o->Mask(), expected_mask);
}

TEST(ProceduralObject, remove_procedural_component)
{
	ProceduralObjectPtr o = std::make_shared<ProceduralObject>();
	std::shared_ptr<GeometryComponent> geom = std::make_shared<GeometryComponent>();
	ProceduralObjectMask expected_mask;

	o->SetComponent(geom);
	auto removed = o->RemoveComponent<GeometryComponent>();

	EXPECT_EQ(removed, geom);
	EXPECT_EQ(o->GetComponent<GeometryComponent>(), nullptr);
	EXPECT_EQ(geom->GetParentObject(), nullptr);
	EXPECT_EQ(o->Mask(), expected_mask);
}

TEST(ProceduralObject, hierarchical_component_set_parent)
{
	auto hierarchical_system = std::make_shared<HierarchicalSystem>();
	auto parent = std::dynamic_pointer_cast<HierarchicalComponent>(hierarchical_system->CreateComponent());
	auto child = std::dynamic_pointer_cast<HierarchicalComponent>(hierarchical_system->CreateComponent());

	hierarchical_system->SetParent(parent, child);
	EXPECT_EQ(child->GetParent(), parent);
	EXPECT_EQ(parent->ChildrenCount(), 1);
	EXPECT_EQ(parent->cbegin()->lock(), child);

	hierarchical_system->SetParent(nullptr, child);
	EXPECT_EQ(child->GetParent(), nullptr);
	EXPECT_EQ(parent->ChildrenCount(), 0);
	EXPECT_EQ(parent->cbegin(), parent->cend());
}

TEST(ProceduralObject, test_mask_set_and_unset)
{
	ProceduralObjectPtr o = std::make_shared<ProceduralObject>();
	std::shared_ptr<GeometryComponent> geom = std::make_shared<GeometryComponent>();

	ProceduralObjectMask expected_mask;

	o->SetComponent(geom);
	expected_mask.set(static_cast<uint32_t>(GeometryComponent::GetType()), true);
	EXPECT_EQ(o->Mask(), expected_mask);

	o->RemoveComponent<GeometryComponent>();
	expected_mask.set(static_cast<uint32_t>(GeometryComponent::GetType()), false);
	EXPECT_EQ(o->Mask(), expected_mask);
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
