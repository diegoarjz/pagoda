#ifndef PAGODA_PROCEDURAL_OBJECTS_CREATE_RECT
#define PAGODA_PROCEDURAL_OBJECTS_CREATE_RECT

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class CreateRectGeometry : public objects::ProceduralOperation
{
public:
	static const std::string output_geometry;
	static const char* name;

	CreateRectGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~CreateRectGeometry();

	void DoWork() override;
};  // class CreateSquareGeometry

}  // namespace pagoda::geometry::operations

#endif
