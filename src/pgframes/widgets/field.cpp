#include "field.h"

#include "pgframes/widgets/text_input.h"

#include <fmt/format.h>
#include <magic_enum/magic_enum.hpp>

namespace pgframes::widgets {
template <>
bool fieldFor<std::string>(const std::string &identifier, std::string &value,
                           bool modifiable) {
  return widgets::TextInput(fmt::format("##{}", identifier).c_str(), value,
                            modifiable);
}

template <>
bool fieldFor<float>(const std::string &identifier, float &value,
                     bool modifiable) {
  ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
  if (!modifiable) {
    flags |= ImGuiInputTextFlags_ReadOnly;
  }
  return ImGui::InputFloat(fmt::format("##{}", identifier).c_str(), &value,
                           0.0f, 0.0f, nullptr, flags);
}

template <>
bool fieldFor<int>(const std::string &identifier, int &value, bool modifiable) {
  ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
  if (!modifiable) {
    flags |= ImGuiInputTextFlags_ReadOnly;
  }
  return ImGui::InputInt(fmt::format("##{}", identifier).c_str(), &value, 0, 0,
                         flags);
}

template <>
bool fieldFor<unsigned int>(const std::string &identifier, unsigned int &value,
                            bool modifiable) {
  ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
  if (!modifiable) {
    flags |= ImGuiInputTextFlags_ReadOnly;
  }
  return ImGui::InputScalar(fmt::format("##{}", identifier).c_str(),
                            ImGuiDataType_U32, &value, nullptr, nullptr,
                            nullptr, flags);
}

template <>
bool fieldFor<bool>(const std::string &identifier, bool &value,
                    bool modifiable) {
  return modifiable &&
         ImGui::Checkbox(fmt::format("##{}", identifier).c_str(), &value);
}
} // namespace pgframes::widgets
