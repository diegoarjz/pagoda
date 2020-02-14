#ifndef SELECTOR_PROCEDURAL_OPERATION_OBJECT_INTERFACE_H_
#define SELECTOR_PROCEDURAL_OPERATION_OBJECT_INTERFACE_H_

#include "procedural_object_mask.h"

#include <list>
#include <string>

namespace selector
{
class ProceduralOperationObjectInterface
{
public:
	ProceduralOperationObjectInterface(const std::string& name);

	const std::string& Name() const { return interface_name; }

	bool Accepts(ProceduralObjectPtr procedural_object);
	bool AddProceduralObject(ProceduralObjectPtr procedural_object);
	bool HasProceduralObjects() const { return !procedural_objects.empty(); }
	ProceduralObjectPtr GetFrontProceduralObject();
	ProceduralObjectPtr GetAndPopProceduralObject();

private:
	std::string interface_name;
	std::list<ProceduralObjectPtr> procedural_objects;
};  // class ProceduralOperationObjectInterface
}  // namespace selector

#endif
