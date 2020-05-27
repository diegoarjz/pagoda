#pragma once

#include <memory>
#include "bound_state.h"

namespace pgeditor::rendering
{
class Renderer;

class Renderable : public std::enable_shared_from_this<Renderable>
{
public:
	Renderable();

	void Load(Renderer* r);
	void Render(Renderer* r);
	void Dispose(Renderer* r);

protected:
	virtual void DoLoad(Renderer* r) = 0;
	virtual void DoRender(Renderer* r) = 0;
	virtual void DoDispose(Renderer* r) = 0;

public:
	BoundState m_boundState;
};
}  // namespace pgeditor::rendering
