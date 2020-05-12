#pragma once

#include "procedural_object_predicate.h"

#include <pagoda/math/degrees.h>
#include <pagoda/math/vec_base.h>

namespace pagoda
{
class GeometrySystem;
using GeometrySystemPtr = std::shared_ptr<GeometrySystem>;

class ScopeAxisDirectionPredicate : public ProceduralObjectPredicate
{
public:
	/**
	 * Constructs the predicate with the \p direction and a \p tolerance.
	 */
	ScopeAxisDirectionPredicate(ProceduralObjectSystemPtr objectSystem, char scopeAxis, const math::Vec3F& direction,
	                            const math::Degrees<float>& tolerance = math::Degrees<float>(0.0f));

	virtual ~ScopeAxisDirectionPredicate();

	/**
	 * Returns true if the given axis of the \p object's \c Scope is facing the specified direction.
	 */
	bool operator()(const ProceduralObjectPtr object) override;

	/**
	 * Describes this predicate.
	 */
	std::string ToString() override;

private:
	char m_scopeAxis;
	math::Vec3F m_direction;
	math::Degrees<float> m_tolerance;

	GeometrySystemPtr m_geometrySystem;
};
}  // namespace pagoda
