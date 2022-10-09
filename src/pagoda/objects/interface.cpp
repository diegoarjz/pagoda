#include "pagoda/objects/interface.h"

#include "pagoda/objects/procedural_object.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>

namespace pagoda::objects
{
Interface::Interface(const std::string& name, Type type, Arity arity)
  : m_name{name}, m_type{type}, m_arity{arity}
{
}

////////////////////////////////////////////////////////////
/// \name Getters and Setters
////////////////////////////////////////////////////////////

const std::string& Interface::GetName() const
{
	return m_name;
}

Interface::Type Interface::GetType() const
{
	return m_type;
}

Interface::Arity Interface::GetArity() const
{
	return m_arity;
}

const std::string& Interface::GetArityName() const
{
  static const std::string sInvalid{"Invalid"};
	static const std::string sOne{"One"};
	static const std::string sMany{"Many"};
	static const std::string sAll{"All"};

	switch (m_arity) {
		case Arity::One:
			return sOne;
		case Arity::Many:
			return sMany;
		case Arity::All:
			return sAll;
	};
  return sInvalid;
}

////////////////////////////////////////////////////////////
/// \name Connection Management
////////////////////////////////////////////////////////////
bool Interface::Accepts(const InterfaceWeakPtr& i) const
{
	auto interfacePtr = i.lock();
	if (m_type == interfacePtr->m_type) {
		return false;
	}
	if (m_type == Type::Input && m_arity == Arity::One) {
		// Input interfaces with arity of one can't accept more than one input
		// object, so they can't accept more than one interface, but they can still
		// accept the interface that's already connected.
		return (m_connectedInterfaces.size() == 0 ||
		        m_connectedInterfaces.find(i) != m_connectedInterfaces.end()) &&
		       static_cast<int>(interfacePtr->m_arity) == static_cast<int>(m_arity);
	}
	return true;
}

bool Interface::Connect(const InterfaceWeakPtr& interface)
{
	auto interfacePtr = interface.lock();
	if (!Accepts(interfacePtr) || !interfacePtr->Accepts(shared_from_this())) {
		return false;
	}
	m_connectedInterfaces.insert(interface);
	interfacePtr->m_connectedInterfaces.insert(shared_from_this());
	return true;
}

void Interface::Disconnect(const InterfaceWeakPtr& interface)
{
	auto interfacePtr = interface.lock();
	m_connectedInterfaces.erase(interface);
	interfacePtr->m_connectedInterfaces.erase(shared_from_this());
}

void Interface::ConnectedInterfaces(std::function<void(InterfacePtr)> f) const
{
	for (auto& i : m_connectedInterfaces) {
		f(i.lock());
	}
}

std::size_t Interface::ConnectionCount() const
{
	return m_connectedInterfaces.size();
}

////////////////////////////////////////////////////////////
/// \name Object Accessors
////////////////////////////////////////////////////////////
ProceduralObjectPtr Interface::Get()
{
	LOG_TRACE(ProceduralObjects, "Getting object from interface "
	                               << GetName() << " with arity "
	                               << GetArityName());
  if (m_objects.empty()) {
    return nullptr;
  }
	auto obj = m_objects.front();
  m_objects.pop_front();
	return obj;
}

void Interface::Set(ProceduralObjectPtr object)
{
  Add(object);
}

ProceduralObjectPtr Interface::GetNext()
{
  return Get();
}

void Interface::SetNext(ProceduralObjectPtr object)
{
  Add(object);
}

std::size_t Interface::GetAll(std::function<void(ProceduralObjectPtr&)> f)
{
	LOG_TRACE(ProceduralObjects, "Getting all objects from interface "
	                               << GetName() << " with arity "
	                               << GetArityName());

	for (auto& o : m_objects) {
		f(o);
	}
	std::size_t size = m_objects.size();
	m_objects.clear();
	return size;
}

void Interface::Add(ProceduralObjectPtr object)
{
	LOG_TRACE(ProceduralObjects, "Adding object to interface " << GetName()
	                                                           << " with arity "
	                                                           << GetArityName());
  for (auto& i : m_connectedInterfaces) {
    i.lock()->m_objects.push_back(object->Clone());
  }
}

const ProceduralObjectPtr Interface::PeekObject() const
{
	if (m_objects.empty()) {
		return nullptr;
	}
	return m_objects.front();
}
}  // namespace pagoda::objects
