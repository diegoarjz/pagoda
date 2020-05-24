#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class ScopeTextureProjection : public objects::ProceduralOperation
{
public:
	static const std::string s_inputGeometry;
	static const std::string s_outputGeometry;

	ScopeTextureProjection(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~ScopeTextureProjection();

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
