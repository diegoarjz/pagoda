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

	IndexedContainer(const ValueType& invalidValue) : m_nextIndex(0), m_count(0), m_invalidValue(invalidValue) {}

	ValueType& Get(const IndexType& index)
	{
		START_PROFILE;
		return m_container.at(index);
	}

	const ValueType& Get(const IndexType& index) const
	{
		START_PROFILE;
		return m_container.at(index);
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
		m_container[index] = m_invalidValue;
	}

	bool HasIndex(const IndexType& index) { return m_container.size() > index && m_container[index] != m_invalidValue; }

	std::size_t Count() const { return m_count; }

	class iterator
	{
	public:
		iterator(IndexedContainer_t& container, const std::size_t& index)
		    : m_container(container), m_currentIndex(FindNextValid(index))
		{
		}

		IndexValuePair operator*() { return IndexValuePair{m_currentIndex, m_container.m_container.at(m_currentIndex)}; }

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
				if (m_container.m_container[i] != m_container.m_invalidValue)
				{
					return i;
				}
			}
			return m_container.m_container.size();
		}

        IndexedContainer_t &m_container;
		std::size_t m_currentIndex;
	};

	iterator begin() { return iterator(*this, 0); }
	iterator end() { return iterator(*this, m_container.size()); }

private:
	IndexType CreateAtIndex(const IndexType& index, const ValueType& v)
	{
		if (m_container.size() <= index)
		{
			m_container.resize(index * 2 + 1, ValueType(m_invalidValue));
		}
		++m_count;
		m_container[index] = v;
		return index;
	}

	IndexType CreateIndex()
	{
		while (m_nextIndex < m_container.size() && m_container[m_nextIndex] != m_invalidValue)
		{
			++m_nextIndex;
		}
		return m_nextIndex;
	}
	IndexType m_nextIndex;

	ContainerType m_container;
	std::size_t m_count;
	ValueType m_invalidValue;
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
}  // namespace selector

#endif
