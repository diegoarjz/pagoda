#pragma once

#include <pagoda/objects/procedural_object_system.h>
#include <pagoda/objects/procedural_operation.h>

namespace pagoda::material::operations
{
class SetShader : public objects::ProceduralOperation
{
	public:
	static const std::string inputObject;
	static const std::string outputObject;

	SetShader(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~SetShader();

	const std::string& GetOperationName() const override;

	void DoWork() override;
};
}  // namespace pagoda::material::operations
