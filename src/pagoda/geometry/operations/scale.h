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

	void SetParameters(objects::ParameterCallback* cb) override
	{
	}
	void Parameters(objects::NewParameterCallback* cb) override;
	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_input;
	objects::InterfacePtr m_output;

	float m_scaleX;
	float m_scaleY;
	float m_scaleZ;
	std::string m_scaleCenter;
};
}  // namespace pagoda::geometry::operations
