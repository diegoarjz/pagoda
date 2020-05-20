#include "operation_factory.h"

namespace pagoda::objects
{
OperationFactory::OperationFactory() : Factory<ProceduralOperation>("ProceduralOperation") {}

OperationFactory::~OperationFactory() {}
}  // namespace pagoda::objects
