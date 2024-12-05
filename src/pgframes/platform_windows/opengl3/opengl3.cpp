#include "opengl3.h"

#include "pagoda/api.h"
#include "pagoda/common/pluggable_factory.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

namespace pgframes {
namespace {
static void glfwErrorCallback(int error, const char *description) {
  LOG_ERROR("GLFW Error " << error << " " << description);
}
}

bool OpenGL3GlfwPlatform::Startup() {
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

  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  m_window = glfwCreateWindow(1280, 720, "Pagoda", nullptr, nullptr);
  if (m_window == nullptr) {
    return false;
  }

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1); // Enable vsync

  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  const char* glsl_version = "#version 130";
  ImGui_ImplOpenGL3_Init(glsl_version);

  glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
    std::cerr << "glew initialisation failed!\n" << std::endl;
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

  return true;
}

void OpenGL3GlfwPlatform::RunMainLoop() {
  // Main loop
  while (!glfwWindowShouldClose(m_window)) {
    // Poll and handle events (inputs, m_window resize, etc.)
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

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (m_mainLoop != nullptr) {
      m_mainLoop->RunMainLoop();
    }

    /*
    if (m_frameBuffer == nullptr) {
      m_frameBuffer = std::make_shared<FrameBuffer>(800, 600);
    }

    m_frameBuffer->Bind();
    glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    m_frameBuffer->Unbind();


    if (ImGui::Begin("TestWindow")) {
      ImTextureID my_tex_id = (void*)(intptr_t)m_frameBuffer->getFrameTexture();
      ImGui::Image(my_tex_id,
                   ImVec2(800, 600));
    }
    ImGui::End();

    m_frameBuffer->Unbind();
    */

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
  }
}

bool OpenGL3GlfwPlatform::Teardown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(m_window);
  glfwTerminate();

  return true;
}

PlatformTexturePtr
OpenGL3GlfwPlatform::CreateTexture(pagoda::image::ImagePtr image) {
  return nullptr;
}
}

extern "C" PAGODA_PLUGIN void *CreateWindow() {
  using FactoryClass =
      pagoda::common::PluggableFactory<pgframes::PlatformWindow>;
  using BaseRegistrationClass = FactoryClass::Registration;

  struct Reg : public BaseRegistrationClass {
    virtual void Register(FactoryClass *factory) {
      factory->Register("opengl3", []() {
        return std::make_shared<pgframes::OpenGL3GlfwPlatform>();
      });
    }
  };
  return new Reg;
}
