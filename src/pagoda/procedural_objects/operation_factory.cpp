#include "operation_factory.h"

namespace pagoda
{
OperationFactory::OperationFactory() : Factory<ProceduralOperation>("ProceduralOperation") {}

OperationFactory::~OperationFactory() {}
}  // namespace pagoda
