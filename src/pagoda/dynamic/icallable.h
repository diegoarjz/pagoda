#pragma once

#include <vector>

namespace pagoda::dynamic
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;
class DynamicValueTable;
using DynamicValueTablePtr = std::shared_ptr<DynamicValueTable>;

/**
 * Interface for a \c DynamicValueBase that can be called.
 */
class ICallable
{
public:
	virtual void SetCallableName(const std::string&) = 0;
	virtual const std::string& GetCallableName() const = 0;

	virtual void SetArity(const std::size_t&) = 0;
	virtual const std::size_t& GetArity() const = 0;
	virtual bool IsVariadic() const = 0;
	virtual void SetVariadic(bool) = 0;
	virtual void SetClosure(const std::shared_ptr<DynamicValueTable>&) = 0;
	virtual const std::shared_ptr<DynamicValueTable>& GetClosure() const = 0;

	virtual DynamicValueBasePtr Call(const std::vector<DynamicValueBasePtr>& args) = 0;
};
}  // namespace pagoda::dynamic
