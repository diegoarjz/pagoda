#include "exception.h"

#include "assertions.h"

namespace selector
{
Exception::Exception(const std::string exceptionMessage) : m_message(exceptionMessage)
{ /*DBG_ASSERT(false);*/
}

const std::string& Exception::What() const { return m_message; }
}  // namespace selector
