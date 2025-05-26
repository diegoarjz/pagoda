#pragma once

#include <pagoda/math/vec_base.h>

#include <list>
#include <memory>

namespace pgframes::widgets {
// TODO: Move this to a better place
struct Size {
  enum class Type { Relative, Absolute, Remaining };
  Type type;
  float size;
};

using WidgetPtr = std::shared_ptr<class Widget>;

class Widget {
public:
  Widget();

  virtual void Draw() = 0;

  void SetSizeHint(Size width, Size height);

protected:

  pagoda::math::Vec2F CalculateSize();

  std::string m_id;
  Size m_width{.type = Size::Type::Remaining, .size = 1.0f };
  Size m_height{.type = Size::Type::Remaining, .size = 1.0f };
};

class GroupWidget : public Widget {
public:
  void AddWdiget(const WidgetPtr& widget);

  const std::list<WidgetPtr>& GetWidgets() const;

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
