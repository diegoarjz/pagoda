#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class Translate : public objects::ProceduralOperation
{
public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	Translate(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~Translate();

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
