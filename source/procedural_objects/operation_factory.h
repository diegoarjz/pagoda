#ifndef PAGODA_OPERATION_FACTORY_H_
#define PAGODA_OPERATION_FACTORY_H_

#include "common/factory.h"

#include "procedural_operation.h"

namespace pagoda
{
class OperationFactory : public Factory<ProceduralOperation>
{
public:
	OperationFactory();
	virtual ~OperationFactory();
};

using OperationFactoryPtr = std::shared_ptr<OperationFactory>;
}  // namespace pagoda
#endif
