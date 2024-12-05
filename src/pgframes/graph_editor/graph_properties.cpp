#include "graph_properties.h"

#include "graph_delegate.h"

#include "pagoda/graph/node.h"
#include "pagoda/graph/operation_node.h"
#include "pagoda/objects/parameter_callback.h"
#include "pgframes/widgets/field.h"
#include <fmt/format.h>
#include <imgui.h>

using namespace pagoda;
using namespace pagoda::dynamic;
using namespace pagoda::objects;
using namespace pagoda::graph;

namespace pgframes::graph_editor {
  class CreateParameters : public objects::NewParameterCallback {
  public:
    StringParameterPtr
    StringParameter(std::string *v, const std::string &name,
                    const std::string &label,
                    const std::string &defaultValue = "") override {
      auto par = std::dynamic_pointer_cast<Parameter<std::string>>(
          m_node->GetParameter(name));
      widgets::field(
          label, fmt::format("{}_{}", m_node->GetName(), label),
          [&]() { return *v; }, [&](auto val) { par->SetValue(val); });
      return par;
    }

    PathParameterPtr
    PathParameter(std::string *v, const std::string &name,
                  const std::string &label,
                  const std::string &defaultValue = "") override {
      return nullptr;
    }


    FloatParameterPtr FloatParameter(float *v, const std::string &name,
                                     const std::string &label,
                                     float defaultValue = 0.0f) override {
      auto par =
          std::dynamic_pointer_cast<Parameter<float>>(m_node->GetParameter(name));
      widgets::field(
          label, fmt::format("{}_{}", m_node->GetName(), label),
          [&]() { return *v; }, [&](auto val) { par->SetValue(val); });
      return par;
    }

    IntParameterPtr IntegerParameter(int *v, const std::string &name,
                                     const std::string &label,
                                     int defaultValue = 0) override {
      auto par =
          std::dynamic_pointer_cast<Parameter<int>>(m_node->GetParameter(name));
      widgets::field(
          label, fmt::format("{}_{}", m_node->GetName(), label),
          [&]() { return *v; }, [&](auto val) { par->SetValue(val); });
      return par;
    }

    BooleanParameterPtr BooleanParameter(bool *v, const std::string &name,
                                         const std::string &label,
                                         bool defaultValue = false) override {
      auto par =
          std::dynamic_pointer_cast<Parameter<bool>>(m_node->GetParameter(name));
      widgets::field(
          label, fmt::format("{}_{}", m_node->GetName(), label),
          [&]() { return *v; }, [&](auto val) { par->SetValue(val); });
      return par;
    }

    IntParameterPtr EnumParameter(int *v, const std::vector<std::string> &values,
                                  const std::string &name,
                                  const std::string &label,
                                  int defaultValue = 0) override {
      auto par =
          std::dynamic_pointer_cast<Parameter<int>>(m_node->GetParameter(name));
      ImGui::PushItemWidth(50);
      static const char *items[50];
      for (auto i = 0; i < values.size(); ++i) {
        items[i] = values[i].c_str();
      }
      if (ImGui::BeginCombo(getImGuiLabel(label, name).c_str(), items[*v])) {
        for (int i = 0; i < values.size(); ++i) {
          const bool isSelected = i == *v;
          if (ImGui::Selectable(items[i], isSelected)) {
            *v = i;
            par->SetValue(*v);
          }
          if (isSelected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();
      return nullptr;
    }

    PlaneParameterPtr PlaneParameter(
        math::Plane<float> *v, const std::string &name, const std::string &label,
        math::Plane<float> defaultValue = math::Plane<float>{}) override {
      return nullptr;
    }

    NodePtr m_node;

  private:
    std::string getImGuiLabel(const std::string &label, const std::string name) {
      return fmt::format("{}##{}_{}", label, m_node->GetName(), name);
    }
  };

  GraphProperties::GraphProperties()
  {
  }

  GraphProperties::~GraphProperties()
  {
  }


  const std::string &GraphProperties::WindowName()
  {
    static const std::string name{"Graph Properties"};
    return name;
  }

  void GraphProperties::OpenNodeProperty(const pagoda::graph::NodePtr& n) {
    m_propertyNodes.push_back(n);
  }

  void GraphProperties::SetGraphDelegate(GraphDelegatePtr delegate) {
    m_graphDelegate = delegate;
  }

  bool GraphProperties::Draw() {
      std::set<NodePtr> toRemove;
      for (auto n : m_propertyNodes) {
        // Node name
        if (auto node = std::dynamic_pointer_cast<OperationNode>(n)) {

          const auto headerLabel = fmt::format(
              "{}##{}_properties", node->GetName(), node->GetId());
          bool visible = true;
          if (ImGui::CollapsingHeader(headerLabel.c_str(), &visible,
                                      ImGuiTreeNodeFlags_DefaultOpen)) { widgets::field( "Node Name", fmt::format("{}_nodename", node->GetId()),
                [&]() { return node->GetName(); },
                [&](auto val) {
                  if (m_graphDelegate != nullptr) {
                    m_graphDelegate->SetNodeText(reinterpret_cast<GraphDelegate::NodeHandle>(node.get()),
                        val);
                  }
                });

            ImGui::Separator();

            // Parameters
            CreateParameters parameters;
            parameters.m_node = node;
            node->GetOperation()->Parameters(&parameters);
          }

          if (!visible) {
            toRemove.insert(n);
          }
        }
    }

    for (const auto &n : toRemove) {
      m_propertyNodes.remove(n);
    }
    return true;
  }

}
