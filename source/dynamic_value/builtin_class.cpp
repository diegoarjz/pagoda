#include "builtin_class.h"

#include "type_info.h"

namespace selector
{
BuiltinClass::BuiltinClass(const TypeInfoPtr& typeInfo) : DynamicValueBase(typeInfo), ClassBase(typeInfo->GetTypeName())
{
}

FunctionPtr BuiltinClass::Bind(std::shared_ptr<ICallableBody> callable,
                               std::shared_ptr<DynamicValueTable> globals)
{
    throw std::runtime_error("Unimplemented");
}
}  // namespace selector
