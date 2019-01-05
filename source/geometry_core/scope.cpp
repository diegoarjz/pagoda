#include "scope.h"

namespace selector
{
Scope::Scope() : m_position(0, 0, 0), m_size(0, 0, 0), m_rotation(1) {}

Scope::Scope(const Vec3F &pos, const Vec3F &size, const Mat3x3F &rot) : m_position(pos), m_size(size), m_rotation(rot)
{
}

Vec3F Scope::GetPosition() const { return m_position; }

void Scope::SetPosition(const Vec3F &pos) { m_position = pos; }

Vec3F Scope::GetSize() const { return m_size; }

void Scope::SetSize(const Vec3F &size) { m_size = size; }

Mat3x3F Scope::GetRotation() const { return m_rotation; }

void Scope::SetRotation(const Mat3x3F &rotation) { m_rotation = rotation; }
}  // namespace selector
