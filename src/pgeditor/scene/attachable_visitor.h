#pragma once

namespace pgeditor::scene
{
class Camera;
class Mesh;

class IAttachableVisitor
{
public:
	virtual void Visit(Camera *camera) = 0;
	virtual void Visit(Mesh *mesh) = 0;
};
}  // namespace pgeditor::scene
