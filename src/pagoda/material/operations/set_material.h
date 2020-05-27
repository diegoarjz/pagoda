#pragma once

#include <pagoda/objects/procedural_operation.h>
#include "pagoda/objects/procedural_object_system.h"

namespace pagoda::material::operations
{
class SetMaterial : public objects::ProceduralOperation
{
public:
	static const std::string inputObject;
	static const std::string outputObject;

	SetMaterial(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~SetMaterial();

	void DoWork() override;
};
}  // namespace pagoda::material::operations
