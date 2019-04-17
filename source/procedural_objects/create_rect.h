#ifndef SELECTOR_PROCEDURAL_OBJECTS_CREATE_RECT
#define SELECTOR_PROCEDURAL_OBJECTS_CREATE_RECT

#include "procedural_operation.h"

namespace selector
{
class CreateRectGeometry : public ProceduralOperation::Registrar<CreateRectGeometry>
{
public:
	static const InterfaceName output_geometry;
	static const char* name;

	CreateRectGeometry();
	virtual ~CreateRectGeometry();

	void Execute() override;
	void SetExecutionContext(std::shared_ptr<OperationExecutionContext> context) override;

private:
	float width;
	float height;

};  // class CreateSquareGeometry

}  // namespace selector

#endif
