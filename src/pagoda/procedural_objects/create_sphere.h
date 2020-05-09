#pragma once

#include "procedural_operation.h"

namespace pagoda
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
}  // namespace pagoda
