#ifndef SELECTOR_COMMON_CONST_STR_H_
#define SELECTOR_COMMON_CONST_STR_H_

#include <stdint.h>
#include <cstddef>
#include <type_traits>

namespace selector
{
template<int N>
class const_str
{
public:
	constexpr const_str(const char (&str)[N + 1]) : string_const(str) {}
	constexpr char operator[](int i) const { return string_const[i]; }
	constexpr std::size_t size() const { return N; }
	constexpr operator const char* const() const { return string_const; }

private:
	const char (&string_const)[N + 1];
};

template<int N_PLUS_1>
constexpr auto literal(const char (&str)[N_PLUS_1]) -> const_str<N_PLUS_1 - 1>
{
	return const_str<N_PLUS_1 - 1>(str);
}

template<int N>
class array_string
{
public:
	template<int N1>
	constexpr array_string(const const_str<N1>& s1, const const_str<N - N1>& s2)
	{
		// TODO: The idea is to make this a compile time concatenation of strings...
		for (int i = 0; i < N1; ++i)
		{
			m_stringArray[i] = s1[i];
		}

		for (int i = 0; i < N - N1; ++i)
		{
			m_stringArray[N1 + i] = s2[i];
		}

		m_stringArray[N] = '\0';
	}

	constexpr operator const char* const() const { return m_stringArray; }
	constexpr operator const_str<N>() const { return const_str<N>(m_stringArray); }

private:
	char m_stringArray[N + 1];
};

template<int N1, int N2>
constexpr auto operator+(const_str<N1> s1, const_str<N2> s2) -> array_string<N1 + N2>
{
	return array_string<N1 + N2>(s1, s2);
}

template<int N1, int N2>
constexpr auto operator+(const_str<N1> s1, const char (&s2)[N2]) -> array_string<N1 + N2 - 1>
{
	return array_string<N1 + N2 - 1>(s1, const_str<N2 - 1>(s2));
}

template<int N1, int N2>
constexpr auto operator+(const char (&s1)[N1], const_str<N2> s2) -> array_string<N1 + N2 - 1>
{
	return array_string<N1 + N2 - 1>(const_str<N1 - 1>(s1), s2);
}

template<int N1, int N2>
constexpr auto operator+(array_string<N1> s1, const_str<N2> s2) -> array_string<N1 + N2>
{
	return array_string<N1 + N2>(static_cast<const_str<N1>>(s1), s2);
}

}  // namespace selector

#endif
