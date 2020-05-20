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

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
