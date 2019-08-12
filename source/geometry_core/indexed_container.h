#ifndef SELECTOR_GEOMETRY_CORE_INDEXED_CONTAINTER_H_
#define SELECTOR_GEOMETRY_CORE_INDEXED_CONTAINTER_H_

#include <common/assertions.h>
#include <common/profiler.h>
#include <common/range.h>

#include <unordered_map>

#include <vector>

namespace selector
{
template<class IndexType, class ValueType>
class IndexedContainer
{
public:
	// TODO: Create an index iterator that iterates over the available indices
	using ContainerType = std::vector<ValueType>;
	using index_iterator = typename Range<size_t>::iterator;
	using iterator = typename ContainerType::iterator;
	using const_iterator = typename ContainerType::const_iterator;
	using IndexValuePair_t = std::pair<IndexType, ValueType&>;
	
	ValueType& Get(const IndexType& index)
	{
		START_PROFILE;

		DBG_ASSERT(index < m_container.size());
		return m_container[index];
	}

	const ValueType& Get(const IndexType& index) const
	{
		START_PROFILE;

		DBG_ASSERT(index < m_container.size());
		return m_container.at(index);
	}

	template<typename... Args>
	IndexType Create(Args... args)
	{
		START_PROFILE;

		m_container.emplace_back(args...);

		return static_cast<IndexType>(m_container.size() - 1);
	}
	
	template<typename... Args>
	IndexValuePair_t CreateAndGet(Args... args)
	{
		START_PROFILE;
		
		auto& value = m_container.emplace_back(args...);
		
		return std::make_pair(static_cast<IndexType>(m_container.size() - 1), value);
	}
	
	index_iterator index_begin() const { return Range<size_t>(m_container.size()).begin(); }
	index_iterator index_end() const { return Range<size_t>(m_container.size()).end(); }
	iterator begin() { return m_container.begin(); }
	iterator end() { return m_container.end(); }
	const_iterator begin() const { return m_container.begin(); }
	const_iterator end() const { return m_container.end(); }

	uint32_t size() const { return static_cast<uint32_t>(m_container.size()); }
	void reserve(const size_t& new_size) { m_container.reserve(new_size); }
	uint32_t capacity() const { return m_container.capacity(); }

private:
	ContainerType m_container;
};  // class IndexedContainer

template<class IndexType, class ValueType>
class AssociativeIndexedContainer
{
public:
	using ContainerType = std::unordered_map<IndexType, ValueType>;
	using iterator = typename ContainerType::iterator;
	using const_iterator = typename ContainerType::const_iterator;
	using IndexValuePair_t = std::pair<IndexType, ValueType&>;

	AssociativeIndexedContainer() : m_nextIndex(IndexType()) {}

	ValueType& Get(const IndexType& index)
	{
		START_PROFILE;

		DBG_ASSERT(m_container.find(index) != std::end(m_container));
		return m_container.at(index);
	}

	const ValueType& Get(const IndexType& index) const
	{
		START_PROFILE;

		DBG_ASSERT(m_container.find(index) != std::end(m_container));
		return m_container.at(index);
	}

	template<typename... Args>
	IndexType Create(Args... args)
	{
		START_PROFILE;

		IndexType new_index = CreateIndex();
		m_container[new_index] = ValueType(args...);

		return new_index;
	}
	
	template<typename... Args>
	IndexValuePair_t CreateAndGet(Args.. args)
	{
		START_PROFILE;
		
		IndexType newIndex = CreateIndex();
		auto insertResult = m_container.emplace(args);
		// TODO: Check if the value was indeed inserted.
		
		return std::make_pair(newIndex, insertResult.second);
	}
	
	void Delete(const IndexType &index)
	{
		m_container.erase(index);
	}
	
	bool HasIndex(const IndexType &index)
	{
		return m_container.find(index) != std::end(m_container);
	}

	iterator begin() { return m_container.begin(); }
	iterator end() { return m_container.end(); }
	const_iterator begin() const { return m_container.begin(); }
	const_iterator end() const { return m_container.end(); }

private:
	IndexType CreateIndex() { return m_nextIndex++; }

	IndexType m_nextIndex;
	ContainerType m_container;
};  // class AssociativeIndexedContainer
}  // namespace selector

#endif
