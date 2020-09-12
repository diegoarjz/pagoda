#pragma once

#include <pagoda/objects/procedural_object_system.h>
#include <pagoda/objects/procedural_operation.h>

namespace pagoda::material::operations
{
class SetMaterialAttribute : public objects::ProceduralOperation
{
	public:
	static const std::string inputObject;
	static const std::string outputObject;

	SetMaterialAttribute(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~SetMaterialAttribute();

	void DoWork() override;
};
}  // namespace pagoda::material::operations
