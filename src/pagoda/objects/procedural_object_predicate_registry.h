#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace pagoda::objects
{
class ProceduralObjectPredicate;
using ProceduralObjectPredicatePtr = std::shared_ptr<ProceduralObjectPredicate>;

/**
 * Maintains a registry of most commonly used \c ProceduralObjectPredicate.
 */
class ProceduralObjectPredicateRegistry
{
	public:
	void Register(const std::string &name, ProceduralObjectPredicatePtr predicate);
	ProceduralObjectPredicatePtr Get(const std::string &name);

	private:
	std::unordered_map<std::string, ProceduralObjectPredicatePtr> m_registry;
};

using ProceduralObjectPredicateRegistryPtr = std::shared_ptr<ProceduralObjectPredicateRegistry>;
}  // namespace pagoda::objects
