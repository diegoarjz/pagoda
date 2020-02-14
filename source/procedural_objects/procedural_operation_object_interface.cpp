#include "procedural_operation_object_interface.h"

#include "procedural_object.h"

namespace selector
{
ProceduralOperationObjectInterface::ProceduralOperationObjectInterface(const std::string& name) : interface_name(name)
{
}

bool ProceduralOperationObjectInterface::Accepts(ProceduralObjectPtr procedural_object) { return true; }

bool ProceduralOperationObjectInterface::AddProceduralObject(ProceduralObjectPtr procedural_object)
{
	START_PROFILE;

	if (Accepts(procedural_object))
	{
		procedural_objects.push_back(procedural_object);

		return true;
	}
	return false;
}

ProceduralObjectPtr ProceduralOperationObjectInterface::GetFrontProceduralObject()
{
	START_PROFILE;

	if (procedural_objects.size() > 0)
	{
		auto object = procedural_objects.front();
		return object;
	}
	else
	{
		return nullptr;
	}
}

ProceduralObjectPtr ProceduralOperationObjectInterface::GetAndPopProceduralObject()
{
	START_PROFILE;

	if (procedural_objects.size() > 0)
	{
		auto object = procedural_objects.front();
		procedural_objects.pop_front();
		return object;
	}
	else
	{
		return nullptr;
	}
}

}  // namespace selector
