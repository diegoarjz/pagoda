#include "interface.h"

namespace pagoda::objects
{
Interface::Interface(const std::string& name, Type type, Arity arity) : m_name{name}, m_type{type}, m_arity{arity} {}

////////////////////////////////////////////////////////////
/// \name Getters and Setters
////////////////////////////////////////////////////////////

const std::string& Interface::GetName() const { return m_name; }

Interface::Type Interface::GetType() const { return m_type; }

Interface::Arity Interface::GetArity() const { return m_arity; }

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
		// Input interfaces with arity of one can't accept more than one input object, so they can't accept more than one
		// interface, but they can still accept the interface that's already connected.
		return (m_connectedInterfaces.size() == 0 || m_connectedInterfaces.find(i) != m_connectedInterfaces.end()) &&
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

std::size_t Interface::ConnectionCount() const { return m_connectedInterfaces.size(); }

////////////////////////////////////////////////////////////
/// \name Object Accessors
////////////////////////////////////////////////////////////
ProceduralObjectPtr Interface::Get()
{
	if (m_arity != Arity::One) {
		return nullptr;
	}
	auto obj = m_objects[0];
	m_objects[0] = nullptr;
	return obj;
}

void Interface::Set(ProceduralObjectPtr object)
{
	if (m_arity != Arity::One) {
		return;
	}
	m_objects.resize(1);
	m_objects[0] = object;
}

ProceduralObjectPtr Interface::GetNext()
{
	if (m_arity != Arity::Many) {
		return nullptr;
	}
	auto obj = m_objects[0];
	m_objects[0] = nullptr;
	return obj;
}

void Interface::SetNext(ProceduralObjectPtr object)
{
	if (m_arity != Arity::Many) {
		return;
	}
	m_objects.resize(1);
	m_objects[0] = object;
}

std::size_t Interface::GetAll(std::function<void(ProceduralObjectPtr&)> f)
{
	if (m_arity != Arity::All) {
		return 0;
	}

	for (auto& o : m_objects) {
		f(o);
	}
	return m_objects.size();
}

void Interface::Add(ProceduralObjectPtr object)
{
	if (m_arity != Arity::All) {
		return;
	}
	m_objects.push_back(object);
}
}  // namespace pagoda::objects
