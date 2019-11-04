#include "class_base.h"

namespace selector
{
ClassBase::ClassBase(const std::string &name) : m_memberTable(name) {}

void ClassBase::RegisterMember(const std::string &name, DynamicValueBasePtr v) { m_memberTable.Declare(name, v); }

void ClassBase::SetMember(const std::string &name, DynamicValueBasePtr v) { m_memberTable.Assign(name, v); }

DynamicValueBasePtr ClassBase::GetMember(const std::string &name) { return m_memberTable.Get(name); }

std::unordered_map<std::string, DynamicValueBasePtr> ClassBase::GetMembers()
{
	throw std::runtime_error("Unimplemented");
	return {};
}
}  // namespace selector
