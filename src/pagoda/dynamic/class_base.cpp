#include "class_base.h"

#include "value_not_found.h"

namespace pagoda::dynamic
{
ClassBase::ClassBase(const std::string &name) : m_memberTable(std::make_shared<DynamicValueTable>(name)) {}

void ClassBase::RegisterMember(const std::string &name, DynamicValueBasePtr v) { m_memberTable->Declare(name, v); }

void ClassBase::SetMember(const std::string &name, DynamicValueBasePtr v) { m_memberTable->Assign(name, v); }
void ClassBase::RegisterOrSetMember(const std::string &name, DynamicValueBasePtr v)
{
	try
	{
		m_memberTable->Assign(name, v);
	}
	catch (ValueNotFoundException &)
	{
		m_memberTable->Declare(name, v);
	}
}

DynamicValueBasePtr ClassBase::GetMember(const std::string &name) { return m_memberTable->Get(name); }

DynamicValueTable::iterator ClassBase::GetMembersBegin() { return m_memberTable->begin(); }
DynamicValueTable::iterator ClassBase::GetMembersEnd() { return m_memberTable->end(); }
std::shared_ptr<DynamicValueTable> ClassBase::GetInstanceValueTable() { return m_memberTable; }
}  // namespace pagoda::dynamic
