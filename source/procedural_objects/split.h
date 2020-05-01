#pragma once

#include "procedural_operation.h"

namespace pagoda
{
class Split : public ProceduralOperation
{
public:
	static const std::string s_inputGeometry;

	Split(ProceduralObjectSystemPtr objectSystem);
	virtual ~Split();

	void DoWork() override;
};
}  // namespace pagoda
