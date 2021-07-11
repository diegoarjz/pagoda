#ifndef PAGODA_PROCEDURAL_OBJECTS_CLIP_GEOMETRY_H_
#define PAGODA_PROCEDURAL_OBJECTS_CLIP_GEOMETRY_H_

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class ClipGeometry : public objects::ProceduralOperation
{
	public:
	static const std::string inputGeometry;
	static const std::string frontGeometry;
	static const std::string backGeometry;
	static const char* name;

	ClipGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~ClipGeometry();

	void SetParameters(graph::ExecutionArgumentCallback* cb) override;

	const std::string& GetOperationName() const override;

	protected:
	void DoWork() override;
};
}  // namespace pagoda::geometry::operations

#endif
