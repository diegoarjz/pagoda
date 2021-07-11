#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class ExtractFaces : public objects::ProceduralOperation
{
	public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	ExtractFaces(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~ExtractFaces();

	void SetParameters(graph::ExecutionArgumentCallback* cb) override;

	const std::string& GetOperationName() const override;

	void DoWork() override;
};  // class ExtractFaces
}  // namespace pagoda::geometry::operations
