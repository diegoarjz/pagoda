#include "procedural_component_system_base.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

namespace pagoda
{
ProceduralComponentSystemBase::ProceduralComponentSystemBase(const std::string &name) : m_systemName(name)
{
	LOG_TRACE(Core, "Creating ProceduralComponentSystem: " << m_systemName.c_str());
}

ProceduralComponentSystemBase::~ProceduralComponentSystemBase()
{
	LOG_TRACE(Core, "Destroying ProceduralComponentSystem: " << m_systemName.c_str());
}

std::string ProceduralComponentSystemBase::GetComponentSystemTypeName() const { return m_systemName; }
}  // namespace pagoda
