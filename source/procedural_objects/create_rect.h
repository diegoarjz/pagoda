#ifndef SELECTOR_PROCEDURAL_OBJECTS_CREATE_RECT
#define SELECTOR_PROCEDURAL_OBJECTS_CREATE_RECT

#include "procedural_operation.h"

namespace selector
{
class CreateRectGeometry : public ProceduralOperation
{
public:
	static const InterfaceName output_geometry;

	CreateRectGeometry();
	virtual ~CreateRectGeometry();

	void Execute() override;
	void SetExecutionContext(std::shared_ptr<OperationExecutionContext> context) override;

private:
	std::shared_ptr<ParameterBase> GetParameter(const std::string& name) override;

	std::shared_ptr<ParameterBase> width;
	std::shared_ptr<ParameterBase> height;

};  // class CreateSquareGeometry

}  // namespace selector

#endif
