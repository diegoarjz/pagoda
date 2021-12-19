#pragma once

#include "types.h"

namespace pgeditor::renderer
{
enum class VertexAttributeSemantics
{
	Position,
	Normal,
	TexCoord,
	Color,
	Custom,
	Invalid
};

struct VertexAttributeDescription
{
	VertexAttributeSemantics m_semantics;
	Type m_type;
	std::size_t m_componentsPerElement;
	std::size_t m_bytesPerComponent;
};

}  // namespace pgeditor::renderer
