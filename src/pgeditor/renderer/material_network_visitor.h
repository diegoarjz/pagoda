#pragma once

namespace pgeditor::renderer
{
class MaterialNetwork;
class MaterialNode;

class MaterialNetworkVisitor
{
	public:
	virtual void Visit(const MaterialNode* node) = 0;
};
}  // namespace pgeditor::renderer
