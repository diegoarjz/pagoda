#include "geometry_system.h"

#include "common/assertions.h"
#include "common/profiler.h"
#include "geometry_component.h"
#include "procedural_object.h"

namespace selector
{
GeometrySystem::~GeometrySystem()
{
	DBG_ASSERT_MSG(geometry_components.size() == 0, "Geometry System has components on destruction");
}

std::shared_ptr<GeometryComponent> GeometrySystem::CreateComponent(GeometryPtr geometry)
{
	START_PROFILE;

	auto geometry_component = std::dynamic_pointer_cast<GeometryComponent>(CreateComponent());
	geometry_component->SetGeometry(geometry);
	return geometry_component;
}

std::shared_ptr<ProceduralComponent> GeometrySystem::CreateComponent()
{
	START_PROFILE;

	auto geometry_component = std::make_shared<GeometryComponent>();
	geometry_components.insert(geometry_component);
	return geometry_component;
}

void GeometrySystem::KillProceduralComponent(std::shared_ptr<ProceduralComponent> component)
{
	START_PROFILE;

	DBG_ASSERT_MSG(std::dynamic_pointer_cast<GeometryComponent>(component) != nullptr,
	               "Trying to kill a component that is not a geometry component");
	geometry_components.erase(std::dynamic_pointer_cast<GeometryComponent>(component));
}

CreateRect<Geometry> GeometrySystem::GetCreateRect(const float& width, const float& height)
{
	START_PROFILE;

	return CreateRect<Geometry>(width, height);
}

Extrusion<Geometry> GeometrySystem::GetExtrude(const float& amount)
{
	START_PROFILE;

	return Extrusion<Geometry>(amount);
}

}  // namespace selector
