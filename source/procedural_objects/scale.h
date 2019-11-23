#pragma once

#include "procedural_operation.h"

namespace selector
{
class Scale : public ProceduralOperation
{
public:
	static const InterfaceName s_inputGeometry;
	static const InterfaceName s_outputGeometry;

	Scale(ProceduralObjectSystemPtr objectSystem);
	virtual ~Scale();

	void DoWork() override;
};
}  // namespace selector

