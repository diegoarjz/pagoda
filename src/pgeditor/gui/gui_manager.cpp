#include "gui_manager.h"

#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

using namespace Magnum;

namespace pgeditor::gui
{
class GuiManager::GuiManagerImpl
{
	public:
	bool Init()
	{
		m_imgui = std::make_unique<ImGuiIntegration::Context>(Vector2i{800, 600});

		GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add, GL::Renderer::BlendEquation::Add);
		GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
		                               GL::Renderer::BlendFunction::OneMinusSourceAlpha);

		return true;
	}

	void Update(double dT)
	{
		GL::Renderer::enable(GL::Renderer::Feature::Blending);
		GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
		GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);

		m_imgui->newFrame();

		// draw the GUI here

		m_imgui->drawFrame();
		GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
		GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
		GL::Renderer::disable(GL::Renderer::Feature::Blending);
	}

	void Destroy() { m_imgui = nullptr; }

	private:
	std::unique_ptr<ImGuiIntegration::Context> m_imgui;
};

GuiManager::GuiManager() : m_implementation{std::make_unique<GuiManagerImpl>()} {}
GuiManager::~GuiManager() {}

bool GuiManager::Init() { return m_implementation->Init(); }
void GuiManager::Update(double dT) { m_implementation->Update(dT); }
void GuiManager::Destroy() { m_implementation->Destroy(); }
}  // namespace pgeditor::gui
