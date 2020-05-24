#pragma once

#include "attachable.h"

#include <memory>

namespace pgeditor::rendering
{
class ShaderProgram;
}

namespace pgeditor::scene
{
class Geometry;

class Mesh final : public IAttachable
{
public:
	Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<rendering::ShaderProgram> shaderProgram);
	virtual ~Mesh();

	std::shared_ptr<Geometry> GetGeometry() const;
	std::shared_ptr<rendering::ShaderProgram> GetShaderProgram() const;

	void AcceptVisitor(IAttachableVisitor *visitor) override;

private:
	std::shared_ptr<Geometry> m_geometry;
	std::shared_ptr<rendering::ShaderProgram> m_shaderProgram;
};
}  // namespace pgeditor::scene
