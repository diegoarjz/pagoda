#pragma once

#include "procedural_operation.h"

namespace pagoda
{
class FaceOffsetOperation : public ProceduralOperation
{
public:
	static const char* name;
	static const std::string inputGeometry;
	static const std::string outputInnerGeometry;
	static const std::string outputOuterGeometry;

	FaceOffsetOperation(ProceduralObjectSystemPtr objectSystem);
	virtual ~FaceOffsetOperation();

	void DoWork() override;
};
}  // namespace pagoda
