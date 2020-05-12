#ifndef PAGODA_PROCEDURAL_OBJECTS_REPEAT_SPLIT_H_
#define PAGODA_PROCEDURAL_OBJECTS_REPEAT_SPLIT_H_

#include <pagoda/math/plane.h>

#include "procedural_operation.h"

namespace pagoda
{
class Scope;

class RepeatSplit : public ProceduralOperation
{
public:
	static const std::string inputGeometry;
	static const std::string outputGeometry;
	static const char *name;

	RepeatSplit(ProceduralObjectSystemPtr objectSystem);
	virtual ~RepeatSplit();

	void DoWork() override;

private:
	std::vector<math::Plane<float>> CreatePlanes(const Scope &scope, const float &size, const std::string &axis,
	                                             bool adjust);
};
}  // namespace pagoda

#endif
