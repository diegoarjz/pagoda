#pragma once

#include <memory>

namespace sscript
{
struct BaseValue;
using BaseValuePtr = std::shared_ptr<BaseValue>;

template<typename FromType>
struct cast_to_base_value
{
	static BaseValuePtr cast(FromType&& from) { return std::move(from); }
};

template<>
struct cast_to_base_value<int>
{
	static BaseValuePtr cast(int&& from);
};

}  // namespace sscript

