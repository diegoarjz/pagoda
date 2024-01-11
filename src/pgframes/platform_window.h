#pragma once

#include "main_loop.h"

#include "pagoda/image/image.h"

namespace pgframes {

using PlatformWindowPtr = std::shared_ptr<class PlatformWindow>;

class PlatformTexture {
public:
  struct Handle;

  PlatformTexture(PlatformWindow* window, pagoda::image::ImagePtr image) {}
  virtual ~PlatformTexture() = default;

  virtual const Handle &GetNativeHandle() const = 0;
};

using PlatformTexturePtr = std::shared_ptr<PlatformTexture>;

class PlatformWindow {
public:
  virtual ~PlatformWindow() = default;

  virtual bool Startup() = 0;
  virtual void RunMainLoop() = 0;
  virtual bool Teardown() = 0;

  void SetMainLoop(const MainLoopPtr &mainLoop) { m_mainLoop = mainLoop; }

  static PlatformWindowPtr Get(const std::string &name);

  virtual PlatformTexturePtr CreateTexture(pagoda::image::ImagePtr image) = 0;

protected:
  MainLoopPtr m_mainLoop{nullptr};
};
} // namespace alpha::frontend
