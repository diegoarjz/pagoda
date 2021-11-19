#pragma once

#include <string>
#include <vector>

namespace pagoda::common
{
/// Suppresses warnings about unused values and guarantees that the code is not
/// optimised out
#define UNUSED(x)      \
	do {                 \
		(void)sizeof((x)); \
	} while (0)

bool is_float(const std::string& str);

/**
 * Computes the cartesian product of \p items.
 * The callback function \p f is called with each result of the cartesian
 * product. If any vector in \p items is empty then, by definition, the
 * cartesian product is empty. \return The number of results.
 */
template<class T, typename Cb>
std::size_t CartesianProduct(const std::vector<std::vector<T>>& items, Cb f)
{
	using Iterator_t = typename std::vector<T>::const_iterator;
	// struct to keep track of the iteration in each item of items
	struct CartProd
	{
		CartProd(const std::vector<T>& v)
		  : m_begin{v.begin()}, m_end{v.end()}, m_current{v.begin()}
		{
		}
		Iterator_t m_begin;
		Iterator_t m_end;
		Iterator_t m_current;
	};

	// Add a CartProd for each vector in item.
	std::vector<CartProd> iters;
	iters.reserve(items.size());
	for (const auto& itemVec : items) {
		if (itemVec.empty()) {
			// Found an empty vector, so bail out here
			return 0;
		}
		iters.emplace_back(itemVec);
	}

	// Keeps track of how many products we have computed
	std::size_t count = 0;

	// Holds each product. Because we know the size of a product before hand, we
	// can avoid successive allocations by reusing the same vector.
	std::vector<T> p;
	p.resize(iters.size());

	// Perform the cartesian product
	while (true) {
		++count;

		// Construct the first product vector by extracting the elements of each
		// CartProd.
		for (std::size_t i = 0; i < iters.size(); ++i) {
			const auto& v = iters[i];
			p[i] = *v.m_current;
		}
		// Pass the product to the callback.
		f(p);

		// Do the iteration from left to right.
		for (auto it = iters.begin(); /**/; /**/) {
			++(it->m_current);
			if (it->m_current == it->m_end) {
				if (it + 1 == iters.end()) {
					return count;
				} else {
					it->m_current = it->m_begin;
					++it;
				}
			} else {
				break;
			}
		}
	}
	return count;
}

}  // namespace pagoda::common
