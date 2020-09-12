#ifndef PAGODA_PROCEDURAL_OBJECTS_REPEAT_SPLIT_H_
#define PAGODA_PROCEDURAL_OBJECTS_REPEAT_SPLIT_H_

#include <pagoda/math/plane.h>

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::core
{
class Scope;
}

namespace pagoda::geometry::operations
{
class RepeatSplit : public objects::ProceduralOperation
{
	public:
	static const std::string inputGeometry;
	static const std::string outputGeometry;
	static const char *name;

	RepeatSplit(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~RepeatSplit();

	void DoWork() override;

	private:
	std::vector<math::Plane<float>> CreatePlanes(const core::Scope &scope, const float &size, const std::string &axis,
	                                             bool adjust);
};
}  // namespace pagoda::geometry::operations

#endif
