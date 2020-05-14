#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class Split : public objects::ProceduralOperation
{
public:
	static const std::string s_inputGeometry;

	Split(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~Split();

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
