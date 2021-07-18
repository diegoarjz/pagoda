#pragma once

#include <pagoda/objects/procedural_operation.h>

#include <array>

namespace pagoda::geometry::operations
{
class Split : public objects::ProceduralOperation
{
	public:
	static const std::string s_inputGeometry;

	Split(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~Split();

	void SetParameters(graph::ExecutionArgumentCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

  private:
  objects::InterfacePtr m_input;
  std::array<objects::InterfacePtr, 8> m_outputs;
};
}  // namespace pagoda::geometry::operations
