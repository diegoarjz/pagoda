#pragma once

#include "pgframes/widgets/widget.h"

#include <any>

namespace pgframes::widgets {
class TreeView : public Widget {
public:
  using NodeHandle = std::any;
  using NodeHandleVisitor = std::function<void(const NodeHandle&)>;

  class Delegate {
  public:
    virtual void Roots(NodeHandleVisitor) = 0;
    virtual std::string Label(const NodeHandle& node) = 0;
    virtual void Children(const NodeHandle&, NodeHandleVisitor) = 0;
    virtual uint32_t NumChildren(const NodeHandle&) const = 0;
  };

  TreeView(const std::shared_ptr<Delegate>& delegate);
  ~TreeView();

  void Draw() override;

  void OnPressed(std::function<void(const NodeHandle&)> pressed);

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
}
