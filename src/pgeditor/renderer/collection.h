#pragma once

#include <memory>
#include <vector>

namespace pgeditor::renderer
{
using RenderablePtr = std::shared_ptr<class Renderable>;

class Collection
{
	public:
	using Container_t = std::vector<RenderablePtr>;
	using Iter_t = Container_t::iterator;
	using ConstIter_t = Container_t::const_iterator;

	Collection();
	Collection(const Container_t& c);
	Collection(Container_t&& c);
	Collection(ConstIter_t& begin, ConstIter_t& end);

	~Collection();

	void Add(const RenderablePtr& r);

	ConstIter_t begin() const;
	ConstIter_t end() const;

	private:
	Container_t m_renderables;
};
}  // namespace pgeditor::renderer
