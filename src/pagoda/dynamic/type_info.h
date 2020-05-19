#pragma once

#include "dynamic_value_base.h"

#include <string>

namespace pagoda::dynamic
{
/**
 * Represents the type information for all \c DynamicValueBase.
 * Used for run time type information.
 */
class TypeInfo : public DynamicValueBase
{
public:
	/// A \c TypeInfo is also a \c DynamicValueBase so it must have its own type.
	static const std::shared_ptr<TypeInfo> s_typeInfo;

	/**
	 * Constructs an invalid \c TypeInfo.
	 */
	TypeInfo();

	/**
	 * Constructs a \c TypeInfo with a name.
	 */
	explicit TypeInfo(const std::string& name);

	std::string GetTypeName() const;

	std::string ToString() const override;

	void AcceptVisitor(ValueVisitorBase& visitor) override;

private:
	std::string m_typeName;
};
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

}  // namespace pagoda::dynamic
