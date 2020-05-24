#pragma once

#include "builtin_class.h"
#include "dynamic_value_base.h"

#include <pagoda/math/vec_base.h>

namespace pagoda::dynamic
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
	explicit Vector3(const boost::qvm::vec<float, 3>& v);
	explicit Vector3(const Vector3& v);
	virtual ~Vector3();

	explicit operator boost::qvm::vec<float, 3>() const;

	Vector3& operator=(const boost::qvm::vec<float, 3>& v);

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

	boost::qvm::vec<float, 3> m_nativeVector;
};

}  // namespace pagoda::dynamic
