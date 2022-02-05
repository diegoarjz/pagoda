#pragma once

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

#include <cstddef>
#include <variant>

namespace pagoda::common {
// clang-format off
using Value = std::variant<int,
                          float,
                          double,
                          pagoda::math::Vec2F,
                          pagoda::math::Vec3F,
                          pagoda::math::Vec4F,
                          pagoda::math::Mat2x2F,
                          pagoda::math::Mat3x3F,
                          pagoda::math::Mat4x4F,
                          std::string>;
// clang-format on

enum class Format { I32, U32, F32, F64, String, Invalid };

enum class Type {
  Int,
  Int2,
  UInt,
  UInt2,
  Float,
  Double,
  Vec2,
  Vec3,
  Vec4,
  Mat2,
  Mat3,
  Mat4,
  String
};

struct TypeDescription {
  Format m_format;
  std::size_t componentsPerElement;
  std::size_t bytesPerComponent;

  static TypeDescription Description(Type type);
};

template <class T> struct TypeTraits {
  static const Format format;
  static const std::size_t componentsPerElement;
  static const std::size_t bytesPerComponent;

  static TypeDescription Description() {
    return TypeDescription{format, componentsPerElement, bytesPerComponent};
  }
};

} // namespace pgeditor::renderer
