#pragma once

#include <memory>
#include <vector>

namespace pagoda
{
namespace dynamic
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;
class FloatValue;
using FloatPtr = std::shared_ptr<FloatValue>;
class Integer;
using IntegerPtr = std::shared_ptr<Integer>;
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;
}  // namespace dynamic

void print(const std::vector<dynamic::DynamicValueBasePtr>& args);
dynamic::TypeInfoPtr type(const dynamic::DynamicValueBasePtr& value);
}  // namespace pagoda
