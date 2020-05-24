#include "mesh.h"

#include "attachable_visitor.h"

using namespace pgeditor::rendering;

namespace pgeditor::scene
{
Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<ShaderProgram> shaderProgram)
    : m_geometry(geometry), m_shaderProgram(shaderProgram)
{
}

Mesh::~Mesh() {}

std::shared_ptr<Geometry> Mesh::GetGeometry() const { return m_geometry; }
std::shared_ptr<ShaderProgram> Mesh::GetShaderProgram() const { return m_shaderProgram; }

void Mesh::AcceptVisitor(IAttachableVisitor *visitor) { visitor->Visit(this); }
}  // namespace pgeditor::scene
