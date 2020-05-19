#pragma once

#include "dynamic_value_base.h"

#include <memory>
#include <vector>

namespace pagoda::dynamic
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;

class DynamicValueTable;

class ICallableBody
{
public:
	virtual void Call(const std::vector<DynamicValueBasePtr>& args) = 0;
	virtual void SetClosure(const std::shared_ptr<DynamicValueTable>&) = 0;
	virtual const std::shared_ptr<DynamicValueTable>& GetClosure() const = 0;
};
}  // namespace pagoda::dynamic
