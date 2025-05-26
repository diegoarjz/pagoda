#include "pgframes/widgets/tree_view.h"

#include "pagoda/common/delegate.h"

#include "imgui.h"

namespace pgframes::widgets {
class TreeView::Impl {
public:
  Impl() {}
  ~Impl() {}

  std::shared_ptr<TreeView::Delegate> m_delegate;

  void draw(const NodeHandle& n) {
    const std::string& label = m_delegate->Label(n);
    const uint32_t numChildren = m_delegate->NumChildren(n);

    bool itemWasClicked = false;

    if (numChildren == 0) {
      if (ImGui::Selectable(label.c_str())) {
        itemWasClicked |= true;
      }
    }
    else {
      if (ImGui::TreeNodeEx(label.c_str())) {
        itemWasClicked |= ImGui::IsItemClicked();
        m_delegate->Children(n, [this] (const auto& c) {
          this->draw(c);
        });
        ImGui::TreePop();
      }
      else {
        itemWasClicked |= ImGui::IsItemClicked();
      }
    }

    if (itemWasClicked) {
      m_pressed(n);
    }
  }

  pagoda::common::Delegate<void, const TreeView::NodeHandle&> m_pressed;
};

TreeView::TreeView(const std::shared_ptr<Delegate>& delegate)
  : m_impl{std::make_unique<Impl>()}
{
  m_impl->m_delegate = delegate;
}

TreeView::~TreeView()
{
}

void TreeView::Draw()
{
  auto& delegate = m_impl->m_delegate;
  delegate->Roots([this] (const NodeHandle& n) {
    m_impl->draw(n);
  });
}

void TreeView::OnPressed(std::function<void(const NodeHandle&)> pressed) {
  m_impl->m_pressed.AddCallback(pressed);
}
}
