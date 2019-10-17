#include "symbol_table.h"

#include "../value/base_value.h"

#include <iostream>

namespace sscript
{
SymbolTable::SymbolTable(const std::string& tableName) : m_symbolTableName(tableName) {}

SymbolTable::SymbolTable(const std::string& tableName, const std::shared_ptr<SymbolTable>& parentScope)
    : m_parentScope(parentScope), m_symbolTableName(tableName)
{
}

void SymbolTable::Declare(const SymbolEntry&& entry) { m_symbols[entry.m_symbolName] = entry; }

void SymbolTable::Assign(const std::string& name, const BaseValuePtr& v)
{
	auto& symbol = findSymbol(name);
	symbol.m_value = v;
}

SymbolTable::SymbolEntry& SymbolTable::Get(const std::string& name) { return findSymbol(name); }

SymbolTable::SymbolEntry& SymbolTable::findSymbol(const std::string& name)
{
	auto iter = m_symbols.find(name);
	if (iter != std::end(m_symbols))
	{
		return iter->second;
	}
	if (!m_parentScope.expired())
	{
		return m_parentScope.lock()->findSymbol(name);
	}
	throw SymbolNotFoundException(name);
}

void SymbolTable::DumpSymbols() const
{
	std::cout << "Symbols for " << m_symbolTableName << ":" << std::endl;
	for (auto symbol : m_symbols)
	{
		std::cout << symbol.first << ": " << symbol.second.m_value->ToString() << std::endl;
	}

	if (!m_parentScope.expired())
	{
		m_parentScope.lock()->DumpSymbols();
	}
}

}  // namespace sscript
