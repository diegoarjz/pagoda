#pragma once

#include "dynamic_value_base.h"

namespace pagoda
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;
class Integer;

/**
 * Represents a dynamic string value.
 */
class String : public DynamicValueBase
{
public:
	static const TypeInfoPtr s_typeInfo;

	/**
	 * Constructs an empty string.
	 */
	String();
	/**
	 * Constructs the string given in \p s.
	 */
	explicit String(const std::string& s);
	virtual ~String();

	std::string ToString() const override;

	/**
	 * Explicitly converts to std::string.
	 */
	explicit operator std::string() const;

	String& operator=(const std::string& s);

	String operator+(const String& s) const;
	String operator*(const Integer& i) const;
	String operator+(const Integer& i) const;

	bool operator==(const String& s) const;
	bool operator!=(const String& s) const;

	void AcceptVisitor(ValueVisitorBase& visitor) override;

private:
	std::string m_value;
};
using StringPtr = std::shared_ptr<String>;
}  // namespace pagoda
