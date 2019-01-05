#ifndef SELECTOR_TESTS_GEOMETRY_CORE_MOCK_OBJECTS_H_
#define SELECTOR_TESTS_GEOMETRY_CORE_MOCK_OBJECTS_H_

#include <math_lib/vec_base.h>

class MockGeometry
{
public:
	using IndexType = uint32_t;
	using PositionType = selector::Vec3F;

	MOCK_METHOD1(CreateVertex, IndexType(const PositionType &position));
	MOCK_CONST_METHOD0(GetNumVertices, uint32_t(void));
	MOCK_CONST_METHOD0(GetReservedVertices, uint32_t(void));
	MOCK_CONST_METHOD0(GetReservedFaces, uint32_t(void));
	MOCK_METHOD1(UntemplatedCreateFace, IndexType(const std::vector<IndexType> &));
	MOCK_METHOD1(UntemplatedCreateFace, IndexType(const std::list<IndexType> &));

	template<class T>
	IndexType CreateFace(const T &vertex_indices)
	{
		return UntemplatedCreateFace(vertex_indices);
	}
};

#endif
