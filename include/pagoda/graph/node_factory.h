#ifndef PAGODA_NODE_FACTORY_H_
#define PAGODA_NODE_FACTORY_H_

#include "pagoda/common/pluggable_factory.h"

#include "pagoda/graph/node.h"

namespace pagoda::graph
{
class NodeFactory : public common::PluggableFactory<Node>
{
	public:
	NodeFactory();
	virtual ~NodeFactory();
};

using NodeFactoryPtr = std::shared_ptr<NodeFactory>;
}  // namespace pagoda::graph

#endif
