#ifndef PAGODA_PROCEDURAL_OBJECTS_EXTRUDE_GEOMETRY
#define PAGODA_PROCEDURAL_OBJECTS_EXTRUDE_GEOMETRY

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class ExtrudeGeometry : public objects::ProceduralOperation
{
public:
	static const char* name;
	static const std::string input_geometry;
	static const std::string output_geometry;

	ExtrudeGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~ExtrudeGeometry();

	void DoWork() override;
};  // class ExtrudeGEometry

}  // namespace pagoda::geometry::operations

#endif
