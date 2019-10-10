#ifndef SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_H_
#define SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_H_

#include "procedural_component.h"
#include "procedural_operation_object_interface.h"

#include "common/factory.h"
#include "parameter/context.h"
#include "parameter/parameterizable.h"

#include <bitset>
#include <list>
#include <string>
#include <unordered_map>

namespace selector
{
class ProceduralObjectSystem;
using ProceduralObjectSystemPtr = std::shared_ptr<ProceduralObjectSystem>;

/**
 * @brief Base class for a procedural operation.
 *
 * Makes no assumptions about the schedulling system, making it agnostic to the
 * procedural modelling methodology (e.g., graph based or shape grammars)
 *
 * Has input and output \c ProceduralOperationObjectInterface which is used to pass
 * input and output procedural objects.
 */
class ProceduralOperation : public IParameterizable, public std::enable_shared_from_this<ProceduralOperation>
{
public:
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

	/**
	 * Gets a \c Parameter from this \c ProceduralOperation.
	 */
	Parameter GetParameter(const std::string& parameterName) const override;

	/**
	 * Sets a \c Parameter in this \c ProceduralOperation.
	 */
	void SetParameter(const std::string& parameterName, const Parameter& parameter) override;

	/**
	 * Gets the name of all \c Parameter in this \c ProceduralOperation.
	 */
	std::unordered_set<std::string> GetParameterNameList() const override;

	/**
	 * Gets all the \c Parameter in this \c ProceduralOperation.
	 */
	std::unordered_map<std::string, Parameter> GetParameters() const override;

	/**
	 * Resolves a \c Variable within the hierarchy of \c IParameterizable.
	 */
	Parameter ResolveVariable(const Variable& v) const override;

protected:
	void CreateInputInterface(const InterfaceName& interfaceName);
	void CreateOutputInterface(const InterfaceName& interfaceName);
	std::shared_ptr<ProceduralObject> GetInputProceduralObject(const InterfaceName& interfaceName);
	bool HasInput(const InterfaceName& interfaceName) const;
	std::shared_ptr<ProceduralObject> CreateOutputProceduralObject(const InterfaceName& interfaceName);

	std::shared_ptr<Context> m_parameterContext;  ///< The parameter \c Context for the \c ProceduralOperation
	ProceduralObjectSystemPtr m_proceduralObjectSystem;

private:
	using InterfaceContainer_t =
	    std::unordered_map<InterfaceName, std::unique_ptr<ProceduralOperationObjectInterface>, InterfaceNameHasher>;

	InterfaceContainer_t input_interfaces;
	InterfaceContainer_t output_interfaces;

};  // class ProceduralOperation
}  // namespace selector
#endif
