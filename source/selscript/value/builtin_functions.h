#pragma once

#include <memory>
#include <vector>

namespace selector
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;
class FloatValue;
using FloatPtr = std::shared_ptr<FloatValue>;
class Integer;
using IntegerPtr = std::shared_ptr<Integer>;
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

void print(const std::vector<DynamicValueBasePtr>& args);
TypeInfoPtr type(const DynamicValueBasePtr& value);
}  // namespace selector
