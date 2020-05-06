#ifndef PAGODA_PROCEDURAL_OPERATIONS_EXPORT_GEOMETRY_H_
#define PAGODA_PROCEDURAL_OPERATIONS_EXPORT_GEOMETRY_H_

#include "procedural_operation.h"

namespace pagoda
{
class ExportGeometry : public ProceduralOperation
{
public:
	static const char* name;
	static const std::string inputGeometry;

	ExportGeometry(ProceduralObjectSystemPtr objectSystem);
	~ExportGeometry();

	void DoWork() override;
};
}  // namespace pagoda

#endif
