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
  InterfaceCreator(const std::string& nodeName, Graph* graph)
      : m_nodeName{nodeName}, m_graph{graph} {}

  void InputInterface(InterfacePtr &interface, const std::string &name,
                      const std::string &label,
                      Interface::Arity arity) override {
    interface =
        std::make_shared<Interface>(name, Interface::Type::Input, arity);

    const auto inputInterfaceName = m_nodeName + "_" + name;
    if (m_graph->GetNode(inputInterfaceName) != nullptr) {
      LOG_WARNING("InputInterfaceNode '" << inputInterfaceName << "' already exists");
    }
    auto interfaceName = m_graph->CreateNode<InputInterfaceNode>(m_nodeName + "_" + name);
    std::dynamic_pointer_cast<InputInterfaceNode>(m_graph->GetNode(interfaceName))->SetInterfaceName(name);
    auto res = m_graph->CreateEdge(interfaceName, m_nodeName);
    if (res != Graph::EdgeCreated::Created) {
      LOG_ERROR("Could not create edge between InputInterfaceNode '" 
          << interfaceName << "' and node '" << m_nodeName << "'");
    }
  }

  void OutputInterface(InterfacePtr &interface, const std::string &name,
                       const std::string &label,
                       Interface::Arity arity) override {
    interface =
        std::make_shared<Interface>(name, Interface::Type::Output, arity);

    const auto outputInterfaceName = m_nodeName + "_" + name;
    if (m_graph->GetNode(outputInterfaceName) != nullptr) {
      LOG_WARNING("OutputInterfaceNode '" << outputInterfaceName << "' already exists");
    }
    auto interfaceName = m_graph->CreateNode<OutputInterfaceNode>(m_nodeName + "_" + name);
    std::dynamic_pointer_cast<OutputInterfaceNode>(m_graph->GetNode(interfaceName))->SetInterfaceName(name);
    auto res = m_graph->CreateEdge(m_nodeName, interfaceName);
    if (res != Graph::EdgeCreated::Created) {
      LOG_ERROR("Could not create edge between OutputInterfaceNode '" 
          << interfaceName << "' and node '" << m_nodeName << "'");
    }
  }

private:
  const std::string& m_nodeName;
  Graph* m_graph;
};

void OperationNode::SetOperation(ProceduralOperationPtr operation) {
  LOG_TRACE(ProceduralGraph, "Setting Procedural Operation '" << operation->GetOperationName()
      << "' in Operation Node '" << GetName() << "'.");
  m_operation = operation;

  // Create the interfaces
  InterfaceCreator interfaceCreator{GetName(), GetGraph()};
  m_operation->Interfaces(&interfaceCreator);

  // Create the parameters
  ParameterCreator paramCreator([this](ParameterBasePtr p) {
    return m_parameters.insert({p->GetName(), p}).second;
  });
  m_operation->Parameters(&paramCreator);

  RegisterOrSetMember("op", m_operation);
}

void OperationNode::ForEachOperationParameter(
    const std::function<void(const std::string &,
                             const dynamic::DynamicValueBasePtr &)> &f) const {
  m_operation->ForEachParameter(f);
}

namespace {
class Callback : public objects::InterfaceCallback {
public:
  Callback(const std::string& name, bool input, InterfacePtr& iface)
    : name{name}, isInput{input}, interface{iface} {}

  void InputInterface(InterfacePtr& i, const std::string& name, const std::string& label,
                              Interface::Arity arity) override {
    if (isInput && name == this->name) {
      interface = i;
    }
  }

  void OutputInterface(InterfacePtr& i, const std::string& name, const std::string& label,
                               Interface::Arity arity) override {
    if (!isInput && name == this->name) {
      interface = i;
    }
  }
  
  std::string name;
  bool isInput;
  InterfacePtr& interface;
};
}

objects::InterfacePtr
OperationNode::GetInputInterface(const std::string &name) const {
  InterfacePtr interface = nullptr;
  Callback cb{name, true, interface};
  m_operation->Interfaces(&cb);
  return interface;
}

objects::InterfacePtr
OperationNode::GetOutputInterface(const std::string &name) const {
  InterfacePtr interface = nullptr;
  Callback cb{name, false, interface};
  m_operation->Interfaces(&cb);
  return interface;
}

void OperationNode::Execute(const NodeSet &inNodes, const NodeSet &outNodes) {
  LOG_TRACE(ProceduralGraph,
            "Executing OperationNode '" << GetName() << "'(" << GetId() << ")");

  m_operation->Execute();

}

const char *const OperationNode::GetNodeType() {
  static const char *const sNodeType = "Operation";
  return sNodeType;
}

void OperationNode::Interfaces(objects::InterfaceCallback* cb) {
  if (m_operation != nullptr) {
    m_operation->Interfaces(cb);
  }
}

void OperationNode::InputInterfaces(objects::InterfaceCallback* cb) {
  if (m_operation != nullptr) {
    m_operation->InputInterfaces(cb);
  }
}

void OperationNode::OutputInterfaces(objects::InterfaceCallback* cb) {
  if (m_operation != nullptr) {
    m_operation->OutputInterfaces(cb);
  }
}

} // namespace pagoda::graph
