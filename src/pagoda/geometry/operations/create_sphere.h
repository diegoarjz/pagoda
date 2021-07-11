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

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
