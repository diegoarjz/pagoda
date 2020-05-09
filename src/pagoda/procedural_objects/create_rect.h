#ifndef PAGODA_PROCEDURAL_OBJECTS_CREATE_RECT
#define PAGODA_PROCEDURAL_OBJECTS_CREATE_RECT

#include "procedural_operation.h"

namespace pagoda
{
class CreateRectGeometry : public ProceduralOperation
{
public:
	static const std::string output_geometry;
	static const char* name;

	CreateRectGeometry(ProceduralObjectSystemPtr objectSystem);
	virtual ~CreateRectGeometry();

	void DoWork() override;
};  // class CreateSquareGeometry

}  // namespace pagoda

#endif
