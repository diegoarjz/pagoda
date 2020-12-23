#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <pgeditor/window/window.h>
#include <pgeditor/window/window_creation_params.h>

#include <pgeditor/input/input_manager.h>
#include <pgeditor/input/keys.h>

using namespace boost;

using namespace pagoda;
using namespace pagoda::common;
using namespace pagoda::common::debug;
using namespace pagoda::common::instrument;

using namespace pgeditor;
using namespace pgeditor::input;
using namespace pgeditor::window;

int main(int argc, char* argv[])
{
	WindowCreationParams wcp;
	InputManager inputManager;

	auto w = std::make_shared<Window>();
	w->Create(wcp);

	double previousTime = w->GetEllapsedTime();
	double currentTime;
	double ellapsedTime;
	while (!w->GetWindowShouldClose()) {
		currentTime = w->GetEllapsedTime();
		ellapsedTime = currentTime - previousTime;
		previousTime = currentTime;

		inputManager.Update(ellapsedTime);

		w->PollEvents();
		w->SwapBuffers();
	}

	w->Destroy();

	return 0;
}
