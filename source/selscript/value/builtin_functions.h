#pragma once

#include <memory>
#include <vector>

namespace sscript
{
struct BaseValue;
using BaseValuePtr = std::shared_ptr<BaseValue>;
struct Float;
using FloatPtr = std::shared_ptr<Float>;
struct Integer;
using IntegerPtr = std::shared_ptr<Integer>;
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

void print(const std::vector<BaseValuePtr>& args);
IntegerPtr time();

TypeInfoPtr type(const BaseValuePtr& value);
}  // namespace sscript
