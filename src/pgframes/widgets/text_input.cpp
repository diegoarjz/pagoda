#include "pgframes/widgets/text_input.h"
#include "pagoda/common/delegate.h"

#include <fmt/format.h>
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

//----------------------------------------
// Text Edir

class TextEdit::Impl {
public:
  Impl(const std::string& value)
    : m_value{value}
  {
  }
  ~Impl()
  {
  }

  std::string m_value;
  pagoda::common::Delegate<void, const std::string&> m_onValueChanged;
};

TextEdit::TextEdit(const std::string& text)
  : m_impl{std::make_unique<Impl>(text)}
{
}

TextEdit::~TextEdit() { }

void TextEdit::Draw() {
  UserData userData;
  userData.str = &m_impl->m_value;

  ImGuiInputTextFlags flags = ImGuiInputTextFlags_CallbackResize;

  ImGui::SetNextItemWidth(X(CalculateSize()));

  const bool modified = ImGui::InputText(fmt::format("##{}", m_id).c_str(),
                                         (char *)m_impl->m_value.c_str(),
                                         m_impl->m_value.capacity() + 1,
                                         flags,
                                         textCallback,
                                         &userData);
  if (modified) {
    m_impl->m_onValueChanged(m_impl->m_value);
  }
}

void TextEdit::OnValueChanged(ValueChangedCalback_t cb) {
  m_impl->m_onValueChanged.AddCallback(cb);
}


} // namespace pgframes::widgets
