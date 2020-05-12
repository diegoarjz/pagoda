#ifndef PAGODA_PROCEDURAL_OBJECTS_CLIP_GEOMETRY_H_
#define PAGODA_PROCEDURAL_OBJECTS_CLIP_GEOMETRY_H_

#include <pagoda/procedural_objects/procedural_operation.h>

namespace pagoda::geometry::operations
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
}  // namespace pagoda::geometry::operations

#endif
