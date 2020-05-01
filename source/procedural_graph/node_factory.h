#ifndef PAGODA_NODE_FACTORY_H_
#define PAGODA_NODE_FACTORY_H_

#include "common/factory.h"

#include "node.h"

namespace pagoda
{
class NodeFactory : public Factory<Node>
{
public:
	NodeFactory();
	virtual ~NodeFactory();
};

using NodeFactoryPtr = std::shared_ptr<NodeFactory>;
}  // namespace pagoda

#endif
