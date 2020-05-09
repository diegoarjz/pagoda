#pragma once

#include <memory>
#include <string>

namespace pagoda
{
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;

class ProceduralObjectSystem;
using ProceduralObjectSystemPtr = std::shared_ptr<ProceduralObjectSystem>;

/**
 * Interface for predicates on \c ProceduralObject.
 */
class ProceduralObjectPredicate
{
public:
	ProceduralObjectPredicate(ProceduralObjectSystemPtr objectSystem);

	/**
	 * Returns true if the \c ProceduralObject referenced by \p object passes the predicate.
	 */
	virtual bool operator()(const ProceduralObjectPtr object) = 0;

	/**
	 * Describes this predicate.
	 * Useful for debugging and logging.
	 */
	virtual std::string ToString() = 0;

protected:
	ProceduralObjectSystemPtr m_objectSystem;
};
}  // namespace pagoda
