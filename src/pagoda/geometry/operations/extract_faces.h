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

	void SetParameters(objects::ParameterCallback* cb) override
	{
	}

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_input;
	objects::InterfacePtr m_output;
};  // class ExtractFaces
}  // namespace pagoda::geometry::operations
