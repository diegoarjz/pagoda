#pragma once

#include <pagoda/procedural_objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class Split : public ProceduralOperation
{
public:
	static const std::string s_inputGeometry;

	Split(ProceduralObjectSystemPtr objectSystem);
	virtual ~Split();

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
