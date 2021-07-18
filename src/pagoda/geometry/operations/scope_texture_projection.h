#pragma once

#include <pagoda/objects/procedural_operation.h>

#include <pagoda/math/vec_base.h>

namespace pagoda::geometry::operations
{
class ScopeTextureProjection : public objects::ProceduralOperation
{
	public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	ScopeTextureProjection(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~ScopeTextureProjection();

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

	std::string m_axis;
	math::Vec2F m_scale;
	math::Vec2F m_offset;
	bool m_clamp;
};
}  // namespace pagoda::geometry::operations
