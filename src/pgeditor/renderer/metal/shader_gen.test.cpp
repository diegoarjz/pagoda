#include "shader_gen.h"

#include <pagoda/material/material_network.h>
#include <pagoda/material/material_node.h>

#include <pagoda/math/vec_base.h>

#include <gtest/gtest.h>

#include <vector>

using namespace pgeditor::renderer;
using namespace pagoda::math;
using namespace pagoda::common;
using namespace pagoda::material;

TEST(ShaderGen, test_simple_node)
{
	MaterialNetwork network("default");
	// The Vert shader network
	auto defaultVert = network.CreateMaterialNode("defaultVert", "defaultVert");
	defaultVert->SetInput("position", {"position", Type::Vec4});
	defaultVert->SetInput("viewport", {"viewport", Type::Vec2});
	defaultVert->SetOutput("position", {"position", Type::Vec4});
	network.SetStageTerminalNode(MaterialNetwork::ShaderStage::Vertex, "defaultVert");

	auto positionNode = network.CreateMaterialNode("bufferView", "position");
	positionNode->SetOutput("position", {"position", Type::Vec4});
	positionNode->SetParameter("bufferName", "position");
	positionNode->SetParameter("semantics", static_cast<int>(VertexAttributeSemantics::Position));
	positionNode->SetParameter("type", static_cast<int>(Type::Vec4));

	auto viewPortUniformNode = network.CreateMaterialNode("uniformView", "viewport");
	viewPortUniformNode->SetOutput("viewport", {"viewport", Type::Vec2});
	viewPortUniformNode->SetParameter("uniformName", "viewport");
	viewPortUniformNode->SetParameter("type", static_cast<int>(Type::Vec2));

	defaultVert->ConnectInput("position", positionNode, "positionn");
	defaultVert->ConnectInput("viewport", viewPortUniformNode, "viewport");

	// The frag shader network
	auto defaultFrag = network.CreateMaterialNode("defaultFrag", "defaultFrag");
	defaultFrag->SetInput("color", {"color", Type::Vec4});
	defaultFrag->SetOutput("color", {"color", Type::Vec4});

	auto colorNode = network.CreateMaterialNode("bufferView", "color");
	colorNode->SetOutput("color", {"color", Type::Vec4});
	colorNode->SetParameter("bufferName", "color");
	colorNode->SetParameter("semantics", static_cast<int>(VertexAttributeSemantics::Color));
	colorNode->SetParameter("type", static_cast<int>(Type::Vec4));

	defaultFrag->ConnectInput("color", colorNode, "color");

	network.SetStageTerminalNode(MaterialNetwork::ShaderStage::Fragment, "defaultFrag");

	auto source = metal::ShaderGen{network}.Generate();
	std::cout << "shader source" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << source << std::endl;
}
