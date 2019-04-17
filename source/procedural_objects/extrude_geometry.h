#ifndef SELECTOR_PROCEDURAL_OBJECTS_EXTRUDE_GEOMETRY
#define SELECTOR_PROCEDURAL_OBJECTS_EXTRUDE_GEOMETRY

#include "procedural_operation.h"

namespace selector
{
class ExtrudeGeometry : public ProceduralOperation::Registrar<ExtrudeGeometry>
{
public:
	static const char* name;
	static const InterfaceName input_geometry;
	static const InterfaceName output_geometry;

	ExtrudeGeometry();
	virtual ~ExtrudeGeometry();

	void Execute() override;
	void SetExecutionContext(std::shared_ptr<OperationExecutionContext> context) override;

private:
	float extrusion_amount;
};  // class ExtrudeGEometry

}  // namespace selector

#endif
