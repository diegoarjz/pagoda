#pragma once

#include <pagoda/procedural_objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class ExtractFaces : public ProceduralOperation
{
public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	ExtractFaces(ProceduralObjectSystemPtr objectSystem);
	virtual ~ExtractFaces();

	void DoWork() override;
};  // class ExtractFaces
}  // namespace pagoda::geometry::operations
