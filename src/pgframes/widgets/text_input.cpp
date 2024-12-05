#include "text_input.h"

#include <imgui.h>

namespace pgframes::widgets {
namespace {
struct UserData {
  std::string *str;
};

int textCallback(ImGuiInputTextCallbackData *data) {
  UserData *userData = static_cast<UserData *>(data->UserData);

  if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
    std::string *str = userData->str;
    str->resize(data->BufTextLen);
    data->Buf = (char *)str->c_str();
  }
  return 0;
}
} // namespace

bool TextInput(const char *label, std::string &text, bool modifiable) {
  UserData userData;
  userData.str = &text;

  ImGuiInputTextFlags flags = ImGuiInputTextFlags_CallbackResize;
  if (!modifiable) {
    flags |= ImGuiInputTextFlags_ReadOnly;
  }

  return ImGui::InputText(label, (char *)text.c_str(), text.capacity() + 1,
                          flags, textCallback, &userData);
}

bool MultiLineTextInput(const char *label, std::string &text, bool modifiable) {
  UserData userData;
  userData.str = &text;

  ImGuiInputTextFlags flags = ImGuiInputTextFlags_CallbackResize;
  if (!modifiable) {
    flags |= ImGuiInputTextFlags_ReadOnly;
  }

  return ImGui::InputTextMultiline(label, (char *)text.c_str(),
                                   text.capacity() + 1, ImVec2(-1, -1), flags,
                                   textCallback, &userData);
}
} // namespace pgframes::widgets
