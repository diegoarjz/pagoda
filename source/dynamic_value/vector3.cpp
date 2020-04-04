#include "vector3.h"

#include "float_value.h"
#include "get_value_as.h"
#include "integer_value.h"
#include "type_info.h"
#include "value_visitor.h"

#include "binding/make_member_function.h"
#include "member_function_callable_body.h"
#include "register_member_function.h"

namespace selector
{
const TypeInfoPtr Vector3::s_typeInfo = std::make_shared<TypeInfo>("Vector3");

std::shared_ptr<Vector3> Vector3::DynamicConstructor(const std::vector<DynamicValueBasePtr>& args)
{
	return std::make_shared<Vector3>(
	    Vec3F{get_value_as<float>(*args[0]), get_value_as<float>(*args[1]), get_value_as<float>(*args[2])});
}

Vector3::Vector3() : BuiltinClass(s_typeInfo), m_nativeVector{0, 0, 0} { RegisterMembers(); }

Vector3::Vector3(const Vec3F& v) : BuiltinClass(s_typeInfo), m_nativeVector(v) { RegisterMembers(); }

Vector3::Vector3(const Vector3& v) : BuiltinClass(s_typeInfo), m_nativeVector(v.m_nativeVector) { RegisterMembers(); }

Vector3::~Vector3() {}

Vector3::operator Vec3F() const { return m_nativeVector; }

Vector3& Vector3::operator=(const Vec3F& v)
{
	m_nativeVector = v;
	return *this;
}

void Vector3::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }

std::string Vector3::ToString() const { return to_string(m_nativeVector); }

Vector3 Vector3::operator-() const { return Vector3(-1 * m_nativeVector); }

Vector3 Vector3::operator+(const Vector3& v) const { return Vector3(m_nativeVector + v.m_nativeVector); }
Vector3 Vector3::operator-(const Vector3& v) const { return Vector3(m_nativeVector - v.m_nativeVector); }

Vector3 Vector3::operator*(const FloatValue& f) const { return Vector3(m_nativeVector * static_cast<float>(f)); }
Vector3 Vector3::operator*(const Integer& i) const { return Vector3(m_nativeVector * static_cast<int>(i)); }
Vector3 Vector3::operator/(const FloatValue& f) const { return Vector3(m_nativeVector / static_cast<float>(f)); }
Vector3 Vector3::operator/(const Integer& i) const { return Vector3(m_nativeVector / static_cast<float>(i)); }

bool Vector3::operator==(const Vector3& v) const { return (m_nativeVector == v.m_nativeVector); }
bool Vector3::operator!=(const Vector3& v) const { return (m_nativeVector != v.m_nativeVector); }

FloatValuePtr Vector3::GetX() { return std::make_shared<FloatValue>(m_nativeVector.a[0]); }
FloatValuePtr Vector3::GetY() { return std::make_shared<FloatValue>(m_nativeVector.a[1]); }
FloatValuePtr Vector3::GetZ() { return std::make_shared<FloatValue>(m_nativeVector.a[2]); }

void Vector3::RegisterMembers()
{
	RegisterMemberFunction(this, "GetX", make_member_function(&Vector3::GetX));
	RegisterMemberFunction(this, "GetY", make_member_function(&Vector3::GetY));
	RegisterMemberFunction(this, "GetZ", make_member_function(&Vector3::GetZ));
}
}  // namespace selector
