#ifndef SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OBJECT_H_
#define SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OBJECT_H_

#include "dynamic_value/builtin_class.h"
#include "geometry_core/geometry.h"
#include "procedural_component.h"

#include <bitset>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace selector
{
class Context;
using ContextPtr = std::shared_ptr<Context>;

class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

class ProceduralObject : public std::enable_shared_from_this<ProceduralObject>, public BuiltinClass
{
public:
	static const TypeInfoPtr s_typeInfo;

	ProceduralObject();
	virtual ~ProceduralObject();

	std::string ToString() const override;

	void AcceptVisitor(ValueVisitorBase& visitor) override;

};  // class ProceduralObject

using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;
using ProceduralObjectWeakPtr = std::weak_ptr<ProceduralObject>;

}  // namespace selector

#endif
