#include "interface.h"

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
	if (m_arity != Arity::One) {
		DBG_ASSERT_MSG(
		  false, "Can only call Interface::Get() on an interface with Arity::One");
		LOG_TRACE(ProceduralObjects, "  Arity Mismatch");
		return nullptr;
	}
	auto obj = m_objects[0];
	m_objects[0] = nullptr;
	return obj;
}

void Interface::Set(ProceduralObjectPtr object)
{
	LOG_TRACE(ProceduralObjects, "Setting object on interface "
	                               << GetName() << " with arity "
	                               << GetArityName());
	if (m_arity != Arity::One) {
		DBG_ASSERT_MSG(
		  false, "Can only call Interface::Set() on an interface with Arity::One");
		LOG_TRACE(ProceduralObjects, "  Arity Mismatch");
		return;
	}
	m_objects.resize(1);
	m_objects[0] = object;
}

ProceduralObjectPtr Interface::GetNext()
{
	LOG_TRACE(ProceduralObjects, "Getting next object from interface "
	                               << GetName() << " with arity "
	                               << GetArityName());
	DBG_ASSERT_MSG(
	  m_arity == Arity::Many,
	  "Can only call Interface::GetNext() on an interface with Arity::Many");
	if (m_arity != Arity::Many) {
		LOG_TRACE(ProceduralObjects, "  Arity Mismatch");
		return nullptr;
	}
	auto obj = m_objects[0];
	m_objects[0] = nullptr;
	return obj;
}

void Interface::SetNext(ProceduralObjectPtr object)
{
	LOG_TRACE(ProceduralObjects, "Setting next object on interface "
	                               << GetName() << " with arity "
	                               << GetArityName());
	if (m_arity != Arity::Many) {
		DBG_ASSERT_MSG(
		  false,
		  "Can only call Interface::SetNext() on an interface with Arity::Many");
		LOG_TRACE(ProceduralObjects, "  Arity Mismatch");
		return;
	}
	m_objects.resize(1);
	m_objects[0] = object;
}

std::size_t Interface::GetAll(std::function<void(ProceduralObjectPtr&)> f)
{
	LOG_TRACE(ProceduralObjects, "Getting all objects from interface "
	                               << GetName() << " with arity "
	                               << GetArityName());
	if (m_arity != Arity::All) {
		DBG_ASSERT_MSG(
		  false,
		  "Can only call Interface::GetAll() on an interface with Arity::All");
		LOG_TRACE(ProceduralObjects, "  Arity Mismatch");
		return 0;
	}

	for (auto& o : m_objects) {
		f(o);
	}
	return m_objects.size();
}

void Interface::Add(ProceduralObjectPtr object)
{
	LOG_TRACE(ProceduralObjects, "Adding object to interface " << GetName()
	                                                           << " with arity "
	                                                           << GetArityName());
	if (m_arity != Arity::All) {
		DBG_ASSERT_MSG(
		  false, "Can only call Interface::Add() on an interface with Arity::All");
		LOG_TRACE(ProceduralObjects, "  Arity Mismatch");
		return;
	}
	m_objects.push_back(object);
}
}  // namespace pagoda::objects
