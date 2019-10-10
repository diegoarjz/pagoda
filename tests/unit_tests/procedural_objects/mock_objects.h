#ifndef SELECTOR_TESTS_PROCEDURAL_OBJECTS_MOCK_OBJECTS_H_
#define SELECTOR_TESTS_PROCEDURAL_OBJECTS_MOCK_OBJECTS_H_

#include <procedural_objects/geometry_component.h>
#include <procedural_objects/geometry_system.h>
#include <procedural_objects/procedural_object.h>
#include <procedural_objects/procedural_object_system.h>

#include <gmock/gmock.h>

using namespace selector;

class GeometrySystemMock : public GeometrySystem
{
public:
	MOCK_METHOD2(GetCreateRect, selector::CreateRect<Geometry>(const float&, const float&));
	MOCK_METHOD1(GetExtrude, selector::Extrusion<Geometry>(const float&));
};

#endif
