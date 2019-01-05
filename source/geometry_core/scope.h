#ifndef SELECTOR_GEOMETRY_CORE_SCOPE_H_
#define SELECTOR_GEOMETRY_CORE_SCOPE_H_

#include <math_lib/matrix_base.h>
#include <math_lib/vec_base.h>

namespace selector
{
class Scope
{
public:
	Scope();
	Scope(const Vec3F &pos, const Vec3F &size, const Mat3x3F &rot);

	Vec3F GetPosition() const;
	void SetPosition(const Vec3F &pos);
	Vec3F GetSize() const;
	void SetSize(const Vec3F &size);
	Mat3x3F GetRotation() const;
	void SetRotation(const Mat3x3F &rotation);

private:
	Vec3F m_position;
	Vec3F m_size;
	Mat3x3F m_rotation;
};  // class Scope
}  // namespace selector
#endif
