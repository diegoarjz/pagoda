#include "dynamic_value_table.h"

#include "dynamic_value_base.h"
#include "value_not_found.h"

#include <ostream>

namespace pagoda::dynamic
{
DynamicValueTable::DynamicValueTable(const std::string &tableName) : m_tableName(tableName) {}

DynamicValueTable::DynamicValueTable(const std::string &tableName,
                                     const std::shared_ptr<DynamicValueTable> &parentTable)
  : m_parentTable(parentTable), m_tableName(tableName)
{
}

void DynamicValueTable::Declare(const std::string &name, DynamicValueBasePtr value) { m_values[name] = {value}; }

void DynamicValueTable::Assign(const std::string &name, DynamicValueBasePtr value)
{
	auto &storedValue = FindValue(name);
	storedValue.m_value = value;
}

DynamicValueBasePtr DynamicValueTable::Get(const std::string &name) { return FindValue(name).m_value; }

std::shared_ptr<DynamicValueTable> DynamicValueTable::GetParent() const { return m_parentTable.lock(); }

void DynamicValueTable::SetParent(const std::shared_ptr<DynamicValueTable> &parent) { m_parentTable = parent; }

using iterator = std::unordered_map<std::string, DynamicValueBasePtr>::iterator;
typename DynamicValueTable::iterator DynamicValueTable::begin() { return std::begin(m_values); }
typename DynamicValueTable::iterator DynamicValueTable::end() { return std::end(m_values); }

std::size_t DynamicValueTable::GetMemberCount() const { return m_values.size(); }

void DynamicValueTable::DumpSymbols(std::ostream &out) const
{
	out << "Symbols for " << m_tableName << ":" << std::endl;
	for (auto value : m_values) {
		out << value.first << ": " << value.second.m_value->ToString() << std::endl;
	}

	if (!m_parentTable.expired()) {
		m_parentTable.lock()->DumpSymbols(out);
	}
}

typename DynamicValueTable::Entry &DynamicValueTable::FindValue(const std::string &name)
{
	auto iter = m_values.find(name);
	if (iter != std::end(m_values)) {
		return iter->second;
	}
	if (!m_parentTable.expired()) {
		return m_parentTable.lock()->FindValue(name);
	}
	throw ValueNotFoundException(name);
}
}  // namespace pagoda::dynamic
