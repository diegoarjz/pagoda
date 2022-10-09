#ifndef PAGODA_PROCEDURAL_OBJECTS_PROCEDURAL_OBJECT_H_
#define PAGODA_PROCEDURAL_OBJECTS_PROCEDURAL_OBJECT_H_


#include <pagoda/dynamic/builtin_class.h>
#include <pagoda/geometry/core/geometry.h>

#include <bitset>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace pagoda::dynamic
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;
}  // namespace pagoda::dynamic

namespace pagoda::objects
{

using ProceduralObjectSystemWeakPtr = std::weak_ptr<class ProceduralObjectSystem>;

using ProceduralObjectPtr = std::shared_ptr<class ProceduralObject>;
using ProceduralObjectWeakPtr = std::weak_ptr<class ProceduralObject>;

class ProceduralObject :  public dynamic::BuiltinClass
{
	public:
	static const dynamic::TypeInfoPtr s_typeInfo;

	ProceduralObject(const ProceduralObjectSystemWeakPtr& owner);
	virtual ~ProceduralObject();

  ProceduralObjectPtr Clone();

	std::string ToString() const override;

	void AcceptVisitor(dynamic::ValueVisitorBase& visitor) override;

  private:

  ProceduralObjectSystemWeakPtr m_owner;

};  // class ProceduralObject

}  // namespace pagoda::objects

#endif
