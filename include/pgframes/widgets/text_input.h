#pragma once

#include "widget.h"

#include <string>

namespace pgframes::widgets {

bool TextInput(const char *label, std::string &text, bool modifiable = true);

bool MultiLineTextInput(const char *label, std::string &text, bool modifiable = true);

class TextEdit : public Widget {
public:
  TextEdit(const std::string& text);
  ~TextEdit();

  void Draw() override;

  void SetText(const std::string& text);
  std::string GetText() const;

  using ValueChangedCalback_t = std::function<void(const std::string&)>;
  void OnValueChanged(ValueChangedCalback_t cb);
private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};

} // namespace pgframes::widgets
