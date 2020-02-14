#ifndef SELECTOR_PROCEDURAL_OBJECTS_CREATE_RECT
#define SELECTOR_PROCEDURAL_OBJECTS_CREATE_RECT

#include "procedural_operation.h"

namespace selector
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

}  // namespace selector

#endif
