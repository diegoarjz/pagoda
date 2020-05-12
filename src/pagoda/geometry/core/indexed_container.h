#ifndef PAGODA_GEOMETRY_CORE_INDEXED_CONTAINTER_H_
#define PAGODA_GEOMETRY_CORE_INDEXED_CONTAINTER_H_

#include "pagoda/common/debug/assertions.h"
#include "pagoda/common/exception/exception.h"
#include "pagoda/common/instrument/profiler.h"
#include "pagoda/common/range.h"

#include <unordered_map>

#include <vector>

namespace pagoda::geometry::core
{
template<class IndexType>
class IndexedDeletedException : public common::exception::Exception
{
public:
	IndexedDeletedException(const IndexType& i)
	    : common::exception::Exception("Tried to access deleted index " + std::to_string(i))
	{
	}
};

template<class IndexType, class ValueType>
class IndexedContainer
{
private:
	using IndexedContainer_t = IndexedContainer<IndexType, ValueType>;

public:
	struct IndexValuePair
	{
		IndexValuePair(const IndexType& i, ValueType& v) : m_index(i), m_value(v) {}

		IndexType m_index;
		ValueType& m_value;
	};

	using ContainerType = std::vector<ValueType>;

	IndexedContainer() : m_nextIndex(0), m_count(0) {}

	ValueType& Get(const IndexType& index)
	{
		START_PROFILE;
		DBG_ASSERT(index < m_container.size());
		if (!m_validIndex[index])
		{
			throw IndexedDeletedException(index);
		}
		return m_container.at(index);
	}

	const ValueType& Get(const IndexType& index) const
	{
		START_PROFILE;
		DBG_ASSERT(index < m_container.size());
		if (!m_validIndex[index])
		{
			throw IndexedDeletedException(index);
		}
		return m_container.at(index);
	}

	template<typename... Args>
	IndexValuePair CreateAndGet(Args... args)
	{
		START_PROFILE;
		auto newIndex = Create(args...);
		return IndexValuePair{static_cast<IndexType>(newIndex), Get(newIndex)};
	}

	template<typename... Args>
	IndexType Create(Args... args)
	{
		START_PROFILE;
		IndexType index = CreateAtIndex(CreateIndex(), ValueType(args...));
		return index;
	}

	ValueType& GetOrCreate(const IndexType& index, const ValueType& v = ValueType())
	{
		START_PROFILE;
		if (HasIndex(index))
		{
			return Get(index);
		}

		CreateAtIndex(index, v);
		return Get(index);
	}

	void Delete(const IndexType& index)
	{
		if (m_container.size() <= index)
		{
			return;
		}
		--m_count;
		m_validIndex[index] = false;
	}

	bool HasIndex(const IndexType& index) { return m_container.size() > index && m_validIndex[index]; }

	std::size_t Count() const { return m_count; }

	class iterator
	{
	public:
		iterator(IndexedContainer_t& container, const std::size_t& index)
		    : m_container(container), m_currentIndex(FindNextValid(index))
		{
		}

		IndexValuePair operator*()
		{
			return IndexValuePair{static_cast<IndexType>(m_currentIndex), m_container.m_container.at(m_currentIndex)};
		}

		iterator& operator++()
		{
			m_currentIndex = FindNextValid(m_currentIndex + 1);
			return *this;
		}

		iterator& operator++(int)
		{
			m_currentIndex = FindNextValid(m_currentIndex + 1);
			return *this;
		}

		bool operator==(const iterator& other) { return m_currentIndex == other.m_currentIndex; }

		bool operator!=(const iterator& other) { return m_currentIndex != other.m_currentIndex; }

	private:
		std::size_t FindNextValid(const std::size_t index) const
		{
			for (auto i = index; i < m_container.m_container.size(); ++i)
			{
				if (m_container.m_validIndex[i])
				{
					return i;
				}
			}
			return m_container.m_container.size();
		}

		IndexedContainer_t& m_container;
		std::size_t m_currentIndex;
	};

	iterator begin() { return iterator(*this, 0); }
	iterator end() { return iterator(*this, m_container.size()); }

private:
	IndexType CreateAtIndex(const IndexType& index, const ValueType& v)
	{
		if (m_container.size() <= index)
		{
			m_container.resize(index * 2 + 1);
			m_validIndex.resize(index * 2 + 1);
		}
		DBG_ASSERT(index < m_container.size());
		++m_count;
		m_container[index] = v;
		m_validIndex[index] = true;
		return index;
	}

	IndexType CreateIndex()
	{
		while (m_nextIndex < m_container.size() && m_validIndex[m_nextIndex])
		{
			++m_nextIndex;
		}
		return m_nextIndex;
	}

	IndexType m_nextIndex;
	ContainerType m_container;
	std::vector<bool> m_validIndex;
	std::size_t m_count;
};

template<class IndexType, class ValueType>
class AssociativeIndexedContainer
{
public:
	using IndexValuePair_t = std::pair<IndexType, ValueType&>;
	using ContainerType = std::unordered_map<IndexType, ValueType>;
	using iterator = typename ContainerType::iterator;
	using const_iterator = typename ContainerType::const_iterator;

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
	IndexValuePair_t CreateAndGet(Args... args)
	{
		START_PROFILE;

		IndexType newIndex = CreateIndex();
		ValueType value(args...);
		auto insertResult = m_container.emplace(newIndex, value);
		DBG_ASSERT_MSG(insertResult.second, "Unable to create and insert");

		return IndexValuePair_t{newIndex, insertResult.first->second};
	}

	ValueType& GetOrCreate(const IndexType& index, const ValueType& v = ValueType())
	{
		START_PROFILE;
		if (HasIndex(index))
		{
			return Get(index);
		}
		m_container[index] = v;

		return m_container[index];
	}

	void Delete(const IndexType& index) { m_container.erase(index); }

	bool HasIndex(const IndexType& index) { return m_container.find(index) != std::end(m_container); }

	std::size_t Count() const { return m_container.size(); }

	iterator begin() { return m_container.begin(); }
	iterator end() { return m_container.end(); }
	const_iterator begin() const { return m_container.begin(); }
	const_iterator end() const { return m_container.end(); }

private:
	IndexType CreateIndex() { return m_nextIndex++; }

	IndexType m_nextIndex;
	ContainerType m_container;
};  // class AssociativeIndexedContainer
}  // namespace pagoda::geometry::core

#endif
