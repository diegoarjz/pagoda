#pragma once

#include "../window.h"

#include <list>

namespace pagoda::graph {
  using NodePtr = std::shared_ptr<class Node>;
}

namespace pgframes::graph_editor {

using GraphDelegatePtr = std::shared_ptr<class GraphDelegate>;

class GraphProperties : public pgframes::Window {
public:
  GraphProperties();
  ~GraphProperties() override;

  pgframes::DockHint GetDockHint() const override { return pgframes::DockHint::RightPanel; }

  void SetGraphDelegate(GraphDelegatePtr delegate);

  void OpenNodeProperty(const pagoda::graph::NodePtr& n);

protected:
  const std::string &WindowName() override;
  bool Draw() override;

  std::list<pagoda::graph::NodePtr> m_propertyNodes;
  GraphDelegatePtr m_graphDelegate;
};
} // namespace pgframes::graph_editor

