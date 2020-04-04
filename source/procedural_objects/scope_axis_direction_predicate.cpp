#include "scope_axis_direction_predicate.h"

#include "geometry_component.h"
#include "geometry_core/scope.h"
#include "geometry_system.h"
#include "math_lib/radians.h"
#include "procedural_object_system.h"

#include <boost/qvm/vec_operations.hpp>

#include <cmath>
#include <sstream>

namespace selector
{
ScopeAxisDirectionPredicate::ScopeAxisDirectionPredicate(ProceduralObjectSystemPtr objectSystem, char scopeAxis,
                                                         const Vec3F& direction, const Degrees<float>& tolerance)
    : ProceduralObjectPredicate(objectSystem),
      m_scopeAxis(scopeAxis),
      m_direction(normalized(direction)),
      m_tolerance(tolerance)
{
	m_geometrySystem = objectSystem->GetComponentSystem<GeometrySystem>();
}

ScopeAxisDirectionPredicate::~ScopeAxisDirectionPredicate() {}

bool ScopeAxisDirectionPredicate::operator()(const ProceduralObjectPtr object)
{
	auto geometryComponent = m_geometrySystem->GetComponentAs<GeometryComponent>(object);
	if (geometryComponent == nullptr)
	{
		return false;
	}
	Scope scope = geometryComponent->GetScope();
	Vec3F axis = scope.GetAxis(m_scopeAxis);

	auto angle = std::acos(boost::qvm::dot(axis, m_direction));
	if (angle <= static_cast<float>(Radians<float>(m_tolerance)))
	{
		return true;
	}

	return false;
}

std::string ScopeAxisDirectionPredicate::ToString()
{
	std::stringstream ss;
	ss << "ScopeAxisDirectionPredicate. axis: " << m_scopeAxis << " direction: " << m_direction
	   << " tolerance: " << static_cast<float>(m_tolerance);
	return ss.str();
}
}  // namespace selector
