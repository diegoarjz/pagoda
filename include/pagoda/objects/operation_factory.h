#ifndef PAGODA_OPERATION_FACTORY_H_
#define PAGODA_OPERATION_FACTORY_H_

#include <pagoda/common/factory.h>

namespace pagoda::objects
{
class OperationFactory : public common::Factory<class ProceduralOperation>
{
	public:
	OperationFactory();
	virtual ~OperationFactory();
};

using OperationFactoryPtr = std::shared_ptr<OperationFactory>;
}  // namespace pagoda::objects
#endif
