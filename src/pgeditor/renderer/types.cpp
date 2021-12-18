#include "types.h"

#include <pagoda/math/vec_base.h>

namespace pgeditor::renderer
{
// Vec3F
template<>
const std::size_t TypeTraits<pagoda::math::Vec3F>::componentsPerElement = 3;

template<>
const std::size_t TypeTraits<pagoda::math::Vec3F>::bytesPerComponent = sizeof(float);

template<>
const Format TypeTraits<pagoda::math::Vec3F>::format = Format::F32;

// Vec3F
template<>
const std::size_t TypeTraits<pagoda::math::Vec4F>::componentsPerElement = 4;

template<>
const std::size_t TypeTraits<pagoda::math::Vec4F>::bytesPerComponent = sizeof(float);

template<>
const Format TypeTraits<pagoda::math::Vec4F>::format = Format::F32;

// Float
template<>
const std::size_t TypeTraits<float>::componentsPerElement = 1;

template<>
const std::size_t TypeTraits<float>::bytesPerComponent = sizeof(float);

template<>
const Format TypeTraits<float>::format = Format::F32;

}  // namespace pgeditor::renderer
