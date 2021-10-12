#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::objects
{
using InterfacePtr = std::shared_ptr<class Interface>;
}

namespace pagoda::geometry::operations
{
class CreateRectGeometry : public objects::ProceduralOperation
{
	public:
	static const std::string output_geometry;
	static const char* name;

	CreateRectGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~CreateRectGeometry();

	void SetParameters(objects::ParameterCallback* cb) override;

	const std::string& GetOperationName() const override;

	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_out;
};  // class CreateSquareGeometry

}  // namespace pagoda::geometry::operations

