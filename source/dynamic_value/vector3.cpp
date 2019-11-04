#include "vector3.h"

#include "float_value.h"
#include "integer_value.h"
#include "math_lib/vec_arithmetic.h"
#include "type_info.h"
#include "value_visitor.h"

#include "binding/make_member_function.h"
#include "member_function_callable_body.h"

namespace selector
{
const TypeInfoPtr Vector3::s_typeInfo = std::make_shared<TypeInfo>("Vector3");

Vector3::Vector3() : DynamicValueBase(s_typeInfo), ClassBase(s_typeInfo->GetTypeName()) { RegisterMembers(); }

Vector3::Vector3(const Vec3F& v) : DynamicValueBase(s_typeInfo), ClassBase(s_typeInfo->GetTypeName()), m_nativeVector(v)
{
	RegisterMembers();
}

Vector3::Vector3(const Vector3& v)
    : DynamicValueBase(s_typeInfo), ClassBase(s_typeInfo->GetTypeName()), m_nativeVector(v.m_nativeVector)
{
	RegisterMembers();
}

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

FloatValuePtr Vector3::GetX() { return std::make_shared<FloatValue>(m_nativeVector.X()); }
FloatValuePtr Vector3::GetY() { return std::make_shared<FloatValue>(m_nativeVector.Y()); }
FloatValuePtr Vector3::GetZ() { return std::make_shared<FloatValue>(m_nativeVector.Z()); }

void Vector3::RegisterMembers()
{
	RegisterMember("GetX",
	               std::make_shared<Function>(
	                   std::make_shared<MemberFunctionCallableBody<Vector3, std::function<FloatValuePtr(Vector3*)>>>(
	                       *this, make_member_function(&Vector3::GetX))));
	RegisterMember("GetY",
	               std::make_shared<Function>(
	                   std::make_shared<MemberFunctionCallableBody<Vector3, std::function<FloatValuePtr(Vector3*)>>>(
	                       *this, make_member_function(&Vector3::GetY))));
	RegisterMember("GetZ",
	               std::make_shared<Function>(
	                   std::make_shared<MemberFunctionCallableBody<Vector3, std::function<FloatValuePtr(Vector3*)>>>(
	                       *this, make_member_function(&Vector3::GetZ))));
}
}  // namespace selector
