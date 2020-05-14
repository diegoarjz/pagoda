#pragma once

#include "node.h"

#include <list>

namespace pagoda
{
namespace objects
{
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;
class ProceduralObjectPredicateRegistry;
using ProceduralObjectPredicateRegistryPtr = std::shared_ptr<ProceduralObjectPredicateRegistry>;
}  // namespace objects

class RouterNode : public Node
{
public:
	static const char *name;

	RouterNode(objects::ProceduralObjectPredicateRegistryPtr predicateRegistry);
	virtual ~RouterNode();

	void SetConstructionArguments(const std::unordered_map<std::string, DynamicValueBasePtr> &) override;
	void AcceptNodeVisitor(NodeVisitor *visitor) override;
	void AddProceduralObject(objects::ProceduralObjectPtr object);

	void Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes) override;

private:
	std::list<objects::ProceduralObjectPtr> m_proceduralObjects;
	objects::ProceduralObjectPredicateRegistryPtr m_predicateRegistry;
};
}  // namespace pagoda
