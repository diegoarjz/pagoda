#include "metal.h"

#include "pagoda/api.h"
#include "pagoda/common/pluggable_factory.h"

#include "imgui_internal.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"


#include <iostream>

namespace pgframes {
namespace {
static void glfwErrorCallback(int error, const char *description) {
  LOG_ERROR("GLFW Error " << error << " " << description);
}
}

class MetalTexture : public PlatformTexture {
public:
  MetalTexture(PlatformWindow *window, pagoda::image::ImagePtr image)
      : PlatformTexture(window, image) {

    const auto textureSize = image->GetDimensions();

    auto metalWindow = dynamic_cast<MetalGlfwPlatform *>(window);

    MTLTextureDescriptor *textureDescriptor =
        [[MTLTextureDescriptor alloc] init];
    textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
    textureDescriptor.width = textureSize.x;
    textureDescriptor.height = textureSize.y;

    m_metalTexture =
        [metalWindow->m_device newTextureWithDescriptor:textureDescriptor];

    MTLRegion region = {{0, 0, 0},
                        {static_cast<NSUInteger>(textureSize.x),
                         static_cast<NSUInteger>(textureSize.y), 1}};

    NSUInteger bytesPerRow = 4 * textureSize.x;

    std::vector<uint8_t> imageData;
    image->CopyImageData(imageData);
    uint8_t *src = imageData.data();

    std::vector<char> buffer;
    buffer.resize(textureSize.y * bytesPerRow);
    char *dst = buffer.data();
    for (int r = 0; r < textureSize.x; ++r) {
      for (int c = 0; c < textureSize.y; ++c) {
        const auto r = *(src++);
        const auto g = *(src++);
        const auto b = *(src++);
        *(dst++) = b;
        *(dst++) = g;
        *(dst++) = r;
        *(dst++) = 0xFF;
      }
    }

    [m_metalTexture replaceRegion:region
                      mipmapLevel:0
                        withBytes:buffer.data()
                      bytesPerRow:bytesPerRow];
  }
  ~MetalTexture() override = default;

  const Handle &GetNativeHandle() const override {
    return reinterpret_cast<const Handle &>(m_metalTexture);
  }

private:
  id<MTLTexture> m_metalTexture;
};

bool MetalGlfwPlatform::Startup() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Set Gruvbox colors
  {
    auto &style = ImGui::GetStyle();
    style.ChildRounding = 0;
    style.GrabRounding = 0;
    style.FrameRounding = 0;
    style.PopupRounding = 0;
    style.ScrollbarRounding = 0;
    style.TabRounding = 0;
    style.WindowRounding = 0;
    style.FramePadding = {4, 4};

    style.WindowTitleAlign = {0.5, 0.5};

    ImVec4 *colors = ImGui::GetStyle().Colors;
    // Updated to use IM_COL32 for more precise colors and to add table colors
    // (1.80 feature)
    colors[ImGuiCol_Text] = ImColor{IM_COL32(0xeb, 0xdb, 0xb2, 0xFF)};
    colors[ImGuiCol_TextDisabled] = ImColor{IM_COL32(0x92, 0x83, 0x74, 0xFF)};
    colors[ImGuiCol_WindowBg] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xF0)};
    colors[ImGuiCol_ChildBg] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xFF)};
    colors[ImGuiCol_PopupBg] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xF0)};
    colors[ImGuiCol_Border] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xFF)};
    colors[ImGuiCol_BorderShadow] = ImColor{0};
    colors[ImGuiCol_FrameBg] = ImColor{IM_COL32(0x3c, 0x38, 0x36, 0x90)};
    colors[ImGuiCol_FrameBgHovered] = ImColor{IM_COL32(0x50, 0x49, 0x45, 0xFF)};
    colors[ImGuiCol_FrameBgActive] = ImColor{IM_COL32(0x66, 0x5c, 0x54, 0xA8)};
    colors[ImGuiCol_TitleBg] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0xFF)};
    colors[ImGuiCol_TitleBgActive] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
    colors[ImGuiCol_TitleBgCollapsed] =
        ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0x9C)};
    colors[ImGuiCol_MenuBarBg] = ImColor{IM_COL32(0x28, 0x28, 0x28, 0xF0)};
    colors[ImGuiCol_ScrollbarBg] = ImColor{IM_COL32(0x00, 0x00, 0x00, 0x28)};
    colors[ImGuiCol_ScrollbarGrab] = ImColor{IM_COL32(0x3c, 0x38, 0x36, 0xFF)};
    colors[ImGuiCol_ScrollbarGrabHovered] =
        ImColor{IM_COL32(0x50, 0x49, 0x45, 0xFF)};
    colors[ImGuiCol_ScrollbarGrabActive] =
        ImColor{IM_COL32(0x66, 0x5c, 0x54, 0xFF)};
    colors[ImGuiCol_CheckMark] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0x9E)};
    colors[ImGuiCol_SliderGrab] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0x70)};
    colors[ImGuiCol_SliderGrabActive] =
        ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
    colors[ImGuiCol_Button] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0x66)};
    colors[ImGuiCol_ButtonHovered] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0x9E)};
    colors[ImGuiCol_ButtonActive] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
    colors[ImGuiCol_Header] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0.4F)};
    colors[ImGuiCol_HeaderHovered] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xCC)};
    colors[ImGuiCol_HeaderActive] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
    colors[ImGuiCol_Separator] = ImColor{IM_COL32(0x66, 0x5c, 0x54, 0.50f)};
    colors[ImGuiCol_SeparatorHovered] =
        ImColor{IM_COL32(0x50, 0x49, 0x45, 0.78f)};
    colors[ImGuiCol_SeparatorActive] =
        ImColor{IM_COL32(0x66, 0x5c, 0x54, 0xFF)};
    colors[ImGuiCol_ResizeGrip] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0x40)};
    colors[ImGuiCol_ResizeGripHovered] =
        ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xAA)};
    colors[ImGuiCol_ResizeGripActive] =
        ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xF2)};
    colors[ImGuiCol_Tab] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0xD8)};
    colors[ImGuiCol_TabHovered] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xCC)};
    colors[ImGuiCol_TabActive] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
    colors[ImGuiCol_TabUnfocused] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0.97f)};
    colors[ImGuiCol_TabUnfocusedActive] =
        ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xFF)};
    colors[ImGuiCol_PlotLines] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0xFF)};
    colors[ImGuiCol_PlotLinesHovered] =
        ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
    colors[ImGuiCol_PlotHistogram] = ImColor{IM_COL32(0x98, 0x97, 0x1a, 0xFF)};
    colors[ImGuiCol_PlotHistogramHovered] =
        ImColor{IM_COL32(0xb8, 0xbb, 0x26, 0xFF)};
    colors[ImGuiCol_TextSelectedBg] = ImColor{IM_COL32(0x45, 0x85, 0x88, 0x59)};
    colors[ImGuiCol_DragDropTarget] =
        ImColor{IM_COL32(0x98, 0x97, 0x1a, 0.90f)};
    colors[ImGuiCol_TableHeaderBg] = ImColor{IM_COL32(0x38, 0x3c, 0x36, 0xFF)};
    colors[ImGuiCol_TableBorderStrong] =
        ImColor{IM_COL32(0x28, 0x28, 0x28, 0xFF)};
    colors[ImGuiCol_TableBorderLight] =
        ImColor{IM_COL32(0x38, 0x3c, 0x36, 0xFF)};
    colors[ImGuiCol_TableRowBg] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xFF)};
    colors[ImGuiCol_TableRowBgAlt] = ImColor{IM_COL32(0x28, 0x28, 0x28, 0xFF)};
    colors[ImGuiCol_TextSelectedBg] = ImColor{IM_COL32(0x45, 0x85, 0x88, 0xF0)};
    colors[ImGuiCol_NavHighlight] = ImColor{IM_COL32(0x83, 0xa5, 0x98, 0xFF)};
    colors[ImGuiCol_NavWindowingHighlight] =
        ImColor{IM_COL32(0xfb, 0xf1, 0xc7, 0xB2)};
    colors[ImGuiCol_NavWindowingDimBg] =
        ImColor{IM_COL32(0x7c, 0x6f, 0x64, 0x33)};
    colors[ImGuiCol_ModalWindowDimBg] =
        ImColor{IM_COL32(0x1d, 0x20, 0x21, 0x59)};
  }

  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  glfwSetErrorCallback(glfwErrorCallback);
  if (!glfwInit()) {
    return false;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  m_window = glfwCreateWindow(1280, 720, "Pagoda", nullptr, nullptr);
  if (m_window == nullptr) {
    return false;
  }

  m_device = MTLCreateSystemDefaultDevice();
  m_commandQueue = [m_device newCommandQueue];

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplMetal_Init(m_device);

  NSWindow *nswin = glfwGetCocoaWindow(m_window);
  m_layer = [CAMetalLayer layer];
  m_layer.device = m_device;
  m_layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
  nswin.contentView.layer = m_layer;
  nswin.contentView.wantsLayer = YES;

  m_renderPassDescriptor = [MTLRenderPassDescriptor new];

  return true;
}

void MetalGlfwPlatform::RunMainLoop() {
  // Main loop
  while (!glfwWindowShouldClose(m_window)) {
    @autoreleasepool {
      // Poll and handle events (inputs, window resize, etc.)
      // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
      // tell if dear imgui wants to use your inputs.
      // - When io.WantCaptureMouse is true, do not dispatch mouse input data
      // to your main application, or clear/overwrite your copy of the mouse
      // data.
      // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
      // data to your main application, or clear/overwrite your copy of the
      // keyboard data. Generally you may always pass all inputs to dear
      // imgui, and hide them from your application based on those two flags.
      glfwPollEvents();

      int width, height;
      glfwGetFramebufferSize(m_window, &width, &height);
      m_layer.drawableSize = CGSizeMake(width, height);
      id<CAMetalDrawable> drawable = [m_layer nextDrawable];

      id<MTLCommandBuffer> commandBuffer = [m_commandQueue commandBuffer];
      m_renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(
          m_clear_color[0] * m_clear_color[3],
          m_clear_color[1] * m_clear_color[3],
          m_clear_color[2] * m_clear_color[3], m_clear_color[3]);
      m_renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
      m_renderPassDescriptor.colorAttachments[0].loadAction =
          MTLLoadActionClear;
      m_renderPassDescriptor.colorAttachments[0].storeAction =
          MTLStoreActionStore;
      id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer
          renderCommandEncoderWithDescriptor:m_renderPassDescriptor];
      [renderEncoder pushDebugGroup:@"ImGui demo"];

      // Start the Dear ImGui frame
      ImGui_ImplMetal_NewFrame(m_renderPassDescriptor);
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      if (m_mainLoop != nullptr) {
        m_mainLoop->RunMainLoop();
      }

      // Rendering
      ImGui::Render();
      ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer,
                                     renderEncoder);

      [renderEncoder popDebugGroup];
      [renderEncoder endEncoding];

      [commandBuffer presentDrawable:drawable];
      [commandBuffer commit];
    }
  }
}

bool MetalGlfwPlatform::Teardown() {
  ImGui_ImplMetal_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(m_window);
  glfwTerminate();

  return true;
}

PlatformTexturePtr
MetalGlfwPlatform::CreateTexture(pagoda::image::ImagePtr image) {
  return std::make_shared<MetalTexture>(this, image);
}

extern "C" PAGODA_PLUGIN void *CreateWindow() {
  using FactoryClass =
      pagoda::common::PluggableFactory<PlatformWindow>;
  using BaseRegistrationClass = FactoryClass::Registration;

  struct Reg : public BaseRegistrationClass {
    virtual void Register(FactoryClass *factory) {
      factory->Register("metal", []() {
        return std::make_shared<MetalGlfwPlatform>();
      });
    }
  };

  return new Reg;
}
}

