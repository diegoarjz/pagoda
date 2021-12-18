#pragma once

#include "types.h"

#include <cstddef>
#include <vector>

namespace pgeditor::renderer
{
class UniformBuffer
{
  public:
  UniformBuffer();
  ~UniformBuffer();

  /// Move constructor
  UniformBuffer(UniformBuffer&& b);

  /// Non-copyable
  UniformBuffer(const UniformBuffer&) = delete;
  UniformBuffer& operator=(const UniformBuffer) = delete;

  const std::byte* const GetData() const;
  std::size_t GetSize() const;
  void Free();

  template<typename T>
  void Add(const T& value)
  {
    const TypeDescription desc = TypeTraits<T>::Description();
    const std::size_t valueSize = desc.componentsPerElement * desc.bytesPerComponent;

    if (m_data == nullptr) {
      m_data = new std::byte[valueSize];
      m_reservedSize = valueSize;
    }
    else if (m_bufferSize + valueSize >= m_reservedSize) {
      std::byte* newBuffer = new std::byte[m_reservedSize * 2];
      std::memcpy(newBuffer, m_data, m_reservedSize);
      delete []m_data;
      m_data = newBuffer;
      m_reservedSize *= 2;
    }

    std::memcpy(m_data + m_bufferSize, &value, valueSize);
    m_bufferSize += valueSize;
    m_storedTypes.push_back(desc);
  }

  private:
  std::byte* m_data{nullptr};
  std::size_t m_reservedSize{0};
  std::size_t m_bufferSize{0};
  std::vector<TypeDescription> m_storedTypes;
};
}
