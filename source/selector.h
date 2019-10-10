#ifndef SELECTOR_SELECTOR_H_
#define SELECTOR_SELECTOR_H_

#include "procedural_graph/node_factory.h"
#include "procedural_objects/operation_factory.h"
#include "procedural_objects/procedural_object_system.h"

namespace selector
{
class Selector
{
public:
	Selector();
	virtual ~Selector();

	ProceduralObjectSystemPtr GetProceduralObjectSystem();

	OperationFactoryPtr GetOperationFactory();

	NodeFactoryPtr GetNodeFactory();

private:
	ProceduralObjectSystemPtr m_proceduralObjectSystem;
	OperationFactoryPtr m_operationFactory;
	NodeFactoryPtr m_nodeFactory;
};

}  // namespace selector

#endif
