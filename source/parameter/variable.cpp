#include "variable.h"

namespace selector
{
Variable::Variable(const std::string &compoundVariable)
{
	std::size_t curr = compoundVariable.find('.');
	std::size_t prev = 0;
	while (curr != std::string::npos)
	{
		m_identifiers.push_back(compoundVariable.substr(prev, curr - prev));
		prev = curr + 1;
		curr = compoundVariable.find('.', prev);
	}
	m_identifiers.push_back(compoundVariable.substr(prev, curr - prev));
}

Variable::Variable(const std::list<std::string> &identifiers) : m_identifiers(identifiers) {}

Variable::~Variable() {}

const std::list<std::string> &Variable::GetIdentifiers() const { return m_identifiers; }

const std::string Variable::ToString() const
{
	auto iter = m_identifiers.begin();
	std::string compound = *iter;
	++iter;
	for (/**/; iter != m_identifiers.end(); ++iter)
	{
		compound += '.' + (*iter);
	}

	return compound;
}

void Variable::AddIdentifier(const std::string &i) { m_identifiers.push_back(i); }

bool Variable::IsCompound() const { return m_identifiers.size() > 1; }

bool Variable::operator==(const Variable &v) const { return m_identifiers == v.m_identifiers; }

bool Variable::operator!=(const Variable &v) const { return !(*this == v); }

bool Variable::operator==(const std::string &v) const { return ToString() == v; }

bool Variable::operator!=(const std::string &v) const { return !(*this == v); }

}  // namespace selector
