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

class ProceduralObject : public std::enable_shared_from_this<ProceduralObject>, public IParameterizable
{
public:
	ProceduralObject();
	virtual ~ProceduralObject();

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
	ContextPtr m_context;  ///< The parameter \c Context for this \c ProceduralObject
};                         // class ProceduralObject

using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;
using ProceduralObjectWeakPtr = std::weak_ptr<ProceduralObject>;

}  // namespace selector

#endif
