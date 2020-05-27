#pragma once

#include "attachable.h"
#include "pgeditor/rendering/renderable_object.h"

#include <pagoda/image/image.h>

#include <memory>

namespace pgeditor::rendering
{
class Geometry;
class ShaderProgram;
class Material;
}  // namespace pgeditor::rendering

namespace pgeditor::scene
{
class Mesh final : public IAttachable
{
public:
	Mesh(std::shared_ptr<rendering::Geometry> geometry, std::shared_ptr<rendering::Material> material);
	virtual ~Mesh();

	std::shared_ptr<rendering::Geometry> GetGeometry() const;
	std::shared_ptr<rendering::Material> GetMaterial() const;

	void SetTexture(pagoda::image::ImagePtr texture);
	pagoda::image::ImagePtr GetTexture() const;

	void AcceptVisitor(IAttachableVisitor *visitor) override;

	std::shared_ptr<rendering::RenderableObject> GetRenderableObject() const;
	void SetRenderableObject(std::shared_ptr<rendering::RenderableObject> o);

private:
	std::shared_ptr<rendering::Geometry> m_geometry;
	std::shared_ptr<rendering::Material> m_material;
	pagoda::image::ImagePtr m_texture;

	std::shared_ptr<rendering::RenderableObject> m_renderableObject;
};
}  // namespace pgeditor::scene
