#pragma once

#include <pgeditor/common/manager.h>

#include <entt/entt.hpp>

#include <functional>
#include <memory>

namespace pgeditor::event
{
class Event;

/**
 * Main GUI event manager.
 *
 * Senders push or enqueue events and the \c EventManager
 * passes the event to the registered targets.
 *
 * Pushing an event propagates the event immediately to the
 * targets whereas queued events are only propagated when calling
 * Update.
 */
class EventManager : public pgeditor::common::Manager
{
	public:
	EventManager();
	~EventManager() override;

	bool Init() override;
	void Update(double dT) override;
	void Destroy() override;

	template<class E, auto H, class T>
	auto Register(T& target)
	{
		return m_dispatcher.sink<E>().template connect<H>(target);
	}

	template<class E, auto H, class T>
	auto Unregister(T& target)
	{
		return m_dispatcher.sink<E>().template disconnect<H>(target);
	}

	template<class T, class... Args>
	void PushEvent(Args... args)
	{
		m_dispatcher.trigger<T>(args...);
	}

	template<class T, class... Args>
	void Enqueue(Args... args)
	{
		m_dispatcher.enqueue<T>(args...);
	}

	private:
	entt::dispatcher m_dispatcher;
};
}  // namespace pgeditor::event
