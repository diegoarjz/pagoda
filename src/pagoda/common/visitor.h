#pragma once

namespace pagoda::common {

//------------------------------------------------------------
// Visitor class
template <typename... Types> class Visitor;

template <typename T> class Visitor<T> {
public:
  virtual void Visit(T &visitable) { }
};

template <typename T, typename... Types>
class Visitor<T, Types...> : public Visitor<Types...> {
public:
  using Visitor<Types...>::Visit;

  virtual void Visit(T &visitable) { }
};

//------------------------------------------------------------
// Visitable class

template <typename Derived> class Visitable {
public:
  template<class T>
  void Accept(T &visitor) {
    visitor.Visit(static_cast<Derived &>(*this));
  }
};

} // namespace pagoda::common
