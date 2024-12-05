#pragma once

#include "imgui.h"

#include <magic_enum/magic_enum.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <string>
#include <vector>

namespace pgframes::widgets {
template <class T>
bool fieldFor(const std::string &identifier, T &value, bool modifiable);

template <>
bool fieldFor<unsigned int>(const std::string &identifier, unsigned int &value, bool modifiable);

template <>
bool fieldFor<int>(const std::string &identifier, int &value, bool modifiable);

template <>
bool fieldFor<float>(const std::string &identifier, float &value,
                     bool modifiable);

template <>
bool fieldFor<bool>(const std::string &identifier, bool &value,
                    bool modifiable);

template <>
bool fieldFor<std::string>(const std::string &identifier, std::string &value,
                           bool modifiable);

template <typename T>
bool enumFieldFor(const std::string &identifier, T &value, bool modifiable,
                  const T &dflt) {
  const auto &values = magic_enum::enum_values<T>();

  std::vector<std::string> names;
  names.reserve(values.size());
  std::transform(
      values.begin(), values.end(), std::back_inserter(names),
      [](const T &v) { return std::string(magic_enum::enum_name(v)); });
  const std::string selectedName = std::string(magic_enum::enum_name(value));

  bool changed = false;
  if (ImGui::BeginCombo(fmt::format("##{}", identifier).c_str(),
                        selectedName.c_str())) {
    for (std::size_t i = 0; i < names.size(); ++i) {
      const auto enumName = names[i];
      const bool isSelected = enumName == selectedName;

      if (ImGui::Selectable(enumName.c_str(), isSelected)) {
        if (modifiable) {
          changed = true;
          value = magic_enum::enum_cast<T>(enumName).value_or(dflt);
        }
      }
    }
    ImGui::EndCombo();
  }

  return changed;
}

template <typename Getter, typename Setter>
bool field(const std::string &label, const std::string &identifier,
           Getter getValue, Setter setValue, bool modifiable = true) {
  int totalWidth = ImGui::GetContentRegionAvail().x * 0.5;

  ImGui::PushItemWidth(totalWidth);
  ImGui::Text("%s", label.c_str());
  ImGui::SameLine(totalWidth);

  auto value = getValue();
  ImGui::PushItemWidth(totalWidth);
  if (fieldFor(identifier, value, modifiable)) {
    setValue(value);
    return true;
  }
  return false;
}
} // namespace pgframes::widgets
