#include "unimplemented.h"

namespace selector
{
Unimplemented::Unimplemented(const char *file, const char *function, uint32_t line)
    : Exception("Unimplemented function: " + std::string(function) + " at " + std::string(file) + ":" +
                std::to_string(line))
{
}
}  // namespace selector
