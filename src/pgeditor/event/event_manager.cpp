#include "event_manager.h"

#include "event.h"

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace pgeditor::event
{
EventManager::EventManager() {}
EventManager::~EventManager() {}

bool EventManager::Init() { return true; }

void EventManager::Update(double dT) { m_dispatcher.update(); }
void EventManager::Destroy() {}

}  // namespace pgeditor::event
