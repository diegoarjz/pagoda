#pragma once

#include "dynamic_value_table.h"

namespace selector
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;

/**
 * Implements common functionality for classes.
 */
class ClassBase
{
public:
	ClassBase(const std::string &name);

	void RegisterMember(const std::string &name, DynamicValueBasePtr v);
	void SetMember(const std::string &name, DynamicValueBasePtr v);
	DynamicValueBasePtr GetMember(const std::string &name);
	std::unordered_map<std::string, DynamicValueBasePtr> GetMembers();

private:
	DynamicValueTable m_memberTable;
};
}  // namespace selector
