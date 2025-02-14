#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imconfig.h>
#include <imgui_internal.h>
#include <imstb_rectpack.h>
#include <imstb_textedit.h>
#include <imstb_truetype.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Hazy/LayerStack/Layer.h"
#include "Hazy/Window.h"
#include "Hazy/EventSystem.h"

namespace Hazy {

    std::atomic<bool> ImGuiLayer::s_ImGuiInited(false);

    ImGuiLayer::ImGuiLayer(Window* window, const std::function<void()>& renderFunc)
        : Layer(window), m_renderFunc(renderFunc) {
        if (!s_ImGuiInited.load()) {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            ImGui::StyleColorsDark();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

            ImGuiStyle& style = ImGui::GetStyle();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                style.WindowRounding = 0.0f;
                style.Colors[ImGuiCol_WindowBg].w = 1.0f;
            }

            {
                ContextLock lock(window->getRenderContext());
                ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window->getRenderContext().getNativeWindow()), true);
                ImGui_ImplOpenGL3_Init("#version 460");
            }

            s_ImGuiInited = true;
        }
        else {
            Logger::LogCritical("ImGuiLayer already instanciated, ImGuiLayer can only be instanciated once");
            std::exit(EXIT_FAILURE);
        }
    }

    ImGuiLayer::ImGuiLayer(Window* window)
        : ImGuiLayer(window, [] { ImGui::ShowDemoWindow(); }) {
    }

    ImGuiLayer::~ImGuiLayer() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::update() {

        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        {
            m_renderFunc();
        }

        {
            ImGuiIO& io = ImGui::GetIO();

            io.DisplaySize = ImVec2(m_window->getWidth(), m_window->getHeight());

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }
    }
}