#include "pagoda/objects/operation_factory.h"

namespace pagoda::objects
{
OperationFactory::OperationFactory() : PluggableFactory<ProceduralOperation>("ProceduralOperation") {}

OperationFactory::~OperationFactory() {}
}  // namespace pagoda::objects
