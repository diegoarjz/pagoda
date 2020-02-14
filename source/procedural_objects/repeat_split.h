#ifndef SELECTOR_PROCEDURAL_OBJECTS_REPEAT_SPLIT_H_
#define SELECTOR_PROCEDURAL_OBJECTS_REPEAT_SPLIT_H_

#include "math_lib/plane.h"

#include "procedural_operation.h"

namespace selector
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
	std::vector<Plane<float>> CreatePlanes(const Scope &scope, const float &size, const std::string &axis, bool adjust);
};
}  // namespace selector

#endif
