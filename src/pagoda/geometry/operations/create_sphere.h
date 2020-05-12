#pragma once

#include <pagoda/procedural_objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class CreateSphereGeometry : public ProceduralOperation
{
public:
	static const std::string outputGeometry;
	static const char* name;

	CreateSphereGeometry(ProceduralObjectSystemPtr objectSystem);
	virtual ~CreateSphereGeometry();

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
