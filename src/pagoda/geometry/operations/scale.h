#pragma once

#include <pagoda/procedural_objects/procedural_operation.h>

namespace pagoda::geometry::operations
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
}  // namespace pagoda::geometry::operations

