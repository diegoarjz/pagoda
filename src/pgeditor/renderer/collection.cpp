#include "collection.h"

namespace pgeditor::renderer
{
Collection::Collection() {}

Collection::Collection(const Container_t& c) : m_renderables{c} {}

Collection::Collection(Container_t&& c) : m_renderables{std::move(c)} {}

Collection::Collection(ConstIter_t& begin, ConstIter_t& end) : m_renderables{begin, end} {}

Collection::~Collection() {}

void Collection::Add(const RenderablePtr& r) { m_renderables.push_back(r); }
Collection::ConstIter_t Collection::begin() const { return m_renderables.cbegin(); }
Collection::ConstIter_t Collection::end() const { return m_renderables.cend(); }
}  // namespace pgeditor::renderer
