#pragma once

#include <string>

namespace pgframes::widgets {

bool TextInput(const char *label, std::string &text, bool modifiable = true);

bool MultiLineTextInput(const char *label, std::string &text, bool modifiable = true);

} // namespace pgframes::widgets
