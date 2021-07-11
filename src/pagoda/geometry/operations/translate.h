#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class Translate : public objects::ProceduralOperation
{
	public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	Translate(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~Translate();

	void SetParameters(graph::ExecutionArgumentCallback* cb) override;

	const std::string& GetOperationName() const override;

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
