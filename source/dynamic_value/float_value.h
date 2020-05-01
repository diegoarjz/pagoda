#pragma once

#include "dynamic_value_base.h"

namespace pagoda
{
class Integer;
using IntegerPtr = std::shared_ptr<Integer>;

/**
 * Represents a dynamic float value.
 */
class FloatValue : public DynamicValueBase
{
public:
	static const TypeInfoPtr s_typeInfo;

	/**
	 * Constructs a \c FloatValue with a default value of 0.
	 */
	FloatValue();
	/**
	 * constructs a \c Floatvalue with the value given in \p f.
	 */
	explicit FloatValue(const float& f);
	/**
	 * constructs a \c Floatvalue with the value given in \p i.
	 */
	explicit FloatValue(const int& i);
	/**
	 * constructs a \c Floatvalue with the value given in \p i.
	 */
	explicit FloatValue(const IntegerPtr& i);
	/**
	 * constructs a \c Floatvalue with the value given in \p i.
	 */
	explicit FloatValue(const Integer& i);
	virtual ~FloatValue();

	/**
	 * Explicitly converts the \c FloatValue to a float.
	 */
	explicit operator float() const;

	/**
	 * Explicitly converts the \c FloatValue to an int.
	 */
	explicit operator int() const;

	FloatValue& operator=(const float& f);
	FloatValue& operator=(const int& f);

	std::string ToString() const override;

	FloatValue operator+(const FloatValue& f) const;
	FloatValue operator-(const FloatValue& f) const;
	FloatValue operator*(const FloatValue& f) const;
	FloatValue operator/(const FloatValue& f) const;
	FloatValue operator-() const;

	FloatValue operator+(const Integer& i) const;
	FloatValue operator-(const Integer& i) const;
	FloatValue operator*(const Integer& i) const;
	FloatValue operator/(const Integer& i) const;

	bool operator==(const FloatValue& f) const;
	bool operator!=(const FloatValue& f) const;
	bool operator<(const FloatValue& f) const;
	bool operator<=(const FloatValue& f) const;
	bool operator>(const FloatValue& f) const;
	bool operator>=(const FloatValue& f) const;

	bool operator==(const Integer& f) const;
	bool operator!=(const Integer& f) const;
	bool operator<(const Integer& f) const;
	bool operator<=(const Integer& f) const;
	bool operator>(const Integer& f) const;
	bool operator>=(const Integer& f) const;

	void AcceptVisitor(ValueVisitorBase& visitor) override;

private:
	float m_value;
};
using FloatValuePtr = std::shared_ptr<FloatValue>;
}  // namespace pagoda
