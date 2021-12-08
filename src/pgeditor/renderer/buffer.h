#pragma once

#include "types.h"

#include <typeinfo>
#include <vector>

namespace pgeditor::renderer
{
/**
 * Holds data of different types.
 */
class Buffer
{
	public:
	/// Initializes with no data an invalid states.
	Buffer();
	/// Create a Buffer with the given data.
	template<class T>
	Buffer(const std::vector<T>& data)
	{
		SetData(data);
	}
	/// Moves the data between buffers.
	Buffer(Buffer&& b);
	/// Destroys and frees up the data
	~Buffer();

	/// Returns the storage format
	Format GetFormat() const;

	/// How many components has each separate element.
	/// E.g. Vec3F has 3
	std::size_t GetComponentsPerElement() const;
	/// How many bytes has each component.
	std::size_t GetBytesPerComponent() const;
	/// The size of the buffer
	std::size_t GetBufferSize() const;
	/// The number of elements
	std::size_t GetNumElements() const;

	/// Returns a pointer to the data.
	const std::byte* const GetData() const;

	/// Checks whether the buffer is holding values of type T
	template<class T>
	bool IsHolding() const
	{
		return m_typeHash == typeid(T).hash_code();
	}

	/// Returns a pointer to the data, cast to a specific type.
	/// If T is different from the holding type, returns nullptr.
	template<class T>
	const T* const GetArrayAs() const
	{
		if (!IsHolding<T>()) {
			return nullptr;
		}
		return reinterpret_cast<T*>(m_data);
	}

	/// Sets the data to the contents of \p data
	/// Frees the buffer before.
	template<class T>
	void SetData(const std::vector<T>& data)
	{
		const std::size_t newComponentsPerElement = BufferTypeTraits<T>::componentsPerElement;
		const std::size_t newBytesPerComponent = BufferTypeTraits<T>::bytesPerComponent;

		if (m_data != nullptr) {
			delete[] reinterpret_cast<std::byte*>(m_data);
		}

		const std::size_t newBufferSize = data.size() * newComponentsPerElement * newBytesPerComponent;
		m_componentsPerElement = newComponentsPerElement;
		m_bytesPerComponent = newBytesPerComponent;
		m_bufferSize = newBufferSize;
		m_format = BufferTypeTraits<T>::format;
		m_numElements = data.size();

		m_data = new std::byte[newBufferSize];
		std::memcpy(m_data, data.data(), m_bufferSize);

		m_typeHash = typeid(T).hash_code();
	}

	/// Returns the element at \p if the buffer is holding the same type as T.
	template<class T>
	const T* GetAt(std::size_t i) const
	{
		if (m_data == nullptr) {
			return nullptr;
		}

		if (!IsHolding<T>()) {
			return nullptr;
		}

		const std::size_t componentsPerElement = BufferTypeTraits<T>::componentsPerElement;
		const std::size_t bytesPerComponent = BufferTypeTraits<T>::bytesPerComponent;
		const std::size_t offset = i * componentsPerElement * bytesPerComponent;

		if (offset > m_bufferSize) {
			return nullptr;
		}
		return &reinterpret_cast<T*>(m_data)[i];
	}

	/// Returns the contents as a vector of the given type.
	template<class T>
	std::vector<T> GetAsVector() const
	{
		if (!IsHolding<T>()) {
			return {};
		}
		std::vector<T> vec;
		vec.reserve(m_numElements);
		for (std::size_t i = 0; i < m_numElements; ++i) {
			vec.push_back(*GetAt<T>(i));
		}
		return vec;
	}

	/// Frees the buffer
	void Free();

	private:
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	Format m_format{Format::Invalid};

	std::size_t m_componentsPerElement{0};
	std::size_t m_bytesPerComponent{0};
	std::size_t m_bufferSize{0};
	std::size_t m_numElements{0};

	std::byte* m_data{nullptr};

	std::size_t m_typeHash;
};

}  // namespace pgeditor::renderer
