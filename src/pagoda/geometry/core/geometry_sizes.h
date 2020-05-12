#pragma once
#ifndef PAGODA_GEOMETRY_CORE_GEOMETRY_SIZES_H_
#define PAGODA_GEOMETRY_CORE_GEOMETRY_SIZES_H_

#include <cstdint>

namespace pagoda::geometry::core
{
struct GeometrySizes
{
	uint32_t m_numVertices;
	uint32_t m_numEdges;
	uint32_t m_numFaces;

	GeometrySizes(uint32_t verts, uint32_t edges, uint32_t faces)
	    : m_numVertices(verts), m_numEdges(edges), m_numFaces(faces)
	{
	}

};  // struct GeometryDimensions
}  // namespace pagoda::geometry::core

#endif
