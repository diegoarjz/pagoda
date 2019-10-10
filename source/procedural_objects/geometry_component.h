#ifndef SELECTOR_PROCEDURAL_OBJECTS_GEOMETRY_COMPONENT_H_
#define SELECTOR_PROCEDURAL_OBJECTS_GEOMETRY_COMPONENT_H_

#include "geometry_system.h"

#include "geometry_core/geometry.h"
#include "geometry_core/geometry_builder.h"

namespace selector
{
class GeometryComponent : public ProceduralComponent
{
public:
	static std::string GetComponentSystemName();

	virtual ~GeometryComponent(){};

    std::string GetType() const override { return GetComponentSystemName(); }
	void SetGeometry(GeometryPtr geom) { geometry = geom; }
	GeometryPtr GetGeometry() const { return geometry; }

private:
	GeometryPtr geometry;
};  // class GeometryComponent

}  // namespace selector

#endif
