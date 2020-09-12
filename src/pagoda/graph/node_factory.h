#ifndef PAGODA_NODE_FACTORY_H_
#define PAGODA_NODE_FACTORY_H_

#include "pagoda/common/factory.h"

#include "node.h"

namespace pagoda::graph
{
class NodeFactory : public common::Factory<Node>
{
	public:
	NodeFactory();
	virtual ~NodeFactory();
};

using NodeFactoryPtr = std::shared_ptr<NodeFactory>;
}  // namespace pagoda::graph

#endif
