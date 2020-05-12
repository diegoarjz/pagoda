#include <pagoda/procedural_objects/geometry_system.h>

#include <pagoda/procedural_objects/geometry_component.h>
#include <pagoda/procedural_objects/hierarchical_component.h>
#include <pagoda/procedural_objects/hierarchical_system.h>
#include <pagoda/procedural_objects/procedural_object.h>
#include <pagoda/procedural_objects/procedural_object_system.h>
#include <pagoda/procedural_objects/scope_axis_direction_predicate.h>

#include <gtest/gtest.h>

#include <boost/qvm/map_vec_mat.hpp>

using namespace pagoda;

class ProceduralObjectPredicateTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_objectSystem = std::make_shared<ProceduralObjectSystem>();
		m_geometrySystem = std::make_shared<GeometrySystem>();
		m_objectSystem->RegisterProceduralComponentSystem(m_geometrySystem);
		m_object = m_objectSystem->CreateProceduralObject();
	}

	void TearDown() {}

	ProceduralObjectSystemPtr m_objectSystem;
	GeometrySystemPtr m_geometrySystem;
	ProceduralObjectPtr m_object;
};

TEST_F(ProceduralObjectPredicateTest,
       when_testing_scope_axis_direction_should_evaluate_whether_the_axis_is_facing_the_direction)
{
	auto component = m_geometrySystem->CreateComponentAs<GeometryComponent>(m_object);
	component->SetScope(Scope({0, 0, 0}, {1, 1, 1}, boost::qvm::diag_mat(XYZ(Vec3F{1, 1, 1}))));

	{
		ScopeAxisDirectionPredicate pred(m_objectSystem, 'x', {1, 0, 0});
		EXPECT_TRUE(pred(m_object));
	}

	{
		ScopeAxisDirectionPredicate pred(m_objectSystem, 'y', {1, 0, 0});
		EXPECT_FALSE(pred(m_object));
	}

	{
		ScopeAxisDirectionPredicate pred(m_objectSystem, 'x', {1, 1, 0}, Degrees<float>(45.0f));
		EXPECT_TRUE(pred(m_object));
	}

	{
		ScopeAxisDirectionPredicate pred(m_objectSystem, 'x', {1, 1, 0}, Degrees<float>(44.0f));
		EXPECT_FALSE(pred(m_object));
	}
}
