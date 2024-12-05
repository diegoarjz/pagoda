#pragma once

#include <functional>
#include <string>
#include <vector>

#include <imgui.h>

namespace pgframes::widgets {

struct Command {
  std::string label;
  std::function<void()> callback;
};

struct Section {
  std::string name;
  std::vector<Command> commands;
};

class CommandListDelegate {
public:
  virtual ~CommandListDelegate() {}

  virtual bool AllowFiltering() const = 0;
  virtual std::string &FilterString() = 0;

  virtual bool AllowKeyNavigation() const = 0;
  virtual int &SelectedItem() = 0;
  virtual void CloseRequested() {};

  virtual const std::vector<Section> GetCommandList() const = 0;

  void Closed() { m_firstOpened = true; }
private:
  bool m_firstOpened{true};

  friend bool CommandList(const char *id, CommandListDelegate &delegate,
                          const ImVec2 &size);
};

bool CommandList(const char *id, CommandListDelegate &delegate,
                 const ImVec2 &size = {0, 0});
} // namespace pgframes::widgets
