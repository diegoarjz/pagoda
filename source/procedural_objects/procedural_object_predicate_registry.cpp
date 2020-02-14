#include "procedural_object_predicate_registry.h"

#include "procedural_object_predicate.h"

namespace selector
{
void ProceduralObjectPredicateRegistry::Register(const std::string &name, ProceduralObjectPredicatePtr predicate)
{
	m_registry.emplace(name, predicate);
}

ProceduralObjectPredicatePtr ProceduralObjectPredicateRegistry::Get(const std::string &name)
{
	auto iter = m_registry.find(name);
	if (iter == m_registry.end())
	{
		return nullptr;
	}
	return iter->second;
}
}  // namespace selector
