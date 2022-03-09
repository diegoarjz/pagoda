#include "default_vert.h"

#include <pagoda/math/vec_base.h>
#include <pagoda/math/matrix_base.h>

using namespace pagoda::math;

namespace pagoda::material::nodes {
DefaultVert::DefaultVert()
    : MaterialNode(nodeName) {

  static Mat4x4F identity = boost::qvm::identity_mat<float, 4>();

  m_inputs = {
    {"position", {"position", Type_t::Vec4, Vec4F{0,0,0,1}, {{HintKeys::stageIn, "position"}}}},
    {"modelMatrix", {"modelMatrix", Type_t::Mat4, identity, {{HintKeys::uniform, "modelMatrix"}}}},
    {"viewMatrix", {"viewMatrix", Type_t::Mat4, identity, {{HintKeys::uniform, "viewMatrix"}}}},
    {"projMatrix", {"projMatrix", Type_t::Mat4, identity, {{HintKeys::uniform, "projectionMatrix"}}}},
  };

  m_outputs = {
    {"position", {"position", Type_t::Vec4, {{HintKeys::stageOut, "position"}, {HintKeys::stageOutAttribute, "position"}}}},
    {"normal", {"normal", Type_t::Vec3, {{HintKeys::stageOut, "normal"}}}}
  };
}

DefaultVert::~DefaultVert()
{
}

const std::vector<std::string>& DefaultVert::InputOrder()
{
  static const std::vector<std::string> sInputOrder{"position", "modelMatrix", "viewMatrix", "projMatrix"};
  return sInputOrder;
}

bool DefaultVert::SourceCode(ShaderSource &source) {
  if (source.TargetLanguage() == ShaderSource::ShadingLanguage::MSL) {
    source.EmitFunctionStart(InputOrder());
    source.EmitLine("  float4x4 mvp = projMatrix * viewMatrix * modelMatrix;");
    source.EmitLine("  default_vert_output output;");
    source.EmitLine("  output.position = mvp * position;");
    source.EmitLine("  output.normal = float3(1,0,0);");
    source.EmitText("  return output;");
    source.EmitFunctionEnd();
  }
  return false;
}
} // namespace papgoda::material::nodes
