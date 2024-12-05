#include "viewer_node.h"

#include "pagoda/common/pluggable_factory.h"
#include "pagoda/graph/input_interface_node.h"
#include "pagoda/graph/graph.h"
#include "pagoda/pagoda.h"

using namespace pagoda;
using namespace pagoda::objects;
using namespace pagoda::graph;

namespace pgframes::viewer {
const char *ViewerNode::name = "ViewerNode";

ViewerNode::ViewerNode()
  : m_inputInterface{
    std::make_shared<Interface>("in", Interface::Type::Input, Interface::Arity::All)} {
  //
}

void ViewerNode::Execute(const pagoda::graph::NodeSet &inNodes, const pagoda::graph::NodeSet &outNodes) {
  for (auto i : inNodes) {
    if (auto inInterface = std::dynamic_pointer_cast<InputInterfaceNode>(i)) {
    }
  }
  m_inputInterface->GetAll([](auto object) {
    std::cout << "Found an object" << std::endl;
  });
}

const char *const ViewerNode::GetNodeType() { return name; }

void ViewerNode::AttachToGraph(pagoda::graph::Graph* graph) {
  Node::AttachToGraph(graph);
  auto interface = GetGraph()->CreateNode<pagoda::graph::InputInterfaceNode>("ViewerNode_in");
  m_inputInterfaceNode = std::dynamic_pointer_cast<InputInterfaceNode>(GetGraph()->GetNode(interface));
  std::dynamic_pointer_cast<pagoda::graph::InputInterfaceNode>(m_inputInterfaceNode)->SetInterfaceName("in");
  GetGraph()->CreateEdge(interface, GetName());
}

void ViewerNode::SetViewer(std::shared_ptr<ViewerNode> v) { m_viewer = v; }

void ViewerNode::Interfaces(pagoda::objects::InterfaceCallback* cb) {
  cb->InputInterface(m_inputInterface, "in", "in", Interface::Arity::All);
}

void ViewerNode::InputInterfaces(pagoda::objects::InterfaceCallback* cb) {
  Interfaces(cb);
}

void ViewerNode::OutputInterfaces(pagoda::objects::InterfaceCallback* cb) {
  Interfaces(cb);
}

pagoda::objects::InterfacePtr ViewerNode::GetInputInterface(const std::string& name) const {
  if (name == m_inputInterface->GetName()) {
    return m_inputInterface;
  }
  return nullptr;
}

pagoda::objects::InterfacePtr ViewerNode::GetOutputInterface(const std::string& name) const {
  return nullptr;
}

}

extern "C" PAGODA_PLUGIN void *CreateNode() {
  using FactoryClass =
      pagoda::common::PluggableFactory<pagoda::graph::Node>;
  using BaseRegistrationClass = FactoryClass::Registration;

  struct Reg : public BaseRegistrationClass {
    virtual void Register(FactoryClass *factory) {
      factory->Register("ViewerNode", []() {
        return std::make_shared<pgframes::viewer::ViewerNode>();
      });
    }
  };

  return new Reg;
}
