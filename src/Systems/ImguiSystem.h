#pragma once
#include "paperback_pch.h"
#include "WindowSystem.h"

#include <IconsFontAwesome5.h>
#include <sstream>
//----------------------------------
// ImGui Headers
//----------------------------------
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct imgui_system : paperback::system::instance
{
    GLFWwindow* m_pWindow;
    ImFont* m_Imgfont;
    //paperback::archetype::instance* m_pEntity; //refers back to the archetype that the entity is referencing to
    std::vector <rttr::instance> m_Components = {};
    paperback::u32 m_EntityNum;

    ImGuiDockNodeFlags m_Dockspaceflags;
    ImGuiWindowFlags m_Windowflags;

    bool m_bDockspaceopen, m_bFullscreenpersistant, m_bFullscreen, m_bImgui, m_bDemoWindow;


    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "imgui_system"
    };

    //Handle Imgui Initialization Here
    PPB_INLINE
        void OnSystemCreated(void) noexcept
    {
        m_pWindow = GetSystem< window_system >().m_pWindow; //Get window ptr
        ImGuiContext(); //Setup ImGui Context
        m_bImgui = true;
        m_bDemoWindow = false;
        //m_pEntity = nullptr;
        m_EntityNum = -1; //need to update this initialization
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
                ImGuiDockingSetup();

                ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
                ImGui::SetNextWindowBgAlpha(0.0f); // set the transparency of the docking central node
                ImGui::Begin("DockSpace", &m_bDockspaceopen, m_Windowflags);
                ImGui::PopStyleVar();

                if (m_bFullscreen)
                    ImGui::PopStyleVar(2);

                EditorMenuBar();
                //Popups();

                if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {

                    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
                    ImGui::DockSpace( dockspace_id, ImVec2( 0.0f, 0.0f ), m_Dockspaceflags );
                }


                //Call Windows Here
                ImGui::PushFont(m_Imgfont);

                InspectorPanel();
                ComponentInspector();

                ImGui::PopFont();

                if (m_bDemoWindow)
                    ImGui::ShowDemoWindow();

                ImGui::End(); //End of Docking Space
            }

            ImGuiRender();
        }
    }

    PPB_INLINE
        void Terminated(void) noexcept
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void EditorMenuBar()
    {
        if (ImGui::BeginMenuBar())
        {
            ImGui::PushFont(m_Imgfont);

            if (ImGui::BeginMenu( ICON_FA_FOLDER " File" ))
            {
                if (ImGui::MenuItem( ICON_FA_TIMES " New Scene" ))
                {
                    
                }

                if (ImGui::MenuItem( ICON_FA_FOLDER_OPEN " Open Scene" ))
                {

                }

                if (ImGui::MenuItem( ICON_FA_SAVE " Save" ))
                {
                    //link the file browser here
                }

                if (ImGui::MenuItem( "Save Scene As..." ))
                {
                    //link the file browser here
                }

                if (ImGui::MenuItem( ICON_FA_REPLY " Return to Menu" ))
                {

                }

                if (ImGui::MenuItem( ICON_FA_POWER_OFF " Exit" ))
                {

                }

                ImGui::EndMenu();
            }

            ImGui::PopFont();
        }

        if (ImGui::Selectable("Show Demo Window"))
            m_bDemoWindow = !m_bDemoWindow;

        ImGui::EndMenuBar();
    }


    void InspectorPanel()
    {
        ImGui::Begin("Entity Inspector");
        //ImGui::PushFont(m_Imgfont);


        //entityinfo has a archetype::instance*
        //for (int i = 0; i < PPB.m_EntityMgr.m_pArchetypeList.size(); ++i)
        //{
        //    bool b_Selected = (Selection_Mask & (1 << i)) != 0;

        //    if (b_Selected) NodeFlags |= ImGuiTreeNodeFlags_Selected;

        //    b_NodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, NodeFlags, "Entity %d", i);
        //    if (ImGui::IsItemClicked())
        //    {
        //        Components = PPB.m_EntityMgr.m_pArchetypeList[i].GetEntityComponents();
        //    }

        //    if (b_NodeOpen)
        //    {
        //        std::cout << "opened" << std::endl;
        //        ImGui::TreePop();
        //    }
        //    std::stringstream ss;
        //    ss << ICON_FA_CUBE << " Entity (" << std::to_string(i) << ")";
        //    if (ImGui::Selectable(ss.str().c_str()))
        //    {
        //    }


        //}


        bool b_NodeOpen{ false };
        for (auto& Archetype : PPB.m_EntityMgr.m_pArchetypeList)
        {
            for (paperback::u32 i = 0; i < Archetype->m_EntityCount; ++i)
            {

                ImGuiTreeNodeFlags NodeFlags = (m_EntityNum == i ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

                std::stringstream ss; ss << "Entity (" << i << ")";

                b_NodeOpen = ImGui::TreeNodeEx((void*)(size_t)i, NodeFlags, ss.str().c_str());

                if (ImGui::IsItemClicked())
                {
                    m_EntityNum = i; //Using the index to access the components for each entity
                    std::cout << m_EntityNum << std::endl;

                    m_Components = Archetype->GetEntityComponents(m_EntityNum);

                }

                if (b_NodeOpen)
                {
                    // for spawning entities etc 
                    ImGui::Text("To handle spawning of entities and stuff");
                    ImGui::TreePop();
                }
            }
        }

        //ImGui::PopFont();
        ImGui::End();
    }


    void ComponentInspector()
    {
        ImGui::Begin("Component Inspector");
        if (!m_Components.empty())
        {
            for (auto& ComponentInstance : m_Components)
            {
                if (ImGui::CollapsingHeader(ComponentInstance.get_type().get_name().to_string().c_str()))
                {
                    for (auto& property : ComponentInstance.get_type().get_properties())
                    {
                        rttr::variant propValue = property.get_value(ComponentInstance);

                        if (!propValue) continue;

                        auto propertyType = property.get_type(); //etc vector 3, std::string etc
                        auto propertyName = property.get_name().to_string();

                        //propertyName.assign(propertyName + ComponentInstance.get_type().get_name().to_string());

                        if (propertyType == rttr::type::get<std::reference_wrapper<xcore::vector3>>())
                        {
                            auto& ref = propValue.get_value<std::reference_wrapper<xcore::vector3>>().get();
                            ImGui::Text(propertyName.c_str());
                            ImGui::DragFloat4(("##" + propertyName).c_str(), (float*)&ref, 0.01f);

                        }

                        if (propertyType == rttr::type::get<std::string>())
                        {
                            auto& str = propValue.get_value<std::string>();
                            ImGui::Text((propertyName + ": ").c_str()); ImGui::SameLine();
                            ImGui::Text(str.c_str());
                        }

                    }
                }
            }
        }

        ImGui::End();
    }



















































    ///////////////////////////////////////////////////////////////////////////////////////////////////////

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

        //Load Custom Fonts
        io.FontDefault = io.Fonts->AddFontFromFileTTF("../../resources/fonts/FredokaOne-Regular.ttf", 16.0f);
        static const ImWchar iconranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        ImFontConfig iconsconfig; iconsconfig.MergeMode = true; iconsconfig.PixelSnapH = true;
        m_Imgfont = io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 14.0f, &iconsconfig, iconranges);

        //////////// End iof ImGui Context Setup///////////////////////////

        m_bDockspaceopen = true;
        m_bFullscreenpersistant = true;
        m_bFullscreen = m_bFullscreenpersistant;

        m_Dockspaceflags = ImGuiDockNodeFlags_PassthruCentralNode;
        m_Windowflags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    }

    void ImGuiRender()
    {
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

    void ImGuiDockingSetup()
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
    }


};