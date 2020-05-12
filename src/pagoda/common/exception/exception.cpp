#include "exception.h"

#include "pagoda/common/debug/assertions.h"

namespace pagoda::common::exception
{
Exception::Exception(const std::string exceptionMessage) : m_message(exceptionMessage)
{ /*DBG_ASSERT(false);*/
}

const std::string& Exception::What() const { return m_message; }
}  // namespace pagoda::common::exception
