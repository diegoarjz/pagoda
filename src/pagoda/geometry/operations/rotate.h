#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class Rotate : public objects::ProceduralOperation
{
	public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	Rotate(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~Rotate();

	void SetParameters(objects::ParameterCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_input;
	objects::InterfacePtr m_output;
};
}  // namespace pagoda::geometry::operations
