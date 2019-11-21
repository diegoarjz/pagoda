#pragma once

#include "builtin_class.h"
#include "dynamic_value_base.h"

#include "math_lib/vec_base.h"

namespace selector
{
class FloatValue;
using FloatValuePtr = std::shared_ptr<FloatValue>;
class Integer;

class Vector3 : public BuiltinClass
{
public:
	static const TypeInfoPtr s_typeInfo;

    static std::shared_ptr<Vector3> DynamicConstructor(const std::vector<DynamicValueBasePtr>& args);

	Vector3();
	explicit Vector3(const Vec3F& v);
	explicit Vector3(const Vector3& v);
	virtual ~Vector3();

	explicit operator Vec3F() const;

	Vector3& operator=(const Vec3F& v);

	void AcceptVisitor(ValueVisitorBase& visitor) override;

	std::string ToString() const override;

	Vector3 operator-() const;

	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;

	Vector3 operator*(const FloatValue& f) const;
	Vector3 operator*(const Integer& i) const;
	Vector3 operator/(const FloatValue& f) const;
	Vector3 operator/(const Integer& i) const;

	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;

	FloatValuePtr GetX();
	FloatValuePtr GetY();
	FloatValuePtr GetZ();

private:
	void RegisterMembers();

	Vec3F m_nativeVector;
};

}  // namespace selector
