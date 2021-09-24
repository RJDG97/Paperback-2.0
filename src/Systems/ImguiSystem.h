#pragma once
#include "paperback_pch.h"
#include "WindowSystem.h"

//----------------------------------
// ImGui Headers
//----------------------------------
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct imgui_system : paperback::system::instance
{
    GLFWwindow* m_pWindow;

    ImGuiDockNodeFlags m_Dockspaceflags;
    ImGuiWindowFlags m_Windowflags;

    bool m_bDockspaceopen, m_bFullscreenpersistant, m_bFullscreen, m_bImgui;


    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "imgui_system"
    };

    //Handle Imgui Initialization Here
    PPB_INLINE
        void OnSystemCreated(void) noexcept
    {
        m_pWindow = GetSystem<window_system>().m_pWindow; //Get window ptr
        //m_bImgui = true;
        ImGuiContext();
        m_bImgui = true;
    }

    //Handle Imgui Main Loop Here (For all the windows)
    PPB_INLINE
        void Update(void)
    {
        if (m_bImgui)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGuiIO& io = ImGui::GetIO();

            if (m_bDockspaceopen)
            {
                m_bFullscreen = m_bFullscreenpersistant;
                if (m_bFullscreen) {

                    ImGuiViewport* viewport = ImGui::GetMainViewport();

                    ImGui::SetNextWindowPos(viewport->Pos);
                    ImGui::SetNextWindowSize(viewport->Size);
                    ImGui::SetNextWindowViewport(viewport->ID);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

                    m_Windowflags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                    m_Windowflags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                }

                if (m_Dockspaceflags & ImGuiDockNodeFlags_PassthruCentralNode)
                    m_Windowflags |= ImGuiWindowFlags_NoBackground;

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::SetNextWindowBgAlpha(0.0f); // set the transparency of the docking central node
                ImGui::Begin("DockSpace", &m_bDockspaceopen, m_Windowflags);
                ImGui::PopStyleVar();

                if (m_bFullscreen)
                    ImGui::PopStyleVar(2);

                //ImguiMenuBar();
                //Popups();
                ImGui::ShowDemoWindow();

                if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {

                    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
                    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), m_Dockspaceflags);
                }

                ImGui::Begin("Hey");
                ImGui::Text("Nui");
                ImGui::End();

                ImGui::End();
            }

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(m_pWindow, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }
    }

    PPB_INLINE
        void OnFrameStart(void) noexcept {}

    PPB_INLINE
        void OnFrameEnd(void) noexcept {}

    PPB_INLINE
        void Terminated(void) noexcept
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiContext()
    {
        //////////// Setup Dear ImGui context///////////////////////////

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();

        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking (Merging of windows)
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
        ImGui_ImplOpenGL3_Init(NULL);

        //////////// End iof ImGui Context Setup///////////////////////////

        m_bDockspaceopen = true;
        m_bFullscreenpersistant = true;
        m_bFullscreen = m_bFullscreenpersistant;

        m_Dockspaceflags = ImGuiDockNodeFlags_PassthruCentralNode;
        m_Windowflags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    }
};