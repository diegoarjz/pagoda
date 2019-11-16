#pragma once

#include "procedural_operation.h"

namespace selector
{
class Split : public ProceduralOperation
{
public:
	static const InterfaceName s_inputGeometry;

	Split(ProceduralObjectSystemPtr objectSystem);
	virtual ~Split();

	void DoWork() override;
};
}  // namespace selector
