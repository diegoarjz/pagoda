#ifndef SELECTOR_PROCEDURAL_OPERATIONS_EXPORT_GEOMETRY_H_
#define SELECTOR_PROCEDURAL_OPERATIONS_EXPORT_GEOMETRY_H_

#include "procedural_operation.h"

namespace selector
{
class ExportGeometry : public ProceduralOperation
{
public:
	static const char* name;
	static const InterfaceName inputGeometry;

	ExportGeometry(ProceduralObjectSystemPtr objectSystem);
	~ExportGeometry();

	void Execute() override;
};
}  // namespace selector

#endif
