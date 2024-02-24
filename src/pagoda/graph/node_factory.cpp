#include "pagoda/graph/node_factory.h"

namespace pagoda::graph
{
NodeFactory::NodeFactory() : pagoda::common::PluggableFactory<Node>("Node") {}

NodeFactory::~NodeFactory() {}
}  // namespace pagoda::graph
