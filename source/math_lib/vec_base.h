#ifndef SELECTOR_MATH_LIB_VEC_BASE_H
#define SELECTOR_MATH_LIB_VEC_BASE_H

#include <common/assertions.h>
#include <common/profiler.h>

#include <cstring>

namespace selector
{
/**
 * Base vector class.
 */
template<int Size, class Rep>
class VecBase
{
public:
	using VecType = VecBase<Size, Rep>;
	using Rep_t = Rep;
	static const size_t kSize = Size;
	static const size_t kSizeInBytes = Size * sizeof(Rep);

	enum class Element
	{
		X,
		Y,
		Z,
		W
	};

	explicit VecBase(const Rep& initialValue = Rep())
	{
		START_PROFILE;
		for (auto i = 0u; i < Size; ++i)
		{
			m_elems[i] = initialValue;
		}
	}

	VecBase(const Rep& v0, const Rep& v1)
	{
		START_PROFILE;
		static_assert(Size == 2, "Wrong VecBase constructor used");
		m_elems[0] = v0;
		m_elems[1] = v1;
	}

	VecBase(const Rep& v0, const Rep& v1, const Rep& v2)
	{
		START_PROFILE;
		static_assert(Size == 3, "Wrong VecBase constructor used");
		m_elems[0] = v0;
		m_elems[1] = v1;
		m_elems[2] = v2;
	}

	VecBase(const Rep& v0, const Rep& v1, const Rep& v2, const Rep& v3)
	{
		START_PROFILE;
		static_assert(Size == 4, "Wrong VecBase constructor used");
		m_elems[0] = v0;
		m_elems[1] = v1;
		m_elems[2] = v2;
		m_elems[3] = v3;
	}

	VecBase(const VecType& rhs)
	{
		START_PROFILE;
		std::memcpy(m_elems, rhs.m_elems, kSizeInBytes);
	}

	VecType& operator=(const VecType& rhs)
	{
		START_PROFILE;
		std::memcpy(m_elems, rhs.m_elems, kSizeInBytes);
		return *this;
	}

	bool operator==(const VecType& rhs) const
	{
		START_PROFILE;
		for (auto i = 0u; i < Size; ++i)
		{
			if (m_elems[i] != rhs.m_elems[i])
			{
				return false;
			}
		}
		return true;
	}

	bool operator!=(const VecType& rhs) const
	{
		START_PROFILE;
		return !(*this == rhs);
	}

	explicit operator const Rep*() const
	{
		START_PROFILE;
		return &m_elems[0];
	}

	explicit operator Rep*()
	{
		START_PROFILE;
		return &m_elems[0];
	}

	inline Rep& operator[](const unsigned int& idx)
	{
		CRITICAL_ASSERT(idx < Size);
		return m_elems[idx];
	}

	inline const Rep& operator[](const unsigned int& idx) const
	{
		CRITICAL_ASSERT(idx < Size);
		return m_elems[idx];
	}

	Rep& X() { return m_elems[0]; }
	const Rep& X() const { return m_elems[0]; }

	template<int U = Size>
	typename std::enable_if<(U > 1), Rep>::type& Y()
	{
		return m_elems[1];
	}

	template<int U = Size>
	const typename std::enable_if<(U > 1), Rep>::type& Y() const
	{
		return m_elems[1];
	}

	template<int U = Size>
	typename std::enable_if<(U > 2), Rep>::type& Z()
	{
		return m_elems[2];
	}

	template<int U = Size>
	const typename std::enable_if<(U > 2), Rep>::type& Z() const
	{
		return m_elems[2];
	}

	template<int U = Size>
	typename std::enable_if<(U > 3), Rep>::type& W()
	{
		return m_elems[3];
	}

	template<int U = Size>
	const typename std::enable_if<(U > 3), Rep>::type& W() const
	{
		return m_elems[3];
	}

private:
	Rep m_elems[Size];
};  // class VecBase

template<int Size, class Rep>
std::ostream& operator<<(std::ostream& o, const VecBase<Size, Rep>& vec)
{
	o << "[";
	for (auto i = 0u; i < Size; ++i)
	{
		o << vec[i] << ",";
	}
	o << "]";
	return o;
}

template<int Size, class Rep>
std::string to_string(const VecBase<Size, Rep>& vec)
{
	std::stringstream ss;
	ss << vec;
	return ss.str();
}

// Common vector types
using Vec2F = VecBase<2, float>;
using Vec3F = VecBase<3, float>;
using Vec4F = VecBase<4, float>;

using Vec2D = VecBase<2, double>;
using Vec3D = VecBase<3, double>;
using Vec4D = VecBase<4, double>;

}  // namespace selector

#include "vec_arithmetic.h"

#endif
