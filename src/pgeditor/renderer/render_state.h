#pragma once

#include <bitset>

namespace pgeditor::renderer {
enum class DepthFunc {
  Never,     // A new value never passes the comparison test.
  Less,      // A new value passes the comparison test if it is less than the
             // existing value.
  Equal,     // A new value passes the comparison test if it is equal to the
             // existing value.
  LessEqual, // A new value passes the comparison test if it is less than or
             // equal to the existing value.
  Greater,   // A new value passes the comparison test if it is greater than the
             // existing value.
  NotEqual,  // A new value passes the comparison test if it is not equal to the
             // existing value.
  GreaterEqual, // A new value passes the comparison test if it is greater than
                // or equal to the existing value.
  Always,       // A new value always passes the comparison test.
};

struct RenderState {
  enum State {
    // Depth State
    DepthTest,
    DepthWrite,
    DepthFunc,
    // Count
    NumStates
  };

  using Diff_t = std::bitset<NumStates>;

  ////////////////////////////////////////
  // Depth State
  ////////////////////////////////////////
  bool depthTestEnabled{true};
  bool depthWriteEnabled{true};
  enum DepthFunc depthFunc { DepthFunc::LessEqual };

  Diff_t ComputeDiff(const RenderState &o);
};
} // namespace pgeditor::renderer
