#pragma once

#include "node.h"

#include <list>

namespace selector
{
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;

class RouterNode : public Node
{
public:
	static const char *name;

	RouterNode();
	virtual ~RouterNode();

	void SetConstructionArguments(const std::unordered_map<std::string, DynamicValueBasePtr> &) override;
	void AcceptNodeVisitor(NodeVisitor *visitor) override;
	void AddProceduralObject(ProceduralObjectPtr object);

	void Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes) override;

private:
	std::list<ProceduralObjectPtr> m_proceduralObjects;
};
}  // namespace selector
