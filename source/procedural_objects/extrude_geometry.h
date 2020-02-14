#ifndef SELECTOR_PROCEDURAL_OBJECTS_EXTRUDE_GEOMETRY
#define SELECTOR_PROCEDURAL_OBJECTS_EXTRUDE_GEOMETRY

#include "procedural_operation.h"

namespace selector
{
class ExtrudeGeometry : public ProceduralOperation
{
public:
	static const char* name;
	static const std::string input_geometry;
	static const std::string output_geometry;

	ExtrudeGeometry(ProceduralObjectSystemPtr objectSystem);
	virtual ~ExtrudeGeometry();

	void DoWork() override;
};  // class ExtrudeGEometry

}  // namespace selector

#endif
