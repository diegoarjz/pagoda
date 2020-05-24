#pragma once

#include <memory>

namespace pgeditor::scene
{
class Camera;
class Node;
}  // namespace pgeditor::scene

namespace pgeditor::rendering
{
class ShaderProgram;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void SetCamera(std::shared_ptr<scene::Camera> camera);

	void RenderScene(std::shared_ptr<scene::Node> sceneRootNode);

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace pgeditor::rendering
