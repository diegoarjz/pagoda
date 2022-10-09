#include "pagoda/graph/operation_node.h"

#include "pagoda/graph/graph.h"
#include "pagoda/graph/input_interface_node.h"
#include "pagoda/graph/node.h"
#include "pagoda/graph/node_set_visitor.h"
#include "pagoda/graph/output_interface_node.h"
#include "pagoda/graph/unsupported_node_link.h"

#include <pagoda/common/utils.h>

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/value_not_found.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/operation_factory.h>
#include <pagoda/objects/parameter_creator.h>
#include <pagoda/objects/procedural_operation.h>
#include <pagoda/objects/unknown_operation.h>

using namespace pagoda::dynamic;
using namespace pagoda::objects;

namespace pagoda::graph {
const char *OperationNode::name = "Operation";

OperationNode::OperationNode(OperationFactoryPtr operationFactory)
    : m_operationFactory(operationFactory) {
  auto operationParameter =
      std::make_shared<StringParameter>(&m_operationName, "operation");
  operationParameter->OnChanged([this](ParameterBasePtr p) {
    const auto operation = m_operationFactory->Create(m_operationName);
    if (operation == nullptr) {
      throw UnknownOperation(m_operationName);
    }
    SetOperation(operation);
  });
  m_parameters["operation"] = operationParameter;
}

OperationNode::~OperationNode() {}

class InterfaceCreator : public InterfaceCallback {
public:
  InterfaceCreator(InterfacePtrMap &inputs, InterfacePtrMap &outputs)
      : m_inputs{inputs}, m_outputs{outputs} {}

  void InputInterface(InterfacePtr &interface, const std::string &name,
                      const std::string &label,
                      Interface::Arity arity) override {
    interface =
        std::make_shared<Interface>(name, Interface::Type::Input, arity);
    m_inputs.emplace(name, interface);
  }

  void OutputInterface(InterfacePtr &interface, const std::string &name,
                       const std::string &label,
                       Interface::Arity arity) override {
    interface =
        std::make_shared<Interface>(name, Interface::Type::Output, arity);
    m_outputs.emplace(name, interface);
  }

private:
  InterfacePtrMap &m_inputs;
  InterfacePtrMap &m_outputs;
};

void OperationNode::SetOperation(ProceduralOperationPtr operation) {
  m_operation = operation;
  ParameterCreator paramCreator([this](ParameterBasePtr p) {
    return m_parameters.insert({p->GetName(), p}).second;
  });

  InterfaceCreator interfaceCreator{m_inputInterfaces, m_outputInterfaces};
  m_operation->Interfaces(&interfaceCreator);

  m_operation->Parameters(&paramCreator);

  RegisterOrSetMember("op", m_operation);
}

void OperationNode::ForEachOperationParameter(
    const std::function<void(const std::string &,
                             const dynamic::DynamicValueBasePtr &)> &f) const {
  m_operation->ForEachParameter(f);
}

objects::InterfacePtr
OperationNode::GetInputInterface(const std::string &name) const {
  auto iter = m_inputInterfaces.find(name);
  if (iter == m_inputInterfaces.end()) {
    return nullptr;
  }
  return iter->second;
}

objects::InterfacePtr
OperationNode::GetOutputInterface(const std::string &name) const {
  auto iter = m_outputInterfaces.find(name);
  if (iter == m_outputInterfaces.end()) {
    return nullptr;
  }
  return iter->second;
}

namespace {
class InterfaceObjectProvider : public InterfaceCallback {
public:
  using Inserter_t = std::back_insert_iterator<std::vector<InterfacePtr>>;

  InterfaceObjectProvider(Inserter_t inputs, Inserter_t outputs)
      : m_inputs{inputs}, m_outputs{outputs} {}

  void InputInterface(InterfacePtr &interface, const std::string &name,
                      const std::string &label,
                      Interface::Arity arity) override {
    CRITICAL_ASSERT_MSG(interface != nullptr,
                        "Input Interface not set when executing a node");
    m_inputs = interface;
  }

  void OutputInterface(InterfacePtr &interface, const std::string &name,
                       const std::string &label,
                       Interface::Arity arity) override {
    CRITICAL_ASSERT_MSG(interface != nullptr,
                        "Output Interface not set when executing a node");
    m_outputs = interface;
  }

  std::back_insert_iterator<std::vector<InterfacePtr>> m_inputs;
  std::back_insert_iterator<std::vector<InterfacePtr>> m_outputs;
};

template <class T>
void pairInterfaceWithNodes(
    const std::vector<InterfacePtr> &interfaces, const NodeSet &nodes,
    std::unordered_map<InterfacePtr, std::shared_ptr<T>> &pairings) {
  for (auto &inputInterface : interfaces) {
    auto interfaceName = inputInterface->GetName();
    for (auto &n : nodes) {
      if (auto interfaceNode = std::dynamic_pointer_cast<T>(n)) {
        if (interfaceNode->GetInterfaceName() == interfaceName) {
          pairings.emplace(inputInterface, interfaceNode);
        }
      }
    }
  }
}

} // namespace

void OperationNode::Execute(const NodeSet &inNodes, const NodeSet &outNodes) {
  LOG_TRACE(ProceduralGraph,
            "Executing OperationNode '" << GetName() << "'(" << GetId() << ")");

  m_operation->Execute();

}

const char *const OperationNode::GetNodeType() {
  static const char *const sNodeType = "Operation";
  return sNodeType;
}

} // namespace pagoda::graph
