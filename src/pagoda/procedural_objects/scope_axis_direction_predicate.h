#pragma once

#include "procedural_object_predicate.h"

#include <pagoda/math_lib/degrees.h>
#include <pagoda/math_lib/vec_base.h>

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
	ScopeAxisDirectionPredicate(ProceduralObjectSystemPtr objectSystem, char scopeAxis, const Vec3F& direction,
	                            const Degrees<float>& tolerance = Degrees<float>(0.0f));

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
	Vec3F m_direction;
	Degrees<float> m_tolerance;

	GeometrySystemPtr m_geometrySystem;
};
}  // namespace pagoda
