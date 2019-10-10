#ifndef SELECTOR_PROCEDURAL_OPERATION_OBJECT_INTERFACE_H_
#define SELECTOR_PROCEDURAL_OPERATION_OBJECT_INTERFACE_H_

#include "procedural_object_mask.h"

#include <string>
#include <list>

namespace selector
{
struct InterfaceName
{
	std::string name;
	uint16_t offset;

	InterfaceName(const std::string& name, uint16_t offset) : name(name), offset(offset) {}

	InterfaceName Offset(uint16_t o) const { return InterfaceName(name, o); }
	bool operator==(const InterfaceName& other) const { return name == other.name && offset == other.offset; }

	std::string ToString() const { return name + std::to_string(offset); }
};  // struct InterfaceName

struct InterfaceNameHasher
{
	std::size_t operator()(const InterfaceName& key) const
	{
		return std::hash<std::string>()(key.name) ^ (std::hash<uint16_t>()(key.offset) << 1);
	}
};  // struct InterfaceNameHasher

class ProceduralOperationObjectInterface
{
public:
	ProceduralOperationObjectInterface(const InterfaceName& name);

	const InterfaceName& Name() const { return interface_name; }

	bool Accepts(ProceduralObjectPtr procedural_object);
	bool AddProceduralObject(ProceduralObjectPtr procedural_object);
	bool HasProceduralObjects() const { return !procedural_objects.empty(); }
	ProceduralObjectPtr GetFrontProceduralObject();
	ProceduralObjectPtr GetAndPopProceduralObject();

private:
	InterfaceName interface_name;
	std::list<ProceduralObjectPtr> procedural_objects;
};  // class ProceduralOperationObjectInterface
}  // namespace selector

#endif
