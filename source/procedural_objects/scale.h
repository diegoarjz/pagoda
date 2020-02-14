#pragma once

#include "procedural_operation.h"

namespace selector
{
class Scale : public ProceduralOperation
{
public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	Scale(ProceduralObjectSystemPtr objectSystem);
	virtual ~Scale();

	void DoWork() override;
};
}  // namespace selector

