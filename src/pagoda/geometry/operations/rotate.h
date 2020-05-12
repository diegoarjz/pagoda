#pragma once

#include <pagoda/procedural_objects/procedural_operation.h>

namespace pagoda::geometry::operations
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
}  // namespace pagoda::geometry::operations
