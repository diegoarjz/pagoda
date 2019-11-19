#include "dynamic_plane.h"

#include "float_value.h"
#include "type_info.h"
#include "value_visitor.h"
#include "vector3.h"

#include "binding/make_member_function.h"
#include "member_function_callable_body.h"
#include "register_member_function.h"

namespace selector
{
const TypeInfoPtr DynamicPlane::s_typeInfo = std::make_shared<TypeInfo>("Plane");

DynamicPlane::DynamicPlane() : BuiltinClass(s_typeInfo) { RegisterMembers(); }

DynamicPlane::DynamicPlane(const Plane<float>& plane) : BuiltinClass(s_typeInfo), m_nativePlane(plane)
{
	RegisterMembers();
}

DynamicPlane::DynamicPlane(Vector3Ptr point, Vector3Ptr normal)
    : BuiltinClass(s_typeInfo),
      m_nativePlane(Plane<float>::FromPointAndNormal(static_cast<Vec3F>(*point), static_cast<Vec3F>(*normal)))
{
	RegisterMembers();
}

DynamicPlane::~DynamicPlane() {}

DynamicPlane::operator Plane<float>() const { return m_nativePlane; }

DynamicPlane& DynamicPlane::operator=(const Plane<float>& p)
{
	m_nativePlane = p;
	return *this;
}

void DynamicPlane::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }

bool DynamicPlane::operator==(const DynamicPlane& p) const { return m_nativePlane == p.m_nativePlane; }
bool DynamicPlane::operator!=(const DynamicPlane& p) const { return m_nativePlane != p.m_nativePlane; }

std::string DynamicPlane::ToString() const { return to_string(m_nativePlane); }

Vector3Ptr DynamicPlane::GetPoint() { return std::make_shared<Vector3>(m_nativePlane.GetPoint()); }

Vector3Ptr DynamicPlane::GetPoint2() { return std::make_shared<Vector3>(m_nativePlane.GetPoint2()); }

Vector3Ptr DynamicPlane::GetNormal() { return std::make_shared<Vector3>(m_nativePlane.GetNormal()); }

Vector3Ptr DynamicPlane::GetVector() { return std::make_shared<Vector3>(m_nativePlane.GetVector()); }

Vector3Ptr DynamicPlane::GetVector2() { return std::make_shared<Vector3>(m_nativePlane.GetVector2()); }

FloatValuePtr DynamicPlane::GetDistanceToOrigin()
{
	return std::make_shared<FloatValue>(m_nativePlane.GetDistanceToOrigin());
}

void DynamicPlane::RegisterMembers()
{
	RegisterMemberFunction(this, "GetPoint", make_member_function(&DynamicPlane::GetPoint));
	RegisterMemberFunction(this, "GetPoint2", make_member_function(&DynamicPlane::GetPoint2));
	RegisterMemberFunction(this, "GetNormal", make_member_function(&DynamicPlane::GetNormal));
	RegisterMemberFunction(this, "GetVector", make_member_function(&DynamicPlane::GetVector));
	RegisterMemberFunction(this, "GetVector2", make_member_function(&DynamicPlane::GetVector2));
	RegisterMemberFunction(this, "GetDistanceToOrigin", make_member_function(&DynamicPlane::GetDistanceToOrigin));
}
}  // namespace selector

