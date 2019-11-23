#pragma once

#include "procedural_operation.h"

namespace selector
{
class Rotate : public ProceduralOperation
{
public:
	static const InterfaceName s_inputGeometry;
	static const InterfaceName s_outputGeometry;

	Rotate(ProceduralObjectSystemPtr objectSystem);
	virtual ~Rotate();

	void DoWork() override;
};
}  // namespace selector
