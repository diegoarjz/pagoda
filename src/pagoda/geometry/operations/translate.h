#pragma once

#include <pagoda/objects/procedural_operation.h>

#include <pagoda/math/vec_base.h>

namespace pagoda::geometry::operations
{
class Translate : public objects::ProceduralOperation
{
	public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	Translate(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~Translate();

	void Parameters(objects::NewParameterCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_input;
	objects::InterfacePtr m_output;

	math::Vec3F m_translate;
	bool m_world;
};
}  // namespace pagoda::geometry::operations
