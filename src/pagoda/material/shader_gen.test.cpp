#include "shader_gen.h"

#include "material_network.h"
#include "material_node.h"
#include "material_node_registry.h"

#include "nodes/buffer_view.h"
#include "nodes/default_frag.h"
#include "nodes/default_vert.h"
#include "nodes/uniform_view.h"

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::material;
using namespace pagoda::material::nodes;

class ShaderGenTest : public ::testing::Test {
public:
  void SetUp() override {
    m_registry = std::make_shared<MaterialNodeRegistry>();
    m_registry->Register("default_vert", []() {
      return std::make_shared<nodes::DefaultVert>();
    });
    m_registry->Register("default_frag", []() {
      return std::make_shared<nodes::DefaultFrag>();
    });
    m_registry->Register("buffer_view_mat4", []() {
      static pagoda::math::Mat4x4F identity =
          boost::qvm::identity_mat<float, 4>();
      return std::make_shared<nodes::BufferView>("mat4", common::Type::Mat4,
                                                 identity);
    });
    m_registry->Register("uniform_view_mat4", []() {
      static pagoda::math::Mat4x4F identity =
          boost::qvm::identity_mat<float, 4>();
      return std::make_shared<nodes::UniformView>("mat4", common::Type::Mat4,
                                                  identity);
    });

    m_material = std::make_shared<MaterialNetwork>(m_registry.get(), "network");

    m_defaultVert = m_material->CreateMaterialNode("default_vert", "vert");

    m_material->SetStageTerminalNode(MaterialNetwork::ShaderStage::Vertex,
                                     "vert");

    m_material->CreateMaterialNode("default_frag", "frag");
    m_material->SetStageTerminalNode(MaterialNetwork::ShaderStage::Fragment,
                                     "frag");
  }

  std::shared_ptr<MaterialNodeRegistry> m_registry;
  std::shared_ptr<MaterialNetwork> m_material;
  std::shared_ptr<MaterialNode> m_defaultVert;
};


TEST_F(ShaderGenTest, Test1) {
  ShaderGen gen(*m_material);

  {
    std::stringstream s;
    auto shaderSource = ShaderSource::Create(ShaderSource::ShadingLanguage::MSL, s);

    auto vertShader = gen.GetStage(MaterialNetwork::ShaderStage::Vertex);
    vertShader->GetSource(*shaderSource);

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Vert Shader" << std::endl;
    std::cout << s.str() << std::endl;
  }

  {
    std::stringstream s;
    auto shaderSource = ShaderSource::Create(ShaderSource::ShadingLanguage::MSL, s);

    auto fragShader = gen.GetStage(MaterialNetwork::ShaderStage::Fragment);
    fragShader->GetSource(*shaderSource);

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Frag Shader" << std::endl;
    std::cout << s.str() << std::endl;
  }
}
