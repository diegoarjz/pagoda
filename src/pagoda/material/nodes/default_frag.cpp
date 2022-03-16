#include "default_frag.h"

#include <pagoda/math/vec_base.h>
#include <pagoda/math/matrix_base.h>

using namespace pagoda::math;

namespace pagoda::material::nodes {
DefaultFrag::DefaultFrag()
    : MaterialNode(nodeName) {

  m_inputs = {
    {"normal", {"normal", Type_t::Vec3, Vec3F{0,0,0}, {{HintKeys::stageIn, "normal"}}}},
    {"color", {"color", Type_t::Vec4, Vec4F{0,0,0,0}, {{HintKeys::uniform, "color"}}}}
  };

  m_outputs = {
    {"color", {"color", Type_t::Vec4, {{HintKeys::stageOut, "color"}}}}
  };
}

DefaultFrag::~DefaultFrag()
{
}

const std::vector<std::string>& DefaultFrag::InputOrder()
{
  static const std::vector<std::string> sInputOrder{"normal", "color"};
  return sInputOrder;
}

bool DefaultFrag::SourceCode(ShaderSource &source) {
  if (source.TargetLanguage() == ShaderSource::ShadingLanguage::MSL) {
    source.EmitFunctionStart(InputOrder());
    source.EmitLine("  default_frag_output output;");
    source.EmitLine("  output.color = float4(normal, 1.0);");
    source.EmitText("  return output;");
    source.EmitFunctionEnd();
    return true;
  }
  return false;
}
} // namespace papgoda::material::nodes

