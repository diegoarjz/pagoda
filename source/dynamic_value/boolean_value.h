#pragma once

#include "dynamic_value_base.h"

namespace selector
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

/**
 * Represents a dynamic boolean.
 */
class Boolean : public DynamicValueBase
{
public:
	static const TypeInfoPtr s_typeInfo;

	/**
	 * Constructs with a default value of false.
	 */
	Boolean();
	/**
	 * Constructs with the value given in \p b.
	 */
	explicit Boolean(const bool& b);
	virtual ~Boolean();

	std::string ToString() const override;

	/**
	 * Explicitly converts to boolean.
	 */
	explicit operator bool() const;

	Boolean& operator=(const bool& b);

	Boolean operator==(const Boolean& b) const;
	Boolean operator!=(const Boolean& b) const;

	Boolean operator!() const;

	void AcceptVisitor(ValueVisitorBase& visitor) override;

private:
	bool m_value;
};
using BooleanPtr = std::shared_ptr<Boolean>;
}  // namespace selector
