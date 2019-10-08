#ifndef SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OBJECT_H_
#define SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OBJECT_H_

#include "procedural_component.h"

#include "geometry_core/geometry.h"

#include "parameter/parameter.h"
#include "parameter/parameterizable.h"

#include <bitset>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace selector
{
class Context;
using ContextPtr = std::shared_ptr<Context>;

using ProceduralObjectMask = std::bitset<static_cast<size_t>(ComponentType::MaxComponents)>;

class ProceduralObject : public std::enable_shared_from_this<ProceduralObject>, public IParameterizable
{
public:
	ProceduralObject();
	virtual ~ProceduralObject();

	const std::vector<ProceduralComponentWeakPtr>& GetComponents() const { return components; }
	template<class T>
	std::shared_ptr<T> GetComponent()
	{
		return std::dynamic_pointer_cast<T>(components[static_cast<size_t>(T::GetType())].lock());
	}

	void SetComponent(const ComponentType& type, std::shared_ptr<ProceduralComponent> component);

	template<class T>
	void SetComponent(std::shared_ptr<T> component)
	{
		CRITICAL_ASSERT_MSG(component != nullptr, "Setting a null component");
		components[static_cast<size_t>(T::GetType())] = component;
		component->SetParentObject(shared_from_this());
		procedural_object_mask.set(static_cast<size_t>(T::GetType()), true);
	}

	template<class T>
	ProceduralComponentPtr RemoveComponent()
	{
		auto component = components[static_cast<size_t>(T::GetType())];
		component.lock()->SetParentObject(nullptr);
		components[static_cast<size_t>(T::GetType())].reset();
		procedural_object_mask.set(static_cast<size_t>(T::GetType()), false);
		return component.lock();
	}

	const ProceduralObjectMask& Mask() const { return procedural_object_mask; }

	/**
	 * Gets a \c Parameter from this \c ProceduralObject.
	 */
	Parameter GetParameter(const std::string& parameterName) const override;

	/**
	 * Sets a \c Parameter in this \c ProceduralObject.
	 */
	void SetParameter(const std::string& parameterName, const Parameter& parameter) override;

	/**
	 * Gets the name of all \c Parameter in this \c ProceduralObject.
	 */
	std::unordered_set<std::string> GetParameterNameList() const override;

	/**
	 * Gets all the \c Parameter in this \c ProceduralObject.
	 */
	std::unordered_map<std::string, Parameter> GetParameters() const override;

	/**
	 * Resolves a \c Variable within the hierarchy of \c IParameterizable.
	 */
	Parameter ResolveVariable(const Variable& v) const override;

private:
	// Procedural Components
	std::vector<ProceduralComponentWeakPtr> components;
	ProceduralObjectMask procedural_object_mask;

	ContextPtr m_context;  ///< The parameter \c Context for this \c ProceduralObject
};                         // class ProceduralObject

using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;
using ProceduralObjectWeakPtr = std::weak_ptr<ProceduralObject>;

}  // namespace selector

#endif
