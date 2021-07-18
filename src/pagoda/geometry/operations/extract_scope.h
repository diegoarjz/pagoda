#ifndef PAGODA_PROCEDURAL_OBJECTS_EXTRACT_SCOPE_H_
#define PAGODA_PROCEDURAL_OBJECTS_EXTRACT_SCOPE_H_

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class ExtractScope : public objects::ProceduralOperation
{
	public:
	static const std::string inputGeometry;
	static const std::string outputGeometry;
	static const char* name;

	ExtractScope(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~ExtractScope();

	void SetParameters(graph::ExecutionArgumentCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

  private:
  objects::InterfacePtr m_input;
  objects::InterfacePtr m_output;
};
}  // namespace pagoda::geometry::operations
#endif
