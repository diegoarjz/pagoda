#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class ExtractFaces : public objects::ProceduralOperation
{
public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	ExtractFaces(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~ExtractFaces();

	void DoWork() override;
};  // class ExtractFaces
}  // namespace pagoda::geometry::operations