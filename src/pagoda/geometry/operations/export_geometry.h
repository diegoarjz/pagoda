#ifndef PAGODA_PROCEDURAL_OPERATIONS_EXPORT_GEOMETRY_H_
#define PAGODA_PROCEDURAL_OPERATIONS_EXPORT_GEOMETRY_H_

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class ExportGeometry : public objects::ProceduralOperation
{
public:
	static const char* name;
	static const std::string inputGeometry;

	ExportGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	~ExportGeometry();

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations

#endif
