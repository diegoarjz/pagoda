#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::objects
{
using InterfacePtr = std::shared_ptr<class Interface>;
}

namespace pagoda::geometry::operations
{
class ExtrudeGeometry : public objects::ProceduralOperation
{
	public:
	static const char* name;
	static const std::string input_geometry;
	static const std::string output_geometry;

	ExtrudeGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~ExtrudeGeometry();

	void SetParameters(objects::ParameterCallback* cb) override;

	const std::string& GetOperationName() const override;

	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_in;
	objects::InterfacePtr m_out;
};  // class ExtrudeGEometry

}  // namespace pagoda::geometry::operations

