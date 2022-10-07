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

	void Parameters(objects::NewParameterCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_input;
	objects::InterfacePtr m_output;

	float m_rotX;
	float m_rotY;
	float m_rotZ;
	std::string m_rotationOrder;
	bool m_world;
};
}  // namespace pagoda::geometry::operations
