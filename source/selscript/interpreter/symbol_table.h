#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace sscript
{
struct BaseValue;
using BaseValuePtr = std::shared_ptr<BaseValue>;

class SymbolTable
{
public:
	struct SymbolEntry
	{
		std::string m_symbolName;
		BaseValuePtr m_value;
	};

	using iterator = std::unordered_map<std::string, SymbolEntry>::iterator;

	SymbolTable() = delete;
	explicit SymbolTable(const std::string &tableName);
	SymbolTable(const std::string &tableName, const std::shared_ptr<SymbolTable> &parentScope);
	SymbolTable(const SymbolTable &) = delete;
	SymbolTable &operator=(const SymbolTable &) = delete;

	void Declare(const SymbolEntry &&entry);
	void Assign(const std::string &name, const BaseValuePtr &v);
	SymbolEntry &Get(const std::string &name);

	std::shared_ptr<SymbolTable> GetParent() const { return m_parentScope.lock(); }
	void SetParent(const std::shared_ptr<SymbolTable> &parent) { m_parentScope = parent; }

	iterator begin() { return std::begin(m_symbols); }
	iterator end() { return std::end(m_symbols); }

	void DumpSymbols() const;

private:
	SymbolEntry &findSymbol(const std::string &name);

	std::unordered_map<std::string, SymbolEntry> m_symbols;
	std::weak_ptr<SymbolTable> m_parentScope;
	std::weak_ptr<SymbolTable> m_globals;
	std::string m_symbolTableName;
};

class SymbolNotFoundException : public std::runtime_error
{
public:
	explicit SymbolNotFoundException(const std::string &symbolName)
	    : std::runtime_error("Symbol " + symbolName + " referenced before declaration."), m_symbolName(symbolName)
	{
	}

	const std::string &GetSymbolName() const { return m_symbolName; }

private:
	std::string m_symbolName;
};

class SymbolShadowingException : public std::runtime_error
{
public:
	SymbolShadowingException(const std::string &symbolName, const SymbolTable::SymbolEntry &prevDeclaration)
	    : std::runtime_error("Symbol " + symbolName + " declaration shadows previous declaration"),
	      m_symbolName(symbolName),
	      m_previousDeclaration(prevDeclaration)
	{
	}

	const std::string &GetSymbolName() const { return m_symbolName; }
	const SymbolTable::SymbolEntry &GetPreviousDeclaration() const { return m_previousDeclaration; }

private:
	std::string m_symbolName;
	const SymbolTable::SymbolEntry m_previousDeclaration;
};
}  // namespace sscript
