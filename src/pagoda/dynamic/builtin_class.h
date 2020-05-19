#pragma once

#include "class_base.h"
#include "dynamic_value_base.h"

namespace pagoda::dynamic
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

class BuiltinClass : public DynamicValueBase, public ClassBase
{
public:
	BuiltinClass(const TypeInfoPtr &typeInfo);

	FunctionPtr Bind(std::shared_ptr<ICallableBody> callable, std::shared_ptr<DynamicValueTable> globals = nullptr);
};
}  // namespace pagoda::dynamic
