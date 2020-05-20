#pragma once

#include "builtin_class.h"
#include "dynamic_value_base.h"

#include <pagoda/math/plane.h>
#include <pagoda/math/vec_base.h>

namespace pagoda::dynamic
{
class FloatValue;
using FloatValuePtr = std::shared_ptr<FloatValue>;
class Vector3;
using Vector3Ptr = std::shared_ptr<Vector3>;

class DynamicPlane : public BuiltinClass
{
public:
	static const TypeInfoPtr s_typeInfo;

	static std::shared_ptr<DynamicPlane> DynamicConstructor(const std::vector<DynamicValueBasePtr>& args);

	DynamicPlane();
	DynamicPlane(const math::Plane<float>& plane);
	DynamicPlane(Vector3Ptr point, Vector3Ptr normal);
	virtual ~DynamicPlane();

	explicit operator math::Plane<float>() const;

	DynamicPlane& operator=(const math::Plane<float>& p);

	void AcceptVisitor(ValueVisitorBase& visitor) override;

	bool operator==(const DynamicPlane& p) const;
	bool operator!=(const DynamicPlane& p) const;

	std::string ToString() const override;

	Vector3Ptr GetPoint();
	Vector3Ptr GetPoint2();
	Vector3Ptr GetNormal();
	Vector3Ptr GetVector();
	Vector3Ptr GetVector2();
	FloatValuePtr GetDistanceToOrigin();

private:
	void RegisterMembers();

	math::Plane<float> m_nativePlane;
};
}  // namespace pagoda::dynamic
