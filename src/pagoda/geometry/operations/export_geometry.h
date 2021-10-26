#ifndef PAGODA_PROCEDURAL_OPERATIONS_EXPORT_GEOMETRY_H_
#define PAGODA_PROCEDURAL_OPERATIONS_EXPORT_GEOMETRY_H_

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class ExportGeometry : public objects::ProceduralOperation
{
	public:
	static const char* name;
	static const std::string inputGeometry;

	ExportGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	~ExportGeometry();

	void SetParameters(objects::ParameterCallback* cb) override
	{
	}
	void Parameters(objects::NewParameterCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_input;

	int32_t m_objectCount;
	std::string m_path;
};
}  // namespace pagoda::geometry::operations

#endif
