#include "mesh.h"

#include "attachable_visitor.h"

using namespace pgeditor::rendering;

namespace pgeditor::scene
{
Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material)
    : m_geometry(geometry), m_material(material)
{
}

Mesh::~Mesh() {}

std::shared_ptr<Geometry> Mesh::GetGeometry() const { return m_geometry; }
std::shared_ptr<Material> Mesh::GetMaterial() const { return m_material; }

void Mesh::SetTexture(pagoda::image::ImagePtr texture) { m_texture = texture; }
pagoda::image::ImagePtr Mesh::GetTexture() const { return m_texture; }

std::shared_ptr<RenderableObject> Mesh::GetRenderableObject() const { return m_renderableObject; }
void Mesh::SetRenderableObject(std::shared_ptr<RenderableObject> o) { m_renderableObject = o; }

void Mesh::AcceptVisitor(IAttachableVisitor *visitor) { visitor->Visit(this); }
}  // namespace pgeditor::scene
