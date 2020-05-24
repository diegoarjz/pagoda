#pragma once

namespace pgeditor::scene
{
class IAttachableVisitor;

class IAttachable
{
public:
	virtual void AcceptVisitor(IAttachableVisitor *visitor) = 0;
};
}  // namespace pgeditor::scene
