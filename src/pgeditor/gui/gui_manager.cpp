#include "gui_manager.h"

#include <pgeditor/event/event_manager.h>

#include <pgeditor/input/event/character_event.h>
#include <pgeditor/input/event/key_event.h>
#include <pgeditor/input/event/mouse_event.h>
#include <pgeditor/input/event/mouse_move_event.h>
#include <pgeditor/input/event/mouse_scroll_event.h>

#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

#include <iostream>

using namespace Magnum;

using namespace pgeditor::event;
using namespace pgeditor::common;
using namespace pgeditor::input::event;

namespace pgeditor::gui
{
class GuiManager::GuiManagerImpl
{
	public:
	GuiManagerImpl(EventManager* eventManager) : m_eventManager{eventManager} {}

	bool Init()
	{
		m_imgui = std::make_unique<ImGuiIntegration::Context>(Vector2i{800, 600});

		GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add, GL::Renderer::BlendEquation::Add);
		GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
		                               GL::Renderer::BlendFunction::OneMinusSourceAlpha);

		m_eventManager->Register<MouseEvent, &GuiManagerImpl::OnMouseEvent>(*this);
		m_eventManager->Register<MouseMoveEvent, &GuiManagerImpl::OnMouseMoveEvent>(*this);
		m_eventManager->Register<MouseScrollEvent, &GuiManagerImpl::OnMouseScrollEvent>(*this);
		m_eventManager->Register<CharacterEvent, &GuiManagerImpl::OnCharacterEvent>(*this);
		m_eventManager->Register<KeyEvent, &GuiManagerImpl::OnKeyEvent>(*this);
		return true;
	}

	void OnMouseEvent(const MouseEvent& e) { m_mouseEvents.push_back(e); }
	void OnMouseMoveEvent(const MouseMoveEvent& e) { m_mouseMoveEvents.push_back(e); }
	void OnMouseScrollEvent(const MouseScrollEvent& e) { m_mouseScrollEvents.push_back(e); }
	void OnCharacterEvent(const CharacterEvent& e) { m_characterEvents.push_back(e); }
	void OnKeyEvent(const KeyEvent& e) { m_keyEvents.push_back(e); }

	void Update(double dT)
	{
		GL::Renderer::enable(GL::Renderer::Feature::Blending);
		GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
		GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);

		processEvents();
		m_imgui->newFrame();

		m_imgui->drawFrame();
		GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
		GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
		GL::Renderer::disable(GL::Renderer::Feature::Blending);
	}

	void Destroy() { m_imgui = nullptr; }

	private:
	void processEvents()
	{
		ImGui::SetCurrentContext(m_imgui->context());

		for (const auto& e : m_mouseEvents) {
			if (e.pressed()) {
				m_imgui->handleMousePressEvent(e);
			} else {
				m_imgui->handleMouseReleaseEvent(e);
			}
		}
		for (const auto& e : m_mouseMoveEvents) {
			m_imgui->handleMouseMoveEvent(e);
		}
		for (const auto& e : m_mouseScrollEvents) {
			m_imgui->handleMouseScrollEvent(e);
		}
		for (const auto& e : m_characterEvents) {
			ImGui::GetIO().AddInputCharacter(e.GetCharacter());
		}
		for (const auto& e : m_keyEvents) {
			auto value = (e.GetType() != KeyEvent::KeyEventType::Released);
			switch (e.GetKey()) {
				case KeyEvent::Key::LeftShift:
				case KeyEvent::Key::RightShift:
					ImGui::GetIO().KeyShift = value;
					break;
				case KeyEvent::Key::LeftControl:
				case KeyEvent::Key::RightControl:
					ImGui::GetIO().KeyCtrl = value;
					break;
				case KeyEvent::Key::LeftAlt:
				case KeyEvent::Key::RightAlt:
					ImGui::GetIO().KeyAlt = value;
					break;
				case KeyEvent::Key::LeftSuper:
				case KeyEvent::Key::RightSuper:
					ImGui::GetIO().KeySuper = value;
					break;
				case KeyEvent::Key::Tab:
					ImGui::GetIO().KeysDown[ImGuiKey_Tab] = value;
					break;
				case KeyEvent::Key::Up:
					ImGui::GetIO().KeysDown[ImGuiKey_UpArrow] = value;
					break;
				case KeyEvent::Key::Down:
					ImGui::GetIO().KeysDown[ImGuiKey_DownArrow] = value;
					break;
				case KeyEvent::Key::Left:
					ImGui::GetIO().KeysDown[ImGuiKey_LeftArrow] = value;
					break;
				case KeyEvent::Key::Right:
					ImGui::GetIO().KeysDown[ImGuiKey_RightArrow] = value;
					break;
				case KeyEvent::Key::Home:
					ImGui::GetIO().KeysDown[ImGuiKey_Home] = value;
					break;
				case KeyEvent::Key::End:
					ImGui::GetIO().KeysDown[ImGuiKey_End] = value;
					break;
				case KeyEvent::Key::PageUp:
					ImGui::GetIO().KeysDown[ImGuiKey_PageUp] = value;
					break;
				case KeyEvent::Key::PageDown:
					ImGui::GetIO().KeysDown[ImGuiKey_PageDown] = value;
					break;
				case KeyEvent::Key::Enter:
				case KeyEvent::Key::KeyPadEnter:
					ImGui::GetIO().KeysDown[ImGuiKey_Enter] = value;
					break;
				case KeyEvent::Key::Escape:
					ImGui::GetIO().KeysDown[ImGuiKey_Escape] = value;
					break;
				case KeyEvent::Key::Space:
					ImGui::GetIO().KeysDown[ImGuiKey_Space] = value;
					break;
				case KeyEvent::Key::Backspace:
					ImGui::GetIO().KeysDown[ImGuiKey_Backspace] = value;
					break;
				case KeyEvent::Key::Delete:
					ImGui::GetIO().KeysDown[ImGuiKey_Delete] = value;
					break;
				case KeyEvent::Key::A:
					ImGui::GetIO().KeysDown[ImGuiKey_A] = value;
					break;
				case KeyEvent::Key::C:
					ImGui::GetIO().KeysDown[ImGuiKey_C] = value;
					break;
				case KeyEvent::Key::V:
					ImGui::GetIO().KeysDown[ImGuiKey_V] = value;
					break;
				case KeyEvent::Key::X:
					ImGui::GetIO().KeysDown[ImGuiKey_X] = value;
					break;
				case KeyEvent::Key::Y:
					ImGui::GetIO().KeysDown[ImGuiKey_Y] = value;
					break;
				case KeyEvent::Key::Z:
					ImGui::GetIO().KeysDown[ImGuiKey_Z] = value;
					break;
				default:
					// Unknown key, do nothing
					continue;
			}
		}

		m_mouseEvents.clear();
		m_mouseMoveEvents.clear();
		m_mouseScrollEvents.clear();
		m_characterEvents.clear();
		m_keyEvents.clear();
	}

	std::unique_ptr<ImGuiIntegration::Context> m_imgui;
	EventManager* m_eventManager;
	std::vector<MouseEvent> m_mouseEvents;
	std::vector<MouseMoveEvent> m_mouseMoveEvents;
	std::vector<MouseScrollEvent> m_mouseScrollEvents;
	std::vector<CharacterEvent> m_characterEvents;
	std::vector<KeyEvent> m_keyEvents;
};

GuiManager::GuiManager(EventManager* eventManager) : m_implementation{std::make_unique<GuiManagerImpl>(eventManager)} {}
GuiManager::~GuiManager() {}

bool GuiManager::Init() { return m_implementation->Init(); }
void GuiManager::Update(double dT) { m_implementation->Update(dT); }
void GuiManager::Destroy() { m_implementation->Destroy(); }
}  // namespace pgeditor::gui
