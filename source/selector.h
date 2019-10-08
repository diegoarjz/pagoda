#ifndef SELECTOR_SELECTOR_H_
#define SELECTOR_SELECTOR_H_

#include "procedural_objects/procedural_object_system.h"

namespace selector
{
class Selector
{
public:
	Selector& GetInstance();

	ProceduralObjectSystemPtr GetProceduralObjectSystem();

private:
	Selector();
	virtual ~Selector();

	ProceduralObjectSystemPtr m_proceduralObjectSystem;
};

}  // namespace selector

#endif
