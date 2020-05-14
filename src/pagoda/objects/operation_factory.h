#ifndef PAGODA_OPERATION_FACTORY_H_
#define PAGODA_OPERATION_FACTORY_H_

#include "procedural_operation.h"

#include <pagoda/common/factory.h>

namespace pagoda::objects
{
class OperationFactory : public common::Factory<ProceduralOperation>
{
public:
	OperationFactory();
	virtual ~OperationFactory();
};

using OperationFactoryPtr = std::shared_ptr<OperationFactory>;
}  // namespace pagoda::objects
#endif
