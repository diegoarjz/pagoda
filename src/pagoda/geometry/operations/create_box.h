#ifndef PAGODA_PROCEDURAL_OBJECTS_CREATE_BOX_H_
#define PAGODA_PROCEDURAL_OBJECTS_CREATE_BOX_H_

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class CreateBoxGeometry : public objects::ProceduralOperation
{
	public:
	static const std::string outputGeometry;
	static const char* name;

	CreateBoxGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~CreateBoxGeometry();

	void SetParameters(objects::ParameterCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_output;
};
}  // namespace pagoda::geometry::operations
#endif
