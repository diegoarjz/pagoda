#include "operation_factory.h"

namespace selector
{
OperationFactory::OperationFactory() : Factory<ProceduralOperation>("ProceduralOperation") {}

OperationFactory::~OperationFactory() {}
}  // namespace selector
