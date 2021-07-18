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

	void SetParameters(graph::ExecutionArgumentCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

  private:
  objects::InterfacePtr m_output;
};
}  // namespace pagoda::geometry::operations
