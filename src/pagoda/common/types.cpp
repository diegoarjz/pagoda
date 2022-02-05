#include "types.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/math/vec_base.h>

#include <unordered_map>

using namespace pagoda::math;

namespace pagoda::common {

static const std::unordered_map<std::size_t, Type> sTypeInfoToTypeTable = {
    {typeid(int32_t).hash_code(), Type::Int},
    {typeid(Vec2I).hash_code(), Type::Int2},
    {typeid(uint32_t).hash_code(), Type::UInt},
    {typeid(Vec2U).hash_code(), Type::UInt2},
    {typeid(float).hash_code(), Type::Float},
    {typeid(double).hash_code(), Type::Double},
    {typeid(Vec3F).hash_code(), Type::Vec3},
    {typeid(Vec2F).hash_code(), Type::Vec2},
    {typeid(Vec4F).hash_code(), Type::Vec4},
    {typeid(Mat2x2F).hash_code(), Type::Mat2},
    {typeid(Mat3x3F).hash_code(), Type::Mat3},
    {typeid(Mat4x4F).hash_code(), Type::Mat4},
    {typeid(std::string).hash_code(), Type::String},
};

static const std::unordered_map<Type, TypeDescription> sDescriptionTable = {
    {Type::Int, {Format::I32, 1, sizeof(int)}},
    {Type::Int2, {Format::I32, 2, sizeof(int32_t)}},
    {Type::UInt, {Format::U32, 1, sizeof(uint32_t)}},
    {Type::UInt2, {Format::U32, 2, sizeof(uint32_t)}},
    {Type::Float, {Format::F32, 1, sizeof(float)}},
    {Type::Double, {Format::F64, 1, sizeof(double)}},
    {Type::Vec2, {Format::F32, 2, sizeof(float)}},
    {Type::Vec3, {Format::F32, 3, sizeof(float)}},
    {Type::Vec4, {Format::F32, 4, sizeof(float)}},
    {Type::Mat2, {Format::F32, 4, sizeof(float)}},
    {Type::Mat3, {Format::F32, 9, sizeof(float)}},
    {Type::Mat4, {Format::F32, 16, sizeof(float)}},
    {Type::String, {Format::U32, 1, sizeof(std::string)}}};

TypeDescription TypeDescription::Description(Type type) {
  auto iter = sDescriptionTable.find(type);
  DBG_ASSERT_MSG(iter != sDescriptionTable.end(),
                 "Description not found for type");
  return iter->second;
}

////////////////////////////////////////
// Int32
////////////////////////////////////////
template <> const std::size_t TypeTraits<int32_t>::componentsPerElement = 1;

template <>
const std::size_t TypeTraits<int32_t>::bytesPerComponent = sizeof(int32_t);

template <> const Format TypeTraits<int32_t>::format = Format::I32;

////////////////////////////////////////
// UInt32
////////////////////////////////////////
template <> const std::size_t TypeTraits<uint32_t>::componentsPerElement = 1;

template <>
const std::size_t TypeTraits<uint32_t>::bytesPerComponent = sizeof(uint32_t);

template <> const Format TypeTraits<uint32_t>::format = Format::U32;

////////////////////////////////////////
// Float
////////////////////////////////////////
template <> const std::size_t TypeTraits<float>::componentsPerElement = 1;

template <>
const std::size_t TypeTraits<float>::bytesPerComponent = sizeof(float);

template <> const Format TypeTraits<float>::format = Format::F32;

////////////////////////////////////////
// Vec3F
////////////////////////////////////////
template <>
const std::size_t TypeTraits<pagoda::math::Vec3F>::componentsPerElement = 3;

template <>
const std::size_t
    TypeTraits<pagoda::math::Vec3F>::bytesPerComponent = sizeof(float);

template <> const Format TypeTraits<pagoda::math::Vec3F>::format = Format::F32;

////////////////////////////////////////
// Vec3F
////////////////////////////////////////
template <>
const std::size_t TypeTraits<pagoda::math::Vec4F>::componentsPerElement = 4;

template <>
const std::size_t
    TypeTraits<pagoda::math::Vec4F>::bytesPerComponent = sizeof(float);

template <> const Format TypeTraits<pagoda::math::Vec4F>::format = Format::F32;

////////////////////////////////////////
// Mat4x4F
////////////////////////////////////////
template <>
const std::size_t TypeTraits<pagoda::math::Mat4x4F>::componentsPerElement = 16;

template <>
const std::size_t
    TypeTraits<pagoda::math::Mat4x4F>::bytesPerComponent = sizeof(float);

template <>
const Format TypeTraits<pagoda::math::Mat4x4F>::format = Format::F32;

} // namespace pgeditor::renderer
