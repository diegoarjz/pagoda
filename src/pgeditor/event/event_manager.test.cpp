#include <pgeditor/event/event.h>
#include <pgeditor/event/event_manager.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace pgeditor;
using namespace pgeditor::event;

class EventManagerTest : public ::testing::Test
{
	protected:
	void SetUp() override
	{
		//
	}

	void TearDown() override
	{
		//
	}

	EventManager m_eventManager;
};

class MockEvent : public Event
{
	public:
	~MockEvent() override {}

	const std::string& GetName() const override
	{
		static const std::string sName{"MockEvent"};
		return sName;
	}
};
class MockTarget
{
	public:
	void HandleEvent(const MockEvent& e) { m_count += 1; }
	uint32_t m_count = 0u;
};

TEST_F(EventManagerTest, test_register_unregister)
{
	MockTarget mockTarget;
	m_eventManager.Register<MockEvent, &MockTarget::HandleEvent>(mockTarget);
	m_eventManager.PushEvent<MockEvent>();
	EXPECT_EQ(mockTarget.m_count, 1u);
	m_eventManager.Unregister<MockEvent, &MockTarget::HandleEvent>(mockTarget);
	m_eventManager.PushEvent<MockEvent>();
	EXPECT_EQ(mockTarget.m_count, 1u);
}

TEST_F(EventManagerTest, test_push_event)
{
	MockTarget mockTarget;
	m_eventManager.Register<MockEvent, &MockTarget::HandleEvent>(mockTarget);
	m_eventManager.PushEvent<MockEvent>();
	m_eventManager.PushEvent<MockEvent>();
	m_eventManager.PushEvent<MockEvent>();
	m_eventManager.PushEvent<MockEvent>();
	EXPECT_EQ(mockTarget.m_count, 4u);
}

TEST_F(EventManagerTest, test_several_targets)
{
	MockTarget mockTarget;
	MockTarget mockTarget2;
	m_eventManager.Register<MockEvent, &MockTarget::HandleEvent>(mockTarget);
	m_eventManager.Register<MockEvent, &MockTarget::HandleEvent>(mockTarget2);
	m_eventManager.PushEvent<MockEvent>();
	EXPECT_EQ(mockTarget.m_count, 1u);
	EXPECT_EQ(mockTarget2.m_count, 1u);
}

TEST_F(EventManagerTest, test_queue_event)
{
	MockTarget mockTarget;
	m_eventManager.Register<MockEvent, &MockTarget::HandleEvent>(mockTarget);
	m_eventManager.Enqueue<MockEvent>();
	EXPECT_EQ(mockTarget.m_count, 0u);
	m_eventManager.Update(0);
	EXPECT_EQ(mockTarget.m_count, 1u);
}
