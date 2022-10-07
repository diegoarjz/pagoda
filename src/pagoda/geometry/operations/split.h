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

	void Parameters(objects::NewParameterCallback* cb) override;
	void ParameterChanged(std::shared_ptr<objects::ParameterBase> par) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	void updateWithSplitCount();

	objects::InterfacePtr m_input;
	std::array<objects::InterfacePtr, 8> m_outputs;

	std::string m_axis;
	int m_splitCount;
	std::array<float, 8> m_splitSizes;
};
}  // namespace pagoda::geometry::operations
