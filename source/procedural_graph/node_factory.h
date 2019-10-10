#ifndef SELECTOR_NODE_FACTORY_H_
#define SELECTOR_NODE_FACTORY_H_

#include "common/factory.h"

#include "node.h"

namespace selector
{
class NodeFactory : public Factory<Node>
{
public:
	NodeFactory();
	virtual ~NodeFactory();
};

using NodeFactoryPtr = std::shared_ptr<NodeFactory>;
}  // namespace selector

#endif
