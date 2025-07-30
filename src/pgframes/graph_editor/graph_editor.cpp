#include "graph_editor.h"

#include "graph_properties.h"
#include "graph_delegate.h"
#include "pagoda_graph_delegate.h"
#include "pgframes/viewer/viewer_node.h"

#include "../widgets/command_list.h"

#include "pagoda/graph/graph.h"
#include "pagoda/graph/node.h"
#include "pagoda/graph/input_interface_node.h"
#include "pagoda/graph/io/writer.h"

#include "pagoda/objects/operation_factory.h"

#include "pagoda/common/exception/exception.h"
#include "pagoda/common/fs/file_util.h"

#include "pagoda/pagoda.h"

#include "imgui.h"

#include "ImGuiFileDialog.h"
#include "imgui_node_editor.h"

#include <magic_enum/magic_enum.hpp>
#include <fmt/format.h>

#include <boost/functional/hash.hpp>

namespace ed = ax::NodeEditor;

using namespace pagoda;
using namespace pagoda::dynamic;
using namespace pagoda::objects;
using namespace pagoda::graph;
using namespace pagoda::graph::io;

namespace pgframes::graph_editor {

class GraphEditor::Impl : public widgets::CommandListDelegate {
public:
  Impl(GraphEditor *g) : m_editor{g} {
    ed::Config config;
    m_context = ed::CreateEditor(&config);

    m_availableNodes = m_pagoda.GetOperationFactory()->RegisteredTypes();
    m_graph = m_pagoda.CreateGraph();
    m_graphDelegate = std::make_shared<PagodaGraphDelegate>(m_pagoda, m_graph);

    for (const auto &opName : m_availableNodes) {
      widgets::Command command;
      command.label = opName;
      command.callback = [&]() { m_graphDelegate->CreateNode(opName); };

      m_commands.commands.push_back(command);
    }

    widgets::Command viewerNodeCommand;
    viewerNodeCommand.label = "Viewer";
    viewerNodeCommand.callback = [&]() {
      if (auto nodeIdentifier = m_graphDelegate->CreateNode("ViewerNode")) {
        auto viewerNode = reinterpret_cast<pagoda::graph::Node*>(nodeIdentifier);
        if (auto n = static_cast<viewer::ViewerNode*>(viewerNode)) {
          auto viewer = m_editor->GetWindowManager()->CreateWindow("ViewerWindow").lock();
          n->SetViewer(viewer);
        }
      }
    };
    m_commands.commands.push_back(viewerNodeCommand);
  }

  ~Impl() { ed::DestroyEditor(m_context); }

  bool Draw() {
    if (m_graphProperties == nullptr) {
      m_graphProperties = 
        std::dynamic_pointer_cast<GraphProperties>(m_editor->GetWindowManager()->CreateWindow("GraphProperties").lock());
      m_graphProperties->SetGraphDelegate(m_graphDelegate);
    }

    const auto contentRegion = ImGui::GetContentRegionAvail();

    if (ImGui::Button("Load")) {
      ImGuiFileDialog::Instance()->OpenDialog("OpenGraphFile", "Choose File",
                                              ".pgd");
    }

    if (ImGuiFileDialog::Instance()->Display("OpenGraphFile")) {
      if (ImGuiFileDialog::Instance()->IsOk()) {
        auto graphFileInput =
            std::filesystem::path(
                ImGuiFileDialog::Instance()->GetCurrentPath()) /
            ImGuiFileDialog::Instance()->GetCurrentFileName();
        SetFile(graphFileInput);
      }

      ImGuiFileDialog::Instance()->Close();
    }

    ImGui::SameLine();
    if (ImGui::Button("Save")) {
      SaveFile();
    }

    ImGui::SameLine();
    if (ImGui::Button("Execute")) {
      m_graph->Execute();
    }

    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Tab)) {
      m_popupWindowOpen = true;
      m_popupWindowLocation = ImGui::GetMousePos();
    }

    if (m_popupWindowOpen) {
      ImGui::SetNextWindowPos(m_popupWindowLocation, ImGuiCond_Appearing);
      ImGui::SetNextWindowSize({220, 120}, ImGuiCond_Appearing);
      ImGuiWindowFlags flags =
          ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
          ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
      if (ImGui::Begin("##operationsWindow", &m_popupWindowOpen, flags)) {
        widgets::CommandList("operations", *this, {0, 100});
      }
      ImGui::End();
    }

    // Graph editor
    ed::SetCurrentEditor(m_context);
    ed::Begin("MyEditor");

    if (!m_popupWindowOpen) {
      m_nodeCreationLocation = ed::ScreenToCanvas(ImGui::GetMousePos());
    }

    // Draw the nodes
    auto nodes = m_graphDelegate->Nodes();
    for (const auto& n : nodes) {
      ed::BeginNode(n);
      ImGui::Text("%s", m_graphDelegate->NodeText(n).c_str());
      const auto nodeSize = ed::GetNodeSize(n);

      const auto inputs = m_graphDelegate->NodeInputs(n);
      const auto outputs = m_graphDelegate->NodeOutputs(n);

      auto inputIter = inputs.begin();
      auto outputIter = outputs.begin();

      const auto createInputPin = [&](auto interface) {
        ed::BeginPin(interface, ed::PinKind::Input);
        ImGui::Text("%s", m_graphDelegate->InputText(interface).c_str());
        ed::EndPin();
      };
      const auto createOutputPin = [&](auto interface) {
        const std::string text = m_graphDelegate->OutputText(interface);
        const auto textSize =
            ImGui::CalcTextSize(text.c_str());

        const auto currPos = ImGui::GetCursorPos();
        auto posX =
            currPos.x + nodeSize.x - textSize.x -
            (ed::GetStyle().NodePadding.w + ed::GetStyle().NodePadding.x);
        ImGui::SetCursorPosX(posX);

        ed::BeginPin(interface, ed::PinKind::Output);
        ImGui::Text("%s", text.c_str());
        ed::EndPin();
      };

      for (int i = 0; i < std::min(inputs.size(), outputs.size()); ++i) {
        const auto isInputInterface = (i % 2) == 0;

        if (isInputInterface) {
          if (inputIter != inputs.end()) {
            createInputPin(*inputIter);
            ++inputIter;
          }
        } else {
          if (outputIter != outputs.end()) {
            createOutputPin(*outputIter);
            ++outputIter;
          }
        }
      }

      while (inputIter != inputs.end()) {
        createInputPin(*inputIter);
        ++inputIter;
      }
      while (outputIter != outputs.end()) {
        createOutputPin(*outputIter);
        ++outputIter;
      }

      ed::EndNode();
    }

    // All the nodes are created, set their position if the graph is new
    for (auto n : m_newNodes) {
      ImVec2 nodePos;
      m_graphDelegate->GetNodePosition(n, nodePos.x, nodePos.y);
      ed::SetNodePosition(n, nodePos);
    }
    m_newNodes.clear();

    // Handle interaction
    if (ed::BeginCreate()) {
      ed::PinId inputId;
      ed::PinId outputId;
      if (ed::QueryNewLink(&inputId, &outputId)) {
        if (inputId && outputId) {
          const auto inputHandle = reinterpret_cast<GraphDelegate::OutputHandle>(outputId.AsPointer());
          const auto outputHandle = reinterpret_cast<GraphDelegate::InputHandle>(inputId.AsPointer());
          if (m_graphDelegate->CanCreateEdge(outputHandle, inputHandle)) {
            if (ed::AcceptNewItem({0, 1, 0, 1})) {
              m_graphDelegate->CreateEdge(outputHandle, inputHandle);
            }
          }
          else {
            ed::RejectNewItem({1, 0, 0, 1}, 1);
          }
        }
      }
    }
    ed::EndCreate();

    if (ed::BeginDelete()) {
      ed::NodeId nodeId;
      while (ed::QueryDeletedNode(&nodeId)) {
        if (ed::AcceptDeletedItem()) {
          m_graphDelegate->DeleteNode(reinterpret_cast<GraphDelegate::NodeHandle>(nodeId.AsPointer()));
        }
      }

      ed::LinkId linkId;
      while (ed::QueryDeletedLink(&linkId)) {
        if (ed::AcceptDeletedItem()) {
          const auto iter = m_edges.find(linkId.Get());
          if (iter != m_edges.end()) {
            const auto outHandle = iter->second.output;
            const auto inHandle = iter->second.input;
            m_graphDelegate->DeleteEdge(outHandle, inHandle);
          }
        }
      }
    }
    ed::EndDelete();

    // Handle node dragging
    if (auto node = ed::GetHoveredNode()) {
      auto pos = ed::GetNodePosition(node);
      m_graphDelegate->SetNodePosition(reinterpret_cast<GraphDelegate::NodeHandle>(node.AsPointer()),
                                       pos.x, pos.y);
    }

    // Draw the edges
    auto edges = m_graphDelegate->GraphEdges();
    for (const auto& e : edges) {
      std::size_t id = 0;
      boost::hash_combine(id, e.output);
      boost::hash_combine(id, e.input);
      ed::Link(id, e.output, e.input);
      m_edges.emplace(id, e);
    }

    if (auto id = ed::GetDoubleClickedNode().AsPointer()) {
      const auto nodeName = reinterpret_cast<pagoda::graph::Node*>(id)->GetName();
      m_graphProperties->OpenNodeProperty(m_graph->GetNode(nodeName));
    }

    ed::End();
    ed::SetCurrentEditor(nullptr);
    return true;
  }

  void SetFile(const std::string &f) {
    m_currentFile = f;
    try {
      m_graph = m_pagoda.CreateGraphFromFile(m_currentFile);
      m_graphDelegate = std::make_shared<PagodaGraphDelegate>(m_pagoda, m_graph);
      m_newNodes = m_graphDelegate->Nodes();

      NodeSet operationNodes;
    } catch (pagoda::common::exception::Exception &e) {
      m_graph = m_pagoda.CreateGraph();
    }
  }

  void SaveFile() {
    try {
      GraphWriter writer(m_graph);
      std::stringstream outStream;
      writer.Write(outStream);
      common::fs::WriteStringToFile(m_currentFile, outStream.str());
    } catch (common::exception::Exception &e) {
    }
  }

  bool AllowFiltering() const override { return true; }
  std::string &FilterString() override { return m_filterString; }

  bool AllowKeyNavigation() const override { return true; }
  int &SelectedItem() override { return m_selectedIndex; }
  void CloseRequested() override {
    m_popupWindowOpen = false;
    Closed();
  }

  virtual const std::vector<widgets::Section> GetCommandList() const override {
    return {m_commands};
  }

private:
  ed::EditorContext *m_context;
  Pagoda m_pagoda;
  GraphPtr m_graph;
  std::shared_ptr<GraphDelegate> m_graphDelegate;
  std::unordered_map<std::size_t, GraphDelegate::Edge> m_edges;
  std::vector<std::string> m_availableNodes;
  GraphDelegate::NodeHandleList m_newNodes;

  std::string m_currentFile;

  widgets::Section m_commands;
  bool m_popupWindowOpen{false};
  ImVec2 m_popupWindowLocation{0, 0};
  ImVec2 m_nodeCreationLocation{0, 0};

  std::string m_filterString{""};
  int m_selectedIndex{-1};

  GraphEditor *m_editor;
  std::shared_ptr<GraphProperties> m_graphProperties;
};

GraphEditor::GraphEditor() : m_impl{std::make_unique<Impl>(this)} {
}
GraphEditor::~GraphEditor() {}

const std::string &GraphEditor::WindowName() {
  static const std::string name{"Graph Editor"};
  return name;
}

bool GraphEditor::Draw() { 
  return m_impl->Draw(); }
} // namespace pgframes::graph_editor

extern "C" PAGODA_PLUGIN void *CreateWindow() {
  using FactoryClass =
      pagoda::common::PluggableFactory<pgframes::Window>;
  using BaseRegistrationClass = FactoryClass::Registration;

  struct Reg : public BaseRegistrationClass {
    virtual void Register(FactoryClass *factory) {
      factory->Register("GraphEditor", []() {
        return std::make_shared<pgframes::graph_editor::GraphEditor>();
      });
      factory->Register("GraphProperties", []() {
        return std::make_shared<pgframes::graph_editor::GraphProperties>();
      });
    }
  };

  return new Reg;
}
