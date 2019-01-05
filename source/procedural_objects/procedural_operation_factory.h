#ifndef SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_FACTORY_H_
#define SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_FACTORY_H_

#include <memory>

namespace selector
{
class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;

enum class ProceduralOperationType
{
	Extrude,
	CreateRect,
	Triangulate,
	Max
};

class ProceduralOperationFactory
{
public:
	static ProceduralOperationPtr Create(ProceduralOperationType type);

};  // class ProceduralOperationFactory
}  // namespace selector

#endif
