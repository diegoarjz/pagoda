#pragma once

#include <memory>
#include <string>

namespace pagoda
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

class ValueVisitorBase;

/**
 * Represents a base class that can be used as parameters and as instances in pgscript.
 */
class DynamicValueBase : public std::enable_shared_from_this<DynamicValueBase>
{
public:
	static const TypeInfoPtr s_typeInfo;

	/**
	 * Constructs the \c DynamicValueBase with a pointer to a \c TypeInfo.
	 */
	DynamicValueBase(TypeInfoPtr typeInfo);
	virtual ~DynamicValueBase();

	/**
	 * Returns the \c TypeInfo.
	 */
	TypeInfoPtr GetTypeInfo() const;

	/**
	 * All subclasses must be able to convert to a string.
	 */
	virtual std::string ToString() const = 0;

	virtual void AcceptVisitor(ValueVisitorBase &visitor) = 0;

private:
	/// This object's type
	TypeInfoPtr m_typeInfo;
};

using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;
}  // namespace pagoda
