#include "uniform_buffer.h"

namespace pgeditor::renderer
{
UniformBuffer::UniformBuffer()
{

}

UniformBuffer::~UniformBuffer()
{
  Free();
}

UniformBuffer::UniformBuffer(UniformBuffer&& b)
  : m_data{b.m_data},
    m_reservedSize{b.m_reservedSize},
    m_bufferSize{b.m_bufferSize},
    m_storedTypes{std::move(b.m_storedTypes)}
{

}

const std::byte* const UniformBuffer::GetData() const { return m_data; }

std::size_t UniformBuffer::GetSize() const { return m_bufferSize; }

void UniformBuffer::Free()
{
  if (m_data != nullptr) {
    delete []m_data;
  }
}
}
