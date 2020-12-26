#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <pgeditor/window/window.h>
#include <pgeditor/window/window_creation_params.h>

#include <pgeditor/input/input_manager.h>
#include <pgeditor/input/keys.h>

#include <pgeditor/gui/gui_manager.h>

#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

using namespace boost;

using namespace pagoda;
using namespace pagoda::common;
using namespace pagoda::common::debug;
using namespace pagoda::common::instrument;

using namespace pgeditor;
using namespace pgeditor::input;
using namespace pgeditor::window;
using namespace pgeditor::gui;

int main(int argc, char* argv[])
{
	using namespace Magnum;

	WindowCreationParams wcp;
	InputManager inputManager;
	GuiManager guiManager;

	auto w = std::make_shared<Window>();
	w->Create(wcp);

	inputManager.Init();
	guiManager.Init();

	double previousTime = w->GetEllapsedTime();
	double currentTime;
	double ellapsedTime;
	while (!w->GetWindowShouldClose()) {
		GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

		currentTime = w->GetEllapsedTime();
		ellapsedTime = currentTime - previousTime;
		previousTime = currentTime;

		inputManager.Update(ellapsedTime);
		guiManager.Update(ellapsedTime);

		w->PollEvents();
		w->SwapBuffers();
	}

	guiManager.Destroy();
	inputManager.Destroy();

	w->Destroy();

	return 0;
}
