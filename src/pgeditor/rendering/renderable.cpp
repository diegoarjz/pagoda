#include "renderable.h"

namespace pgeditor::rendering
{
Renderable::Renderable() : m_boundState{BoundState::Unbound} {}

void Renderable::Load(Renderer* r)
{
	DoLoad(r);
	m_boundState = BoundState::Bound;
}

void Renderable::Render(Renderer* r)
{
	if (m_boundState == BoundState::Unbound)
	{
		Load(r);
	}
	DoRender(r);
}

void Renderable::Dispose(Renderer* r)
{
	DoDispose(r);
	m_boundState = BoundState::Unbound;
}
}  // namespace pgeditor::rendering
