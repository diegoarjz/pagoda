#pragma once

#include <cstddef>

namespace pgeditor::renderer
{
enum class Format
{
	F32,
	Invalid
};

enum class Type
{
	Int,
	Int2,
	UInt,
	UInt2,
	Float,
	Double,
	Vec2,
	Vec3,
	Vec4,
	Mat2,
	Mat3,
	Mat4
};

template<class T>
struct BufferTypeTraits
{
	static const Format format;
	static const std::size_t componentsPerElement;
	static const std::size_t bytesPerComponent;
};

}  // namespace pgeditor::renderer
