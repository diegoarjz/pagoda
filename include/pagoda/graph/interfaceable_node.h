#pragma once

#include <string>

namespace pagoda::objects {
  class InterfaceCallback;
  using InterfacePtr = std::shared_ptr<class Interface>;
}

namespace pagoda::graph {
/**
 * \brief Represents a \c Node that can have interfaces.
 */
class InterfaceableNode {
public:
  /**
   * Calls \a cb on each input and output interface.
   */
  virtual void Interfaces(objects::InterfaceCallback* cb) = 0;
  /**
   * Calls \b cb on each input interface.
   */
  virtual void InputInterfaces(objects::InterfaceCallback* cb) = 0;
  /**
   * Calls \b cb on each output interface.
   */
  virtual void OutputInterfaces(objects::InterfaceCallback* cb) = 0;
  virtual objects::InterfacePtr GetInputInterface(const std::string& name) const = 0;
  virtual objects::InterfacePtr GetOutputInterface(const std::string& name) const = 0;
};
}
