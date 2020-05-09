#include "builtin_class.h"

#include "function.h"
#include "type_info.h"

#include "common/exception.h"
#include "common/unimplemented.h"

namespace pagoda
{
BuiltinClass::BuiltinClass(const TypeInfoPtr& typeInfo) : DynamicValueBase(typeInfo), ClassBase(typeInfo->GetTypeName())
{
}

FunctionPtr BuiltinClass::Bind(std::shared_ptr<ICallableBody> callable, std::shared_ptr<DynamicValueTable> globals)
{
	auto boundMethod = std::make_shared<Function>(callable);
	boundMethod->SetVariadic(true);
	return boundMethod;
}
}  // namespace pagoda
