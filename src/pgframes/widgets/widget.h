#pragma once

#include <pagoda/math/vec_base.h>

#include <functional>
#include <list>
#include <memory>

namespace pgframes::widgets {
using WidgetPtr = std::shared_ptr<class Widget>;

class Widget {
public:
  Widget();

  virtual void Draw() = 0;

protected:
  std::string m_id;
};

class GroupWidget : public Widget {
public:
  void AddWdiget(const WidgetPtr& widget);

protected:
  std::list<WidgetPtr> m_childWidgets;
};

class HorizontalLayout : public GroupWidget {
public:
  void Draw() override;
};

class VerticalLayout : public GroupWidget {
public:
  void Draw() override;
};

}
