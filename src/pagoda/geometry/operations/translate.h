#pragma once

#include <pagoda/procedural_objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class Translate : public ProceduralOperation
{
public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	Translate(ProceduralObjectSystemPtr objectSystem);
	virtual ~Translate();

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
