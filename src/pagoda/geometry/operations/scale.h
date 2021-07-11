#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class Scale : public objects::ProceduralOperation
{
	public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	Scale(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~Scale();

	void SetParameters(graph::ExecutionArgumentCallback* cb) override;
	const std::string& GetOperationName() const override;

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
