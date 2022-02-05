#include "buffer.h"

#include <pagoda/math/vec_base.h>

namespace pgeditor::renderer
{
Buffer::Buffer()
{
	//
}

Buffer::Buffer(Buffer&& b)
{
	if (this != &b) {
		m_data = b.m_data;
		b.m_data = nullptr;
		m_format = b.m_format;
		m_componentsPerElement = b.m_componentsPerElement;
		m_bytesPerComponent = b.m_bytesPerComponent;
		m_bufferSize = b.m_bufferSize;
		m_typeHash = b.m_typeHash;
	}
}

Buffer::~Buffer() { Free(); }

void Buffer::Free()
{
	if (m_data != nullptr) {
		delete[] static_cast<std::byte*>(m_data);
	}
	m_data = nullptr;

	m_format = pagoda::common::Format::Invalid;
	m_componentsPerElement = 0;
	m_bytesPerComponent = 0;
	m_bufferSize = 0;
	m_typeHash = 0;
}

pagoda::common::Format Buffer::GetFormat() const { return m_format; }
std::size_t Buffer::GetComponentsPerElement() const { return m_componentsPerElement; }
std::size_t Buffer::GetBytesPerComponent() const { return m_bytesPerComponent; }
std::size_t Buffer::GetBufferSize() const { return m_bufferSize; }
std::size_t Buffer::GetNumElements() const { return m_numElements; }
const std::byte* const Buffer::GetData() const { return m_data; }
}  // namespace pgeditor::renderer
