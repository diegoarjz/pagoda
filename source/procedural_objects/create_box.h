#ifndef SELECTOR_PROCEDURAL_OBJECTS_CREATE_BOX_H_
#define SELECTOR_PROCEDURAL_OBJECTS_CREATE_BOX_H_

#include "procedural_operation.h"

namespace selector
{
class CreateBoxGeometry : public ProceduralOperation
{
public:
	static const std::string outputGeometry;
	static const char* name;

	CreateBoxGeometry(ProceduralObjectSystemPtr objectSystem);
	virtual ~CreateBoxGeometry();

	void DoWork() override;
};
}  // namespace selector
#endif
