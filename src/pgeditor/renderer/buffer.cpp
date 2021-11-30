#include "buffer.h"

#include <pagoda/math/vec_base.h>

namespace pgeditor::renderer
{
// Vec3F
template<>
const std::size_t BufferTypeTraits<pagoda::math::Vec3F>::componentsPerElement = 3;

template<>
const std::size_t BufferTypeTraits<pagoda::math::Vec3F>::bytesPerComponent = sizeof(float);

template<>
const Format BufferTypeTraits<pagoda::math::Vec3F>::format = Format::F32;

// Vec3F
template<>
const std::size_t BufferTypeTraits<pagoda::math::Vec4F>::componentsPerElement = 4;

template<>
const std::size_t BufferTypeTraits<pagoda::math::Vec4F>::bytesPerComponent = sizeof(float);

template<>
const Format BufferTypeTraits<pagoda::math::Vec4F>::format = Format::F32;

// Float
template<>
const std::size_t BufferTypeTraits<float>::componentsPerElement = 1;

template<>
const std::size_t BufferTypeTraits<float>::bytesPerComponent = sizeof(float);

template<>
const Format BufferTypeTraits<float>::format = Format::F32;
Buffer::Buffer()
{
	//
}

Buffer::Buffer(Buffer&& b)
{
	m_data = b.m_data;
	m_varyingType = b.m_varyingType;
	m_format = b.m_format;
	m_componentsPerElement = b.m_componentsPerElement;
	m_bytesPerComponent = b.m_bytesPerComponent;
	m_bufferSize = b.m_bufferSize;
	m_typeHash = b.m_typeHash;
}

Buffer::~Buffer() { Free(); }

void Buffer::Free()
{
	if (m_data != nullptr) {
		delete[] static_cast<char*>(m_data);
	}
	m_data = nullptr;

	m_varyingType = VaryingType::Invalid;
	m_format = Format::Invalid;
	m_componentsPerElement = 0;
	m_bytesPerComponent = 0;
	m_bufferSize = 0;
	m_typeHash = 0;
}

VaryingType Buffer::GetVaryingType() const { return m_varyingType; }
Format Buffer::GetFormat() const { return m_format; }
std::size_t Buffer::GetComponentsPerElement() const { return m_componentsPerElement; }
std::size_t Buffer::GetBytesPerComponent() const { return m_bytesPerComponent; }
std::size_t Buffer::GetBufferSize() const { return m_bufferSize; }
std::size_t Buffer::GetNumElements() const { return m_numElements; }
const void* const Buffer::GetData() const { return m_data; }
}  // namespace pgeditor::renderer
