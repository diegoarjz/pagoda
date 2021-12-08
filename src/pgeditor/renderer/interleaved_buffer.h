#pragma once

#include "vertex_attribute.h"

#include <cstddef>
#include <vector>

namespace pgeditor::renderer
{
class Buffer;

/**
 * Class to interleave Buffer data based on VertexAttributeDescription.
 */
class InterleavedBuffer
{
	public:
	/// Constructor with a description and the buffers.
	InterleavedBuffer(const std::vector<VertexAttributeDescription>& vertexDescription,
	                  const std::vector<const Buffer*>& buffers);

	/// Move constructor
	InterleavedBuffer(InterleavedBuffer&& b);

	~InterleavedBuffer();

	/// Gets a pointer to the data.
	const std::byte* const GetData() const;
	/// Returns the buffer size.
	std::size_t GetSize() const;

	/// Deallocates this buffer data.
	void Free();

	private:
	InterleavedBuffer(const InterleavedBuffer&) = delete;
	InterleavedBuffer& operator=(const InterleavedBuffer&) = delete;

	std::byte* m_data{nullptr};
	std::size_t m_bufferSize{0};
	std::vector<VertexAttributeDescription> m_vertexDescription;
};
}  // namespace pgeditor::renderer
