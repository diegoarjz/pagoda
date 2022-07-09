#pragma once

#include "pagoda/objects/interface.h"

#include <string>

namespace pagoda::objects
{
class InterfaceCallback
{
	public:
	virtual void InputInterface(InterfacePtr& i, const std::string& name, const std::string& label,
	                            Interface::Arity arity) = 0;
	virtual void OutputInterface(InterfacePtr& i, const std::string& name, const std::string& label,
	                             Interface::Arity arity) = 0;
};
}  // namespace pagoda::objects
