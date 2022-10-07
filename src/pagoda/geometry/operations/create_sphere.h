#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class CreateSphereGeometry : public objects::ProceduralOperation
{
	public:
	static const std::string outputGeometry;
	static const char* name;

	CreateSphereGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~CreateSphereGeometry();

	void Parameters(objects::NewParameterCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_output;

	float m_radius;
	int m_slices;
	int m_stacks;
};
}  // namespace pagoda::geometry::operations
