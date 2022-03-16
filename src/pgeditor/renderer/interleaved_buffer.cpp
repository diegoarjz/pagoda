#include "interleaved_buffer.h"

#include "buffer.h"
#include "pagoda/common/debug/logger.h"

#include <iomanip>
#include <pagoda/common/debug/assertions.h>

namespace pgeditor::renderer {
int roundUp(int numToRound, int multiple) {
  if (multiple == 0) {
    return numToRound;
  }

  int remainder = numToRound % multiple;
  if (remainder == 0) {
    return numToRound;
  }

  return numToRound + multiple - remainder;
}

InterleavedBuffer::InterleavedBuffer(
    const std::vector<VertexAttributeDescription> &vertexDescription,
    const std::vector<const Buffer *> &buffers)
    : m_vertexDescription{vertexDescription} {
  DBG_ASSERT_MSG(buffers.size() > 0, "At least one buffer expected");
  DBG_ASSERT_MSG(vertexDescription.size() > 0,
                 "At least one vertex description expected");
  DBG_ASSERT_MSG(m_vertexDescription.size() == buffers.size(),
                 "Mismatch between number of vertex descriptions and buffers");

  if (buffers.empty() || vertexDescription.empty() ||
      buffers.size() != vertexDescription.size()) {
    return;
  }

  const std::size_t numBuffers = buffers.size();
  const std::size_t numElements = buffers[0]->GetNumElements();
  std::size_t elementSize = 0;
  std::size_t alignTo = 0;
  m_bufferSize = 0;
  for (std::size_t i = 0; i < numBuffers; ++i) {
    const auto &vert = m_vertexDescription[i];
    const auto &buffer = buffers[i];
    const auto &bytesPerBufferElement =
        vert.m_componentsPerElement * vert.m_bytesPerComponent;
    DBG_ASSERT(vert.m_bytesPerComponent == buffer->GetBytesPerComponent());
    DBG_ASSERT(vert.m_componentsPerElement ==
               buffer->GetComponentsPerElement());
    DBG_ASSERT(numElements == buffer->GetNumElements());

    m_bufferSize += buffer->GetBufferSize();
    elementSize += bytesPerBufferElement;
    alignTo = std::max(alignTo, bytesPerBufferElement);
  }

  m_bufferSize = numElements * roundUp(elementSize, alignTo);
  m_data = new std::byte[m_bufferSize];
  std::memset(m_data, 0, m_bufferSize);

  std::size_t remainingSize = m_bufferSize;
  void *dst = m_data;
  for (std::size_t e = 0; e < numElements; ++e) {
    for (std::size_t b = 0; b < numBuffers; ++b) {
      const auto &buffer = buffers[b];
      const std::size_t elmSize =
          buffer->GetBytesPerComponent() * buffer->GetComponentsPerElement();

      if (auto ptr = std::align(alignTo, elmSize, dst, remainingSize)) {
        std::memcpy(static_cast<std::byte *>(ptr),
                    &buffer->GetData()[e * elmSize], elmSize);

        dst = static_cast<std::byte *>(dst) + elmSize;
        remainingSize -= elmSize;
      } else {
        LOG_FATAL("Not enough space in buffer to align");
      }
    }
  }
}

InterleavedBuffer::InterleavedBuffer(InterleavedBuffer &&b) {
  if (this != &b) {
    m_data = b.m_data;
    b.m_data = nullptr;
    m_bufferSize = b.m_bufferSize;
    m_vertexDescription = std::move(b.m_vertexDescription);
  }
}

InterleavedBuffer::~InterleavedBuffer() { Free(); }

const std::byte *const InterleavedBuffer::GetData() const { return m_data; }
std::size_t InterleavedBuffer::GetSize() const { return m_bufferSize; }

void InterleavedBuffer::Free() {
  if (m_data != nullptr) {
    delete[] m_data;
  }
}

} // namespace pgeditor::renderer
