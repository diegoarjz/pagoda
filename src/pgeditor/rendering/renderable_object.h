#pragma once

#include "renderable.h"

#include <pagoda/math/matrix_base.h>

#include <memory>

namespace pagoda::image
{
class Image;
}

namespace pgeditor::rendering
{
class Geometry;
class Material;

class RenderableObject : public Renderable
{
public:
	RenderableObject(std::shared_ptr<rendering::Geometry> geometry, std::shared_ptr<Material> material);
	virtual ~RenderableObject();

	std::shared_ptr<rendering::Geometry> GetGeometry() const;
	std::shared_ptr<Material> GetMaterial() const;

	void SetModelMatrix(const boost::qvm::mat<float, 4, 4>& modelMatrix);
	const boost::qvm::mat<float, 4, 4>& GetModelMatrix() const;

	void SetViewMatrix(const boost::qvm::mat<float, 4, 4>& viewMatrix);
	const boost::qvm::mat<float, 4, 4>& GetViewMatrix() const;

	void SetProjectionMatrix(const boost::qvm::mat<float, 4, 4>& projectionMatrix);
	const boost::qvm::mat<float, 4, 4>& GetProjectionMatrix() const;

protected:
	void DoLoad(Renderer* r) override;
	void DoRender(Renderer* r) override;
	void DoDispose(Renderer* r) override;

private:
	std::shared_ptr<rendering::Geometry> m_geometry;
	std::shared_ptr<Material> m_material;

	boost::qvm::mat<float, 4, 4> m_modelMatrix;
	boost::qvm::mat<float, 4, 4> m_viewMatrix;
	boost::qvm::mat<float, 4, 4> m_projectionMatrix;
};
}  // namespace pgeditor::rendering
