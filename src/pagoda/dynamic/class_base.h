#pragma once

#include "dynamic_value_table.h"

namespace pagoda::dynamic
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
	/**
	 * Constructs a \c ClassBase of the given \p name.
	 */
	ClassBase(const std::string &name);

	/**
	 * Declares and assigns a class member with the \p name and value \p v.
	 */
	void RegisterMember(const std::string &name, DynamicValueBasePtr v);
	/**
	 * Sets the class member \p name to the value \p v.
	 *
	 * Does not declare the member.
	 */
	void SetMember(const std::string &name, DynamicValueBasePtr v);
	/**
	 * Tries to asign the member \p name with the value \p v.
	 *
	 * If it doesn't exist, declares it.
	 */
	void RegisterOrSetMember(const std::string &name, DynamicValueBasePtr v);
	/**
	 * Returns the class member with \p name.
	 */
	DynamicValueBasePtr GetMember(const std::string &name);
	/**
	 * Returns the \c DynamicValueTable associated with this class.
	 */
	std::shared_ptr<DynamicValueTable> GetInstanceValueTable();

	/**
	 * Returns an iterator to the first class member.
	 */
	DynamicValueTable::iterator GetMembersBegin();
	/**
	 * Returns an iterator to the past-the-end class member.
	 */
	DynamicValueTable::iterator GetMembersEnd();

	/**
	 * Gets the number of class members.
	 */
	std::size_t GetMemberCount() const;

	/**
	 * Binds a \c ICallableBody to this class with the given set of \p globals.
	 */
	virtual FunctionPtr Bind(std::shared_ptr<ICallableBody> callable,
	                         std::shared_ptr<DynamicValueTable> globals = nullptr) = 0;

	protected:
	std::shared_ptr<DynamicValueTable> m_memberTable;  ///< The class members in this class
};
}  // namespace pagoda::dynamic
