#pragma once

#include <pagoda/math/vec_base.h>

namespace pgeditor::rendering
{
struct Vertex
{
	boost::qvm::vec<float, 3> m_position;
	boost::qvm::vec<float, 3> m_normal;
	boost::qvm::vec<float, 2> m_texCoords;
};
}  // namespace pgeditor::rendering
