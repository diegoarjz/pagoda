#ifndef PAGODA_PROCEDURAL_OBJECTS_PROCEDURAL_OBJECT_H_
#define PAGODA_PROCEDURAL_OBJECTS_PROCEDURAL_OBJECT_H_

#include "procedural_component.h"

#include <pagoda/dynamic_value/builtin_class.h>
#include <pagoda/geometry/core/geometry.h>

#include <bitset>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace pagoda
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;
}  // namespace pagoda

namespace pagoda::objects
{
class Context;
using ContextPtr = std::shared_ptr<Context>;

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

}  // namespace pagoda::objects

#endif
