#pragma once

#include <pgeditor/renderer/vertex_attribute.h>

#include <memory>
#include <string>
#include <vector>

namespace pgeditor::renderer::metal
{
struct RenderPipelineStateDescriptor
{
	std::string m_debugName;
	std::string m_materialName;  // TODO: Should be a material descriptor?
	std::vector<VertexAttributeDescription> m_vertexDescription;

	// TODO:
	//  - Color, Depth, Stencil Attachments
};

class RenderPipelineSate
{
	public:
	const std::string& GetDebugName() const { return m_debugName; }

	private:
	std::string m_debugName;
};
using RenderPipelineStatePtr = std::shared_ptr<RenderPipelineSate>;

}  // namespace pgeditor::renderer::metal
