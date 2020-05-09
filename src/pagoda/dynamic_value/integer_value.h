#pragma once

#include "dynamic_value_base.h"

namespace pagoda
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

class FloatValue;
using FloatValuePtr = std::shared_ptr<FloatValue>;

class String;

/**
 * Represents a dynamic integer.
 */
class Integer : public DynamicValueBase
{
public:
	static const TypeInfoPtr s_typeInfo;

	/**
	 * Constructs the \c Integer with a default value of 0.
	 */
	Integer();

	/**
	 * Constructs the \c Integer with the value given in \p i.
	 */
	explicit Integer(const int& i);
	/**
	 * Constructs the \c Integer by truncating the value given in \p f.
	 */
	explicit Integer(const float& f);
	/**
	 * Constructs the \c Integer by truncating the value given in \p f.
	 */
	explicit Integer(const FloatValuePtr& f);
	explicit Integer(const FloatValue& f);
	virtual ~Integer();

	/**
	 * Explicitly converts the \c Integer to an int.
	 */
	explicit operator int() const;
	/**
	 * Explicitly converts the \c Integer to a float.
	 */
	explicit operator float() const;

	Integer& operator=(const int& i);
	Integer& operator=(const float& f);

	std::string ToString() const override;

	Integer operator-() const;

	Integer operator+(const Integer& i) const;
	Integer operator-(const Integer& i) const;
	Integer operator*(const Integer& i) const;
	Integer operator/(const Integer& i) const;

	FloatValue operator+(const FloatValue& f) const;
	FloatValue operator-(const FloatValue& f) const;
	FloatValue operator*(const FloatValue& f) const;
	FloatValue operator/(const FloatValue& f) const;

	String operator*(const String& s) const;

	bool operator==(const Integer& i) const;
	bool operator!=(const Integer& i) const;
	bool operator<(const Integer& i) const;
	bool operator<=(const Integer& i) const;
	bool operator>(const Integer& i) const;
	bool operator>=(const Integer& i) const;

	bool operator==(const FloatValue& f) const;
	bool operator!=(const FloatValue& f) const;
	bool operator<(const FloatValue& f) const;
	bool operator<=(const FloatValue& f) const;
	bool operator>(const FloatValue& f) const;
	bool operator>=(const FloatValue& f) const;

	void AcceptVisitor(ValueVisitorBase& visitor) override;

private:
	int m_value;
};
using IntegerPtr = std::shared_ptr<Integer>;
}  // namespace pagoda
