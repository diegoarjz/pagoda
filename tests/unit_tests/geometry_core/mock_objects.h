#ifndef PAGODA_TESTS_GEOMETRY_CORE_MOCK_OBJECTS_H_
#define PAGODA_TESTS_GEOMETRY_CORE_MOCK_OBJECTS_H_

#include <pagoda/math/vec_base.h>

class MockGeometry
{
	public:
	using Index_t = uint32_t;
	using PositionType = pagoda::math::Vec3F;

	MOCK_METHOD1(CreateVertex, Index_t(const PositionType &position));
	MOCK_CONST_METHOD0(GetNumVertices, uint32_t(void));
	MOCK_CONST_METHOD0(GetReservedVertices, uint32_t(void));
	MOCK_CONST_METHOD0(GetReservedFaces, uint32_t(void));
	MOCK_METHOD1(UntemplatedCreateFace, Index_t(const std::vector<Index_t> &));
	MOCK_METHOD1(UntemplatedCreateFace, Index_t(const std::list<Index_t> &));

	template<class T>
	Index_t CreateFace(const T &vertex_indices)
	{
		return UntemplatedCreateFace(vertex_indices);
	}
};

#endif
