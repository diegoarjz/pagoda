#pragma once

#include "material_network.h"

#include <pagoda/objects/procedural_component.h>

namespace pagoda
{
namespace material
{
using MaterialNetworkPtr = std::shared_ptr<class MaterialNetwork>;

class MaterialComponent : public objects::ProceduralComponent
{
	public:
	static std::string GetComponentSystemName();

	virtual ~MaterialComponent();

	std::string GetType() const override;

	void SetMaterial(const MaterialNetworkPtr& m);
	const MaterialNetworkPtr& GetMaterial() const;
	MaterialNetworkPtr& GetMaterial();

	private:
	MaterialNetworkPtr m_material;
};
}  // namespace material
}  // namespace pagoda
