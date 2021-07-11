#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pgeditor::renderer::operations
{
class Render : public pagoda::objects::ProceduralOperation
{
	public:
	static const std::string input;
	static const char* name;

	Render(pagoda::objects::ProceduralObjectSystemPtr objectSystem);
	~Render() override;

	void SetParameters(pagoda::graph::ExecutionArgumentCallback* cb) override;

	const std::string& GetOperationName() const override;

	void DoWork() override;
};
}  // namespace pgeditor::renderer::operations
