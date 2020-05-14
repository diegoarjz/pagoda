#ifndef PAGODA_PROCEDURAL_OBJECTS_GEOMETRY_COMPONENT_H_
#define PAGODA_PROCEDURAL_OBJECTS_GEOMETRY_COMPONENT_H_

#include "geometry_system.h"

#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/geometry/core/scope.h>

namespace pagoda::geometry
{
class GeometryComponent : public objects::ProceduralComponent
{
public:
	static std::string GetComponentSystemName();

	virtual ~GeometryComponent(){};

	std::string GetType() const override { return GetComponentSystemName(); }
	void SetGeometry(GeometryPtr geom) { geometry = geom; }
	GeometryPtr GetGeometry() const { return geometry; }
	const geometry::core::Scope& GetScope() const;
	void SetScope(const geometry::core::Scope& scope);

private:
	GeometryPtr geometry;
	geometry::core::Scope m_scope;
};  // class GeometryComponent

}  // namespace pagoda::geometry

#endif
