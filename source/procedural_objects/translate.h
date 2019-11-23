#pragma once

#include "procedural_operation.h"

namespace selector
{
class Translate : public ProceduralOperation
{
public:
	static const InterfaceName s_inputGeometry;
	static const InterfaceName s_outputGeometry;

	Translate(ProceduralObjectSystemPtr objectSystem);
	virtual ~Translate();

	void DoWork() override;
};
}  // namespace selector
