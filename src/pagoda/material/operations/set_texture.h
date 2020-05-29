#pragma once

#include <pagoda/objects/procedural_object_system.h>
#include <pagoda/objects/procedural_operation.h>

namespace pagoda::material::operations
{
class SetTexture : public objects::ProceduralOperation
{
public:
	static const std::string inputObject;
	static const std::string outputObject;

	SetTexture(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~SetTexture();

	void DoWork() override;
};
}  // namespace pagoda::material::operations
