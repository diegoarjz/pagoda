
#include "pagoda/graph/input_interface_node.h"
#include "pagoda/graph/node.h"
#include "pagoda/graph/interfaceable_node.h"
#include "pgframes/window.h"

namespace pgframes::viewer {

class ViewerNode : public pagoda::graph::Node, public pagoda::graph::InterfaceableNode {
public:
  static const char *name;

  ViewerNode();
  ~ViewerNode() override = default;

  void Execute(const pagoda::graph::NodeSet &inNodes, const pagoda::graph::NodeSet &outNodes) override;

  const char *const GetNodeType() override;

  void AttachToGraph(pagoda::graph::Graph* graph) override;

  void SetViewer(std::shared_ptr<Window> v);

  void Interfaces(pagoda::objects::InterfaceCallback* cb) override;
  void InputInterfaces(pagoda::objects::InterfaceCallback* cb) override;
  void OutputInterfaces(pagoda::objects::InterfaceCallback* cb) override;
  pagoda::objects::InterfacePtr GetInputInterface(const std::string& name) const override;
  pagoda::objects::InterfacePtr GetOutputInterface(const std::string& name) const override;
private:
  std::shared_ptr<Window> m_viewer;
  pagoda::graph::InputInterfaceNodePtr m_inputInterfaceNode;
  pagoda::objects::InterfacePtr m_inputInterface;
};

}
