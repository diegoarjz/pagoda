#include "viewer_node.h"

#include "pagoda/geometry/algorithms/ear_clipping.h"
#include "pgframes/viewer/viewer_window.h"

#include "pagoda/common/pluggable_factory.h"
#include "pagoda/graph/input_interface_node.h"
#include "pagoda/graph/graph.h"

#include <pagoda/objects/procedural_object_system.h>
#include <pagoda/objects/procedural_object.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>
#include <pagoda/objects/hierarchical_system.h>

#include <pagoda/scene/mesh.h>
#include <pagoda/scene/scene_graph.h>

#include <pagoda/api.h>

using namespace pagoda;
using namespace pagoda::objects;
using namespace pagoda::graph;
using namespace pagoda::scene;

namespace pgframes::viewer {
const char *ViewerNode::name = "ViewerNode";

namespace {
std::shared_ptr<Mesh> ConvertGeometry(
    pagoda::geometry::core::GeometryPtr pagodaGeom,
    scene::SceneNodePtr parentNode,
    scene::SceneGraphPtr sceneGraph,
    scene::Path path) {
  std::vector<math::Vec3F> verts;
  std::vector<uint32_t> indices;
  for (auto fIter = pagodaGeom->FacesBegin(); fIter != pagodaGeom->FacesEnd();
       ++fIter) {
    pagoda::geometry::core::Geometry::FaceAttributes fAttr = pagodaGeom->GetFaceAttributes(*fIter);
    //Vec3F normal = fAttr.m_normal;
    for (auto pCirc = pagodaGeom->FacePointCirculatorBegin(*fIter);
         pCirc.IsValid(); ++pCirc) {
      math::Vec3F pos = pagodaGeom->GetPosition(*pCirc);
      //Vec2F texCoord = pagodaGeom->GetVertexAttributes(*pCirc).m_texCoords;
      // clang-format off
      /*
      Mesh::Vertex v{
        {X(pos), Y(pos), Z(pos)},
        {X(normal), Y(normal), Z(normal)},
        {X(texCoord), Y(texCoord)}
      };
      */
      // clang-format on
      verts.push_back(pos);
      indices.push_back(indices.size());
    }
  }
  return sceneGraph->CreateNode<scene::Mesh>(parentNode, path, verts, indices);
}
} // namespace

ViewerNode::ViewerNode()
  : m_inputInterface{
    std::make_shared<Interface>("in", Interface::Type::Input, Interface::Arity::All)} {
}

namespace {
void addComponent(HierarchicalComponentPtr component, SceneNodePtr parentNode, SceneGraphPtr sceneGraph) {
  std::cout << "Adding component " << component->GetParentObject()->GetName()  << "(" << component->GetParentObject() << ")"<< std::endl;
  ProceduralObjectPtr object = component->GetParentObject();
  auto path = scene::Path{object->GetName()};

  if (component->ChildrenCount() == 0) {
    // Add Geometry to scene graph
    auto proceduralObjectSystem = object->GetProceduralObjectSystem();
    auto geometrySystem =
        proceduralObjectSystem->GetComponentSystem<geometry::GeometrySystem>();

    auto geometryComponent =
        geometrySystem->GetComponentAs<pagoda::geometry::GeometryComponent>(object);
    auto geometry = geometryComponent->GetGeometry();
    auto triagulatedGeometry = std::make_shared<pagoda::geometry::core::Geometry>();

    pagoda::geometry::algorithms::EarClipping<pagoda::geometry::core::Geometry> earClipping;
    earClipping.Execute(geometry, triagulatedGeometry);

    const auto mesh = ConvertGeometry(triagulatedGeometry, parentNode, sceneGraph, path);
  }
  else {
    auto newParent = sceneGraph->CreateNode<SceneNode>(parentNode, path);
    for (const auto& child : component->GetChildren()) {
      addComponent(child.lock(), newParent, sceneGraph);
    }
  }
}
}

void ViewerNode::Execute(const pagoda::graph::NodeSet &inNodes, const pagoda::graph::NodeSet &outNodes) {
  auto viewerWindow = std::dynamic_pointer_cast<ViewerWindow>(m_viewer);
  auto sceneGraph = viewerWindow->GetSceneGraph();

  // TODO: There should really only exist one.
  //       We need to rethink how nodes & operations are executed
  //       Should the procedural object system be part of an execution context?
  std::set<HierarchicalSystemPtr> hierarchicalSystems;
  m_inputInterface->GetAll([this, &hierarchicalSystems](pagoda::objects::ProceduralObjectPtr object) {
    auto proceduralObjectSystem = object->GetProceduralObjectSystem();
    auto hierarchicalSystem =
        proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();
    hierarchicalSystems.insert(hierarchicalSystem);
  });

  for (auto& h : hierarchicalSystems) {
    const auto& components = h->GetRootComponents();
    for (auto c : components) {
      addComponent(c.lock(), sceneGraph->GetRootNode(), sceneGraph);
    }
  }
}

const char *const ViewerNode::GetNodeType() { return name; }

void ViewerNode::AttachToGraph(pagoda::graph::Graph* graph) {
  Node::AttachToGraph(graph);
  auto interface = GetGraph()->CreateNode<pagoda::graph::InputInterfaceNode>("ViewerNode_in");
  m_inputInterfaceNode = std::dynamic_pointer_cast<InputInterfaceNode>(GetGraph()->GetNode(interface));
  std::dynamic_pointer_cast<pagoda::graph::InputInterfaceNode>(m_inputInterfaceNode)->SetInterfaceName("in");
  GetGraph()->CreateEdge(interface, GetName());
}

void ViewerNode::SetViewer(std::shared_ptr<Window> v) {
  m_viewer = v;
}

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
