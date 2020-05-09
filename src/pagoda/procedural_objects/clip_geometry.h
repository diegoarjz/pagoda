#ifndef PAGODA_PROCEDURAL_OBJECTS_CLIP_GEOMETRY_H_
#define PAGODA_PROCEDURAL_OBJECTS_CLIP_GEOMETRY_H_

#include "procedural_operation.h"

namespace pagoda
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
}  // namespace pagoda

#endif
