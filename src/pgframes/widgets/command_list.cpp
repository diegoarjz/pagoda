#include "command_list.h"

#include "text_input.h"

#include <algorithm>

namespace pgframes::widgets {
namespace {
inline std::string lower(const std::string &in) {
  std::string s = in;
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); } // correct
  );
  return s;
}

bool Matches(const std::string &filter, const std::string &section,
             const std::string &command) {
  if (filter.empty()) {
    return true;
  }
  const auto concat = lower(section + "." + command);
  return concat.find(lower(filter)) != std::string::npos;
}
} // namespace

bool CommandList(const char *id, CommandListDelegate &delegate,
                 const ImVec2 &size) {

  static std::string emptyFilter;
  std::string &filter = emptyFilter;

  bool firstOpened = delegate.m_firstOpened;
  delegate.m_firstOpened = false;

  if (delegate.AllowFiltering()) {
    filter = delegate.FilterString();

    const auto contentRegion = ImGui::GetContentRegionAvail();
    ImGui::SetNextItemWidth(contentRegion.x);
    if (TextInput(("##" + std::string(id) + "_search").c_str(), filter)) {
      delegate.FilterString() = filter;
    }
    if (firstOpened) {
      ImGui::SetKeyboardFocusHere(-1);
    }
  }

  int selectedItem = -1;
  bool enterPressed = false;
  if (ImGui::IsWindowFocused()) {
    if (delegate.AllowKeyNavigation()) {
      if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
        delegate.SelectedItem() -= 1;
      }

      if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
        delegate.SelectedItem() += 1;
      }

      if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
        enterPressed = true;
      }

      if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        delegate.CloseRequested();
      }

      selectedItem = delegate.SelectedItem();
    }
  }

  int index = 0;
  if (ImGui::BeginChild(id, size, true)) {
    const auto &commandList = delegate.GetCommandList();

    const auto contentRegion = ImGui::GetContentRegionAvail();
    for (const auto &section : commandList) {

      bool sectionOutput = false;

      for (const auto &cmd : section.commands) {
        if (!Matches(filter, section.name, cmd.label)) {
          continue;
        }

        if (!sectionOutput) {
          if (!section.name.empty()) {
            ImGui::TextUnformatted(section.name.c_str());
          }
          sectionOutput = true;
        }

        auto buttonLabel =
          cmd.label + "##" + id + "_" + section.name + "_" + cmd.label;

        const bool itemIsSelected = index == selectedItem;
        if (itemIsSelected) {
          buttonLabel = "*" + buttonLabel;
        }

        if (ImGui::Button(buttonLabel.c_str(), ImVec2(contentRegion.x, 0)) ||
            (itemIsSelected && enterPressed)) {
          cmd.callback();
          delegate.FilterString().clear();
        }

        ++index;
      }

      if (&section != &commandList.back()) {
        ImGui::Separator();
      }
    }
  }
  ImGui::EndChild();

  return true;
}
} // namespace pgframes::widgets
