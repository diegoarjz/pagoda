#include "buffer_view.h"

#include <pagoda/common/types.h>

#include <pagoda/math/vec_base.h>

using namespace pagoda::common;
using namespace pagoda::math;

namespace pagoda::material::nodes {
BufferView::BufferView(const std::string &suffix, common::Type type,
                       common::Value defaultValue)
    : MaterialNode("buffer_view_" + suffix) {
  m_inputs = {
      {"inputBuffer",
       {"inputBuffer", type, defaultValue, {{HintKeys::stageIn, ""}}}}};

  m_outputs = {{"result", {"result", type}}};
}

void BufferView::SetVertexAttributeName(const std::string& name)
{
  m_inputs["inputBuffer"].hints["vertex-attribute"] = name;
}

const std::vector<std::string>& BufferView::InputOrder()
{
  static const std::vector<std::string> sInputOrder{"inputBuffer"};
  return sInputOrder;
}

bool BufferView::SourceCode(ShaderSource &source) {
  if (source.TargetLanguage() == ShaderSource::ShadingLanguage::MSL) {
    source.EmitFunctionStart(InputOrder());
    source.EmitLine("  output.result = inputBuffer;");
    source.EmitFunctionEnd();
    return true;
  }
  return false;
}
} // namespace pagoda::material::nodes
