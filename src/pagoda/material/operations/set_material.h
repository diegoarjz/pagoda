#pragma once

#include <pagoda/objects/procedural_object_system.h>
#include <pagoda/objects/procedural_operation.h>

namespace pagoda::material::operations
{
class SetMaterial : public objects::ProceduralOperation
{
	public:
	static const std::string inputObject;
	static const std::string outputObject;

	SetMaterial(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~SetMaterial();

	void SetParameters(graph::ExecutionArgumentCallback* cb) override;

	const std::string& GetOperationName() const override;
  void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;
};
}  // namespace pagoda::material::operations
