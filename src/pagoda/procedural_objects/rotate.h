#pragma once

#include "procedural_operation.h"

namespace pagoda
{
class Rotate : public ProceduralOperation
{
public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	Rotate(ProceduralObjectSystemPtr objectSystem);
	virtual ~Rotate();

	void DoWork() override;
};
}  // namespace pagoda
