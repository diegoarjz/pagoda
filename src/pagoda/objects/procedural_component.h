#ifndef PAGODA_PROCEDURAL_OBJECTS_PROCEDURAL_COMPONENT_H_
#define PAGODA_PROCEDURAL_OBJECTS_PROCEDURAL_COMPONENT_H_

#include <memory>
#include <string>

namespace pagoda::objects
{
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;
using ProceduralObjectWeakPtr = std::weak_ptr<ProceduralObject>;

class ProceduralComponent
{
	public:
	virtual ~ProceduralComponent(){};

	void SetParentObject(ProceduralObjectPtr parent);
	ProceduralObjectPtr GetParentObject();

	virtual std::string GetType() const = 0;

	protected:
	ProceduralComponent(){};
	ProceduralObjectWeakPtr parent_object;
};  // class ProceduralComponent

using ProceduralComponentPtr = std::shared_ptr<ProceduralComponent>;
using ProceduralComponentWeakPtr = std::weak_ptr<ProceduralComponent>;

}  // namespace pagoda::objects

#endif
