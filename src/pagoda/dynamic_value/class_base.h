#pragma once

#include "dynamic_value_table.h"

namespace pagoda
{
class ICallableBody;

class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;

class Function;
using FunctionPtr = std::shared_ptr<Function>;

/**
 * Implements common functionality for classes.
 */
class ClassBase
{
public:
	ClassBase(const std::string &name);

	void RegisterMember(const std::string &name, DynamicValueBasePtr v);
	void SetMember(const std::string &name, DynamicValueBasePtr v);
	void RegisterOrSetMember(const std::string &name, DynamicValueBasePtr v);
	DynamicValueBasePtr GetMember(const std::string &name);
	std::shared_ptr<DynamicValueTable> GetInstanceValueTable();

	DynamicValueTable::iterator GetMembersBegin();
	DynamicValueTable::iterator GetMembersEnd();

	virtual FunctionPtr Bind(std::shared_ptr<ICallableBody> callable,
	                         std::shared_ptr<DynamicValueTable> globals = nullptr) = 0;

protected:
	std::shared_ptr<DynamicValueTable> m_memberTable;
};
}  // namespace pagoda
