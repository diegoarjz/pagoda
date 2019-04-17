#ifndef SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_H_
#define SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_H_

#include "procedural_component.h"

#include "common/factory.h"
#include "parameter/context.h"

#include <bitset>
#include <list>
#include <string>
#include <unordered_map>

namespace selector
{
class GeometrySystem;
class ProceduralObjectSystem;

using ProceduralObjectMask = std::bitset<static_cast<size_t>(ComponentType::MaxComponents)>;

struct InterfaceName
{
	const char* name;
	uint16_t offset;

	InterfaceName(const char* name, uint16_t offset) : name(name), offset(offset) {}

	InterfaceName Offset(uint16_t o) const { return InterfaceName(name, o); }
	bool operator==(const InterfaceName& other) const { return name == other.name && offset == other.offset; }
};  // struct InterfaceName

struct InterfaceNameHasher
{
	std::size_t operator()(const InterfaceName& key) const
	{
		return std::hash<const char*>()(key.name) ^ (std::hash<uint16_t>()(key.offset) << 1);
	}
};  // struct InterfaceNameHasher

struct OperationExecutionContext
{
	std::shared_ptr<Context> parameter_context;
	std::shared_ptr<ProceduralObjectSystem> procedural_object_system;
	std::shared_ptr<GeometrySystem> geometry_system;
};

class ProceduralOperationObjectInterface
{
public:
	ProceduralOperationObjectInterface(const InterfaceName& name, const ProceduralObjectMask& mask);

	const InterfaceName& Name() const { return interface_name; }
	const ProceduralObjectMask& Mask() const { return interface_mask; }

	bool Accepts(ProceduralObjectPtr procedural_object);
	bool AddProceduralObject(ProceduralObjectPtr procedural_object);
	bool HasProceduralObjects() const { return !procedural_objects.empty(); }
	ProceduralObjectPtr GetFrontProceduralObject();
	ProceduralObjectPtr GetAndPopProceduralObject();

private:
	InterfaceName interface_name;
	ProceduralObjectMask interface_mask;
	std::list<ProceduralObjectPtr> procedural_objects;
};  // class ProceduralOperationObjectInterface

/**
 * @brief Base class for a procedural operation.
 *
 * Makes no assumptions about the schedulling system, making it agnostic to the
 * procedural modelling methodology (e.g., graph based or shape grammars)
 *
 * Has input and output \c ProceduralOperationObjectInterface which is used to pass
 * input and output procedural objects.
 */
class ProceduralOperation : public Factory<ProceduralOperation>
{
public:
	virtual ~ProceduralOperation() {}

	/**
	 * Executes the \c ProceduralOperation.
	 *
	 * Sub classes must implement this.
	 */
	virtual void Execute() = 0;
	virtual void SetExecutionContext(std::shared_ptr<OperationExecutionContext> context) { exection_context = context; }

	/**
	 * Pushes the given \p procedural_object to the input interface with the given \p interface.
	 */
	bool PushProceduralObject(InterfaceName interface, ProceduralObjectPtr procedural_object);
	/**
	 * Pops a \c ProceduralObject from the output interface with the given \p interface
	 */
	ProceduralObjectPtr PopProceduralObject(const InterfaceName& interface) const;

	/**
	 * Sets the \c Parameter to the given \p parameter.
	 *
	 * Overrides the one given by the context. The \p name must exist among the \c ProceduralOperation parameters.
	 */
	void SetParameter(const std::string& name, std::shared_ptr<ParameterBase> parameter);
	/**
	 * Reverts the parameter with the given \p name to the one given in the context.
	 */
	void UnsetParameter(const std::string& name);

	virtual bool ValidateProceduralObject(const InterfaceName& interface, ProceduralObjectPtr proceduralObject)
	{
		return true;
	}

protected:
	virtual std::shared_ptr<ParameterBase> GetParameter(const std::string& name) = 0;

	void CreateInputInterface(const InterfaceName& interfaceName, const ProceduralObjectMask& mask);
	void CreateOutputInterface(const InterfaceName& interfaceName, const ProceduralObjectMask& mask);
	std::shared_ptr<ProceduralObject> GetInputProceduralObject(const InterfaceName& interfaceName);
	bool HasInput(const InterfaceName& interfaceName) const;
	std::shared_ptr<ProceduralObject> CreateOutputProceduralObject(const InterfaceName& interfaceName);

	std::shared_ptr<OperationExecutionContext> exection_context;

private:
	using InterfaceContainer_t =
	    std::unordered_map<InterfaceName, std::unique_ptr<ProceduralOperationObjectInterface>, InterfaceNameHasher>;

	InterfaceContainer_t input_interfaces;
	InterfaceContainer_t output_interfaces;
};  // class ProceduralOperation
}  // namespace selector
#endif
