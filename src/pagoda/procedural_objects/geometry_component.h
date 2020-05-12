#ifndef PAGODA_PROCEDURAL_OBJECTS_GEOMETRY_COMPONENT_H_
#define PAGODA_PROCEDURAL_OBJECTS_GEOMETRY_COMPONENT_H_

#include "geometry_system.h"

#include <pagoda/geometry_core/geometry.h>
#include <pagoda/geometry_core/geometry_builder.h>
#include <pagoda/geometry_core/scope.h>

namespace pagoda
{
class GeometryComponent : public ProceduralComponent
{
public:
	static std::string GetComponentSystemName();

	virtual ~GeometryComponent(){};

	std::string GetType() const override { return GetComponentSystemName(); }
	void SetGeometry(GeometryPtr geom) { geometry = geom; }
	GeometryPtr GetGeometry() const { return geometry; }
	const Scope& GetScope() const;
	void SetScope(const Scope& scope);

private:
	GeometryPtr geometry;
	Scope m_scope;
};  // class GeometryComponent

}  // namespace pagoda

#endif
