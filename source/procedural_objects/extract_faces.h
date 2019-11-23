#pragma once

#include "procedural_operation.h"

namespace selector
{
class ExtractFaces : public ProceduralOperation
{
public:
	static const InterfaceName s_inputGeometry;
	static const InterfaceName s_outputGeometry;

	ExtractFaces(ProceduralObjectSystemPtr objectSystem);
	virtual ~ExtractFaces();

	void DoWork() override;
};  // class ExtractFaces
}  // namespace selector
