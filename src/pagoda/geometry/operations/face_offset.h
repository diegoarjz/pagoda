#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class FaceOffsetOperation : public objects::ProceduralOperation
{
	public:
	static const char* name;
	static const std::string inputGeometry;
	static const std::string outputInnerGeometry;
	static const std::string outputOuterGeometry;

	FaceOffsetOperation(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~FaceOffsetOperation();

	void SetParameters(objects::ParameterCallback* cb) override
	{
	}
	void Parameters(objects::NewParameterCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_input;
	objects::InterfacePtr m_inner;
	objects::InterfacePtr m_outer;

	float m_amount;
};
}  // namespace pagoda::geometry::operations
