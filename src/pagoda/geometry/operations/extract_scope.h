#ifndef PAGODA_PROCEDURAL_OBJECTS_EXTRACT_SCOPE_H_
#define PAGODA_PROCEDURAL_OBJECTS_EXTRACT_SCOPE_H_

#include <pagoda/procedural_objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class ExtractScope : public ProceduralOperation
{
public:
	static const std::string inputGeometry;
	static const std::string outputGeometry;
	static const char* name;

	ExtractScope(ProceduralObjectSystemPtr objectSystem);
	virtual ~ExtractScope();

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
#endif

