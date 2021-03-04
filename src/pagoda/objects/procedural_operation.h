#ifndef PAGODA_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_H_
#define PAGODA_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_H_

#include "procedural_component.h"

#include <pagoda/common/factory.h>
#include <pagoda/dynamic/builtin_class.h>
#include <cstddef>
#include <memory>

#include <boost/signals2.hpp>

#include <list>
#include <string>
#include <unordered_map>

namespace pagoda::dynamic
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;
}  // namespace pagoda::dynamic

namespace pagoda::objects
{
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;
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
class ProceduralOperation : public dynamic::BuiltinClass
{
	public:
	static const dynamic::TypeInfoPtr s_typeInfo;

	ProceduralOperation(ProceduralObjectSystemPtr proceduralObjectSystem);
	virtual ~ProceduralOperation() {}

	/**
	 * Executes the \c ProceduralOperation.
	 */
	void Execute();

	bool HasInputInterface(const std::string& name) const;
	bool HasOutputInterface(const std::string& name) const;

	void LinkInputInterface(const std::string& inputInterface, const std::string& outputInterface,
	                        const std::shared_ptr<ProceduralOperation>& op);

	std::string ToString() const override;

	/**
	 * Returns the name of this operation.
	 */
	virtual const std::string& GetOperationName() const = 0;

	void AcceptVisitor(dynamic::ValueVisitorBase& visitor) override;

	using InterfaceHandler_t = void(ProceduralOperation*, const std::string&, ProceduralObjectPtr);
	void OnOutputObjectCreated(const std::string& interface, const std::function<InterfaceHandler_t>& handler);
	void OnProgress(const std::function<void(const std::size_t&, const std::size_t)>& handler);
	void OnNeedsUpdate(const std::function<void(ProceduralOperation*)>& handler);

	protected:
	/**
	 * Performs the operation work.
	 */
	virtual void DoWork() = 0;

	/**
	 * Registers the \c DynamicValueBase objects that will be used during the execution.
	 */
	void RegisterValues(const std::unordered_map<std::string, dynamic::DynamicValueBasePtr>& values);
	/**
	 * Updates the value of the \c DynamicValueBase with the \p valueName if it is an expression.
	 */
	void UpdateValue(const std::string& valueName);
	/**
	 * Returns the value of the \c DynamicValueBase with the \p valueName.
	 */
	dynamic::DynamicValueBasePtr GetValue(const std::string& valueName);

	void CreateInputInterface(const std::string& interfaceName);
	void CreateOutputInterface(const std::string& interfaceName);
	std::shared_ptr<ProceduralObject> GetInputProceduralObject(const std::string& interfaceName);
	bool HasInput(const std::string& interfaceName) const;
	std::shared_ptr<ProceduralObject> CreateOutputProceduralObject(const std::string& interfaceName);

	/**
	 * Creates a \c ProceduralObject in the output interface named \p interfaceName, copying all components present in
	 * \p base.
	 */
	std::shared_ptr<ProceduralObject> CreateOutputProceduralObject(std::shared_ptr<ProceduralObject>& base,
	                                                               const std::string& interfaceName);

	ProceduralObjectSystemPtr m_proceduralObjectSystem;

	private:
	/**
	 * Pushes the given \p procedural_object to the input interface with the given \p interface.
	 */
	bool PushProceduralObject(const std::string& interface, ProceduralObjectPtr procedural_object);
	/**
	 * Pops a \c ProceduralObject from the output interface with the given \p interface
	 */
	ProceduralObjectPtr PopProceduralObject(const std::string& interface);

	struct Interface
	{
		std::list<ProceduralObjectPtr> m_objects;
		boost::signals2::signal<InterfaceHandler_t> m_signal;
		std::list<std::shared_ptr<ProceduralOperation>> m_operations;

		void Add(ProceduralObjectPtr o) { m_objects.push_back(o); }
		ProceduralObjectPtr GetFront()
		{
			if (m_objects.empty()) {
				return nullptr;
			}
			auto object = m_objects.front();
			m_objects.pop_front();
			return object;
		}
		bool HasObjects() const { return !m_objects.empty(); }
	};

	using InterfaceContainer_t = std::unordered_map<std::string, Interface>;

	boost::signals2::signal<void(const std::size_t&, const std::size_t)> m_progressHandlers;
	boost::signals2::signal<void(ProceduralOperation*)> m_needUpdateHandlers;
	bool m_needsUpdate;

	InterfaceContainer_t m_inputInterfaces;
	InterfaceContainer_t m_outputInterfaces;

	std::size_t m_pendingObjects;
	std::size_t m_processedObjects;

};  // class ProceduralOperation
}  // namespace pagoda::objects
#endif
