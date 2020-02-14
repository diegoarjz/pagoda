#ifndef SELECTOR_PROCEDURAL_OBJECTS_CLIP_GEOMETRY_H_
#define SELECTOR_PROCEDURAL_OBJECTS_CLIP_GEOMETRY_H_

#include "procedural_operation.h"

namespace selector
{
class ClipGeometry : public ProceduralOperation
{
public:
	static const std::string inputGeometry;
	static const std::string frontGeometry;
	static const std::string backGeometry;
	static const char* name;

	ClipGeometry(ProceduralObjectSystemPtr objectSystem);
	virtual ~ClipGeometry();

	void DoWork() override;
};
}  // namespace selector

#endif
