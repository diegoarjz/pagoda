#ifndef PAGODA_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_H_
#define PAGODA_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_H_

#include "common/factory.h"
#include "dynamic_value/builtin_class.h"
#include "procedural_component.h"
#include "procedural_operation_object_interface.h"

#include <bitset>
#include <list>
#include <string>
#include <unordered_map>

namespace pagoda
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
class ProceduralOperation : public std::enable_shared_from_this<ProceduralOperation>, public BuiltinClass
{
public:
	static const TypeInfoPtr s_typeInfo;

	ProceduralOperation(ProceduralObjectSystemPtr proceduralObjectSystem);
	virtual ~ProceduralOperation() {}

	/**
	 * Executes the \c ProceduralOperation.
	 */
	void Execute();

	/**
	 * Pushes the given \p procedural_object to the input interface with the given \p interface.
	 */
	bool PushProceduralObject(const std::string& interface, ProceduralObjectPtr procedural_object);
	/**
	 * Pops a \c ProceduralObject from the output interface with the given \p interface
	 */
	ProceduralObjectPtr PopProceduralObject(const std::string& interface) const;

	std::string ToString() const override;

	void AcceptVisitor(ValueVisitorBase& visitor) override;

protected:
	/**
	 * Performs the operation work.
	 */
	virtual void DoWork() = 0;

	/**
	 * Registers the \c DynamicValueBase objects that will be used during the execution.
	 */
	void RegisterValues(const std::unordered_map<std::string, DynamicValueBasePtr>& values);
	/**
	 * Updates the value of the \c DynamicValueBase with the \p valueName if it is an expression.
	 */
	void UpdateValue(const std::string& valueName);
	/**
	 * Returns the value of the \c DynamicValueBase with the \p valueName.
	 */
	DynamicValueBasePtr GetValue(const std::string& valueName);

	void CreateInputInterface(const std::string& interfaceName);
	void CreateOutputInterface(const std::string& interfaceName);
	std::shared_ptr<ProceduralObject> GetInputProceduralObject(const std::string& interfaceName);
	bool HasInput(const std::string& interfaceName) const;
	std::shared_ptr<ProceduralObject> CreateOutputProceduralObject(const std::string& interfaceName);
	ProceduralObjectSystemPtr m_proceduralObjectSystem;

private:
	using InterfaceContainer_t = std::unordered_map<std::string, std::unique_ptr<ProceduralOperationObjectInterface>>;

	InterfaceContainer_t input_interfaces;
	InterfaceContainer_t output_interfaces;

};  // class ProceduralOperation
}  // namespace pagoda
#endif
