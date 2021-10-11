#pragma once

#include "interface.h"
#include "interface_callback.h"
#include "procedural_component.h"

#include <pagoda/common/factory.h>
#include <pagoda/dynamic/builtin_class.h>
#include <boost/signals2.hpp>

#include <cstddef>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace pagoda
{
namespace graph
{
class ExecutionArgumentCallback;
}

namespace dynamic
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;
}  // namespace dynamic
}  // namespace pagoda

namespace pagoda::objects
{
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;
class ProceduralObjectSystem;
using ProceduralObjectSystemPtr = std::shared_ptr<ProceduralObjectSystem>;
class InterfaceCallback;

/**
 * @brief Base class for a procedural operation.
 *
 * Makes no assumptions about the schedulling system, making it agnostic to the
 * procedural modelling methodology (e.g., graph based or shape grammars)
 *
 * Has input and output \c ProceduralOperationObjectInterface which is used to
 * pass input and output procedural objects.
 */
class ProceduralOperation : public dynamic::BuiltinClass
{
	public:
	static const dynamic::TypeInfoPtr s_typeInfo;

	ProceduralOperation(ProceduralObjectSystemPtr proceduralObjectSystem);
	virtual ~ProceduralOperation()
	{
	}

	/**
	 * Executes the \c ProceduralOperation.
	 */
	void Execute();

	std::string ToString() const override;

	/**
	 * Returns the name of this operation.
	 */
	virtual const std::string& GetOperationName() const = 0;

	void AcceptVisitor(dynamic::ValueVisitorBase& visitor) override;

	////////////////////////////////////////////////////////////
	/// \name Interfaces
	////////////////////////////////////////////////////////////

	virtual void Interfaces(InterfaceCallback* cb) = 0;

	/**
	 * Iterates over each input interface, passing its name to \p f.
	 */
	void ForEachInputInterface(const std::function<void(const std::string&)>& f);
	/**
	 * Iterates over each output interface, passing its name to \p f.
	 */
	void ForEachOutputInterface(const std::function<void(const std::string&)>& f);

	bool HasInputInterface(const std::string& name) const;
	bool HasOutputInterface(const std::string& name) const;

	/**
	 * Iterates over each operation parameter.
	 */
	void ForEachParameter(
	  const std::function<void(const std::string&,
	                           const dynamic::DynamicValueBasePtr& type)>& f);

	virtual void SetParameters(graph::ExecutionArgumentCallback* cb) = 0;

	protected:
	/**
	 * Performs the operation work.
	 */
	virtual void DoWork() = 0;

	/**
	 * Updates the value of the \c DynamicValueBase with the \p valueName if it is
	 * an expression.
	 */
	void UpdateValue(const std::string& valueName);
	/**
	 * Returns the value of the \c DynamicValueBase with the \p valueName.
	 */
	dynamic::DynamicValueBasePtr GetValue(const std::string& valueName);


	std::shared_ptr<ProceduralObject> CreateOutputProceduralObject();

	/**
	 * Creates a \c ProceduralObject in the output interface named \p
	 * interfaceName, copying all components present in \p base.
	 */
	std::shared_ptr<ProceduralObject> CreateOutputProceduralObject(
	  std::shared_ptr<ProceduralObject>& base);

	ProceduralObjectSystemPtr m_proceduralObjectSystem;

	private:
	boost::signals2::signal<void(const std::size_t&, const std::size_t)>
	  m_progressHandlers;
	boost::signals2::signal<void(ProceduralOperation*)> m_needUpdateHandlers;
	bool m_needsUpdate;

	std::size_t m_pendingObjects;
	std::size_t m_processedObjects;

	std::vector<std::string> m_parameterNames;

};  // class ProceduralOperation
}  // namespace pagoda::objects
