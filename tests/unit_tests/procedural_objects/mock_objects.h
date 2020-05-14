#ifndef PAGODA_TESTS_PROCEDURAL_OBJECTS_MOCK_OBJECTS_H_
#define PAGODA_TESTS_PROCEDURAL_OBJECTS_MOCK_OBJECTS_H_

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>
#include <pagoda/objects/procedural_object.h>
#include <pagoda/objects/procedural_object_system.h>

#include <gmock/gmock.h>

using namespace pagoda;
using namespace pagoda::geometry;
using namespace pagoda::objects;
using namespace pagoda::geometry::core;
using namespace pagoda::geometry::algorithms;

class GeometrySystemMock : public GeometrySystem
{
public:
	MOCK_METHOD2(GetCreateRect, CreateRect<Geometry>(const float&, const float&));
	MOCK_METHOD1(GetExtrude, Extrusion<Geometry>(const float&));
};

#endif
