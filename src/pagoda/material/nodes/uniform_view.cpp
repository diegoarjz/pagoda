#include "uniform_view.h"

#include <pagoda/common/types.h>

#include <pagoda/math/vec_base.h>

using namespace pagoda::common;
using namespace pagoda::math;

namespace pagoda::material::nodes {
UniformView::UniformView(const std::string &suffix, common::Type type,
                       common::Value defaultValue)
    : MaterialNode("uniform_view_" + suffix) {
  m_inputs = {
      {"inputUniform",
       {"inputUniform", type, defaultValue, {{HintKeys::uniform, ""}}}}};

  m_outputs = {{"result", {"result", type}}};
}

void UniformView::SetUniformName(const std::string& name)
{
  m_inputs["inputUniform"].hints["uniform"] = name;
}

const std::vector<std::string>& UniformView::InputOrder()
{
  static const std::vector<std::string> sInputOrder{"inputUniform"};
  return sInputOrder;
}

bool UniformView::SourceCode(ShaderSource &source) {
  if (source.TargetLanguage() == ShaderSource::ShadingLanguage::MSL) {
    source.EmitFunctionStart(InputOrder());
    source.EmitLine("  output.result = inputUniform;");
    source.EmitFunctionEnd();
    return true;
  }
  return false;
}
} // namespace pagoda::material::nodes

