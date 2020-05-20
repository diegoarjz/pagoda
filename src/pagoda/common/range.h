#ifndef PAGODA_COMMON_RANGE_H_
#define PAGODA_COMMON_RANGE_H_

namespace pagoda::common
{
/**
 * \brief Implements a Range between two values with increments.
 *
 * Provides an iterator \c Range::iterator to iterate between the values.
 */
template<typename T>
class Range
{
public:
	class iterator
	{
	public:
		iterator(const Range<T>& r, const T& start_value) : current_value(start_value), range(r) {}

		bool operator==(const iterator& rhs) const { return current_value == rhs.current_value; }

		bool operator!=(const iterator& rhs) const { return current_value != rhs.current_value; }

		iterator operator++(int)
		{
			current_value += range.m_rangeIncrement;
			return *this;
		}

		iterator& operator++()
		{
			current_value += range.m_rangeIncrement;
			return *this;
		}

		const T& operator*() { return current_value; }

	private:
		T current_value;
		Range<T> range;
	};  // class iterator

	explicit Range(const T& end) : Range(T(), end, 1) {}
	Range(const T& start, const T& end) : Range(start, end, 1) {}
	Range(const T& start, const T& end, const T& increment)
	    : m_rageStart(start), m_rangeEnd(end), m_rangeIncrement(increment)
	{
	}

	iterator begin() { return iterator(*this, m_rageStart); }

	iterator end() { return iterator(*this, m_rangeEnd); }

private:
	T m_rageStart;
	T m_rangeEnd;
	T m_rangeIncrement;
};  // class Range
}  // namespace pagoda::common
#endif
