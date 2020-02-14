#ifndef SELECTOR_PROCEDURAL_OBJECTS_EXTRACT_SCOPE_H_
#define SELECTOR_PROCEDURAL_OBJECTS_EXTRACT_SCOPE_H_

#include "procedural_operation.h"

namespace selector
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
}  // namespace selector
#endif

