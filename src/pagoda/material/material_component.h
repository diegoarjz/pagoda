#pragma once

#include "material.h"

#include <pagoda/objects/procedural_component.h>

namespace pagoda
{
namespace material
{
class MaterialComponent : public objects::ProceduralComponent
{
	public:
	static std::string GetComponentSystemName();

	virtual ~MaterialComponent();

	std::string GetType() const override;

	void SetMaterial(const Material& m);
	const Material& GetMaterial() const;
	Material& GetMaterial();

	private:
	Material m_material;
};
}  // namespace material
}  // namespace pagoda
