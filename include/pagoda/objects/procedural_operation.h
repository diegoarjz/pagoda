#pragma once

#include "pagoda/objects/interface.h"
#include "pagoda/objects/interface_callback.h"
#include "pagoda/objects/procedural_component.h"

#include <boost/signals2.hpp>
#include <pagoda/common/factory.h>
#include <pagoda/dynamic/builtin_class.h>

#include <cstddef>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace pagoda {
namespace dynamic {
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;
} // namespace dynamic
} // namespace pagoda

namespace pagoda::objects {
using ProceduralObjectPtr = std::shared_ptr<class ProceduralObject>;
using ProceduralObjectSystemPtr = std::shared_ptr<class ProceduralObjectSystem>;
class InterfaceCallback;
class NewParameterCallback;
class ParameterBase;

/**
 * @brief Base class for a procedural operation.
 *
 * Makes no assumptions about the schedulling system, making it agnostic to the
 * procedural modelling methodology (e.g., graph based or shape grammars)
 */
class ProceduralOperation : public dynamic::BuiltinClass {
public:
  static const dynamic::TypeInfoPtr s_typeInfo;

  ProceduralOperation(ProceduralObjectSystemPtr proceduralObjectSystem);
  ~ProceduralOperation() override;

  /**
   * Prepares the \c ProceduralOperation to be executed.
   */
  virtual bool Prepare() { return true; };

  /**
   * Executes the \c ProceduralOperation.
   */
  void Execute();

  /**
   * Finishes the \c ProceduralOperation after execution.
   */
  virtual void Finish() {}

  /**
   * Returns the name of this operation.
   */
  virtual const std::string &GetOperationName() const = 0;

  ////////////////////////////////////////////////////////////
  /// \name Interfaces
  ////////////////////////////////////////////////////////////
  /**
   * When called, subclasses must use \p cb with the correct values to specify
   * an Interface.
   */
  virtual void Interfaces(InterfaceCallback *cb) = 0;

  ////////////////////////////////////////////////////////////
  /// \name Parameters
  ////////////////////////////////////////////////////////////

  /**
   * Iterates over each operation parameter.
   */
  void ForEachParameter(
      const std::function<void(const std::string &,
                               const dynamic::DynamicValueBasePtr &type)> &f);

  virtual void Parameters(objects::NewParameterCallback *cb) {}

  virtual void ParameterChanged(std::shared_ptr<ParameterBase> par) {}

  ////////////////////////////////////////////////////////////
  /// \name BuiltinClass overrides
  ////////////////////////////////////////////////////////////
  std::string ToString() const override;
  void AcceptVisitor(dynamic::ValueVisitorBase &visitor) override;

protected:
  /**
   * Performs the operation work.
   */
  virtual void DoWork() = 0;

  /**
   * Updates the value of the \c DynamicValueBase with the \p valueName if it is
   * an expression.
   */
  void UpdateValue(const std::string &valueName);
  /**
   * Returns the value of the \c DynamicValueBase with the \p valueName.
   */
  dynamic::DynamicValueBasePtr GetValue(const std::string &valueName);

  /**
   * Creates an empty ProceduralObject.
   */
  ProceduralObjectPtr CreateOutputProceduralObject();

  /**
   * Creates a \c ProceduralObject in the output interface named \p
   * interfaceName, copying all components present in \p base.
   */
  ProceduralObjectPtr CreateOutputProceduralObject(ProceduralObjectPtr &base);

  /// Our ProceduralObjectSystem
  ProceduralObjectSystemPtr m_proceduralObjectSystem;

private:
  std::vector<std::string> m_parameterNames;

}; // class ProceduralOperation
} // namespace pagoda::objects
