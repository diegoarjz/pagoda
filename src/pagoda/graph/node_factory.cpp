#include "pagoda/graph/node_factory.h"

namespace pagoda::graph
{
NodeFactory::NodeFactory() : Factory<Node>("Node") {}

NodeFactory::~NodeFactory() {}
}  // namespace pagoda::graph
