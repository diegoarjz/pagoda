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

	const std::string& GetOperationName() const override;

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
