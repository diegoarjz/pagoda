#include "render_state.h"

namespace pgeditor::renderer {

namespace {
template <typename T>
void compare(RenderState::Diff_t &d, const T &lhs, const T &rhs,
             RenderState::State s) {
  if (lhs != rhs) {
    d |= s;
  }
}
} // namespace

RenderState::Diff_t RenderState::ComputeDiff(const RenderState &o) {
  Diff_t diff;

  compare(diff, depthTestEnabled, o.depthTestEnabled, State::DepthTest);
  compare(diff, depthWriteEnabled, o.depthWriteEnabled, State::DepthWrite);
  compare(diff, depthFunc, o.depthFunc, State::DepthFunc);

  return diff;
}
} // namespace pgeditor::renderer
