#ifndef SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_H_
#define SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_H_

#include "../dynamic_value/class_base.h"
#include "../dynamic_value/dynamic_value_base.h"
#include "common/factory.h"
#include "procedural_component.h"
#include "procedural_operation_object_interface.h"

#include <bitset>
#include <list>
#include <string>
#include <unordered_map>

namespace selector
{
class ProceduralObjectSystem;
using ProceduralObjectSystemPtr = std::shared_ptr<ProceduralObjectSystem>;

class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

/**
 * @brief Base class for a procedural operation.
 *
 * Makes no assumptions about the schedulling system, making it agnostic to the
 * procedural modelling methodology (e.g., graph based or shape grammars)
 *
 * Has input and output \c ProceduralOperationObjectInterface which is used to pass
 * input and output procedural objects.
 */
class ProceduralOperation : public std::enable_shared_from_this<ProceduralOperation>,
                            public DynamicValueBase,
                            public ClassBase
{
public:
	static const TypeInfoPtr s_typeInfo;

	ProceduralOperation(ProceduralObjectSystemPtr proceduralObjectSystem);
	virtual ~ProceduralOperation() {}

	/**
	 * Executes the \c ProceduralOperation.
	 *
	 * Sub classes must implement this.
	 */
	virtual void Execute() = 0;

	/**
	 * Pushes the given \p procedural_object to the input interface with the given \p interface.
	 */
	bool PushProceduralObject(InterfaceName interface, ProceduralObjectPtr procedural_object);
	/**
	 * Pops a \c ProceduralObject from the output interface with the given \p interface
	 */
	ProceduralObjectPtr PopProceduralObject(const InterfaceName& interface) const;

	virtual bool ValidateProceduralObject(const InterfaceName& interface, ProceduralObjectPtr proceduralObject)
	{
		return true;
	}

	std::string ToString() const override;

	void AcceptVisitor(ValueVisitorBase& visitor) override;

protected:
	void CreateInputInterface(const InterfaceName& interfaceName);
	void CreateOutputInterface(const InterfaceName& interfaceName);
	std::shared_ptr<ProceduralObject> GetInputProceduralObject(const InterfaceName& interfaceName);
	bool HasInput(const InterfaceName& interfaceName) const;
	std::shared_ptr<ProceduralObject> CreateOutputProceduralObject(const InterfaceName& interfaceName);
	ProceduralObjectSystemPtr m_proceduralObjectSystem;

private:
	using InterfaceContainer_t =
	    std::unordered_map<InterfaceName, std::unique_ptr<ProceduralOperationObjectInterface>, InterfaceNameHasher>;

	InterfaceContainer_t input_interfaces;
	InterfaceContainer_t output_interfaces;

};  // class ProceduralOperation
}  // namespace selector
#endif
