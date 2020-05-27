#pragma once

#include <unordered_map>
#include <unordered_set>
#include "renderable.h"

namespace pgeditor::rendering
{
class ShaderProgram;
class Texture;
class Uniform;

class Material : public Renderable
{
public:
	Material(std::shared_ptr<ShaderProgram> program);
	virtual ~Material();

	void SetTexture(uint8_t slot, std::shared_ptr<Texture> textue);
	const std::vector<std::shared_ptr<Uniform>> &GetUniforms() const;

protected:
	void DoLoad(Renderer *r) override;
	void DoRender(Renderer *r) override;
	void DoDispose(Renderer *r) override;

private:
	std::shared_ptr<ShaderProgram> m_shaderProgram;
	std::unordered_map<uint8_t, std::shared_ptr<Texture>> m_textures;
};
}  // namespace pgeditor::rendering
