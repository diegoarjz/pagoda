#include "builtin_class.h"

#include "type_info.h"

#include "common/exception.h"

namespace selector
{
BuiltinClass::BuiltinClass(const TypeInfoPtr& typeInfo) : DynamicValueBase(typeInfo), ClassBase(typeInfo->GetTypeName())
{
}

FunctionPtr BuiltinClass::Bind(std::shared_ptr<ICallableBody> callable, std::shared_ptr<DynamicValueTable> globals)
{
	throw Exception("Unimplemented");
}
}  // namespace selector
