#ifndef PAGODA_TESTS_PROCEDURAL_OBJECTS_MOCK_OBJECTS_H_
#define PAGODA_TESTS_PROCEDURAL_OBJECTS_MOCK_OBJECTS_H_

#include <procedural_objects/geometry_component.h>
#include <procedural_objects/geometry_system.h>
#include <procedural_objects/procedural_object.h>
#include <procedural_objects/procedural_object_system.h>

#include <gmock/gmock.h>

using namespace pagoda;

class GeometrySystemMock : public GeometrySystem
{
public:
	MOCK_METHOD2(GetCreateRect, pagoda::CreateRect<Geometry>(const float&, const float&));
	MOCK_METHOD1(GetExtrude, pagoda::Extrusion<Geometry>(const float&));
};

#endif
