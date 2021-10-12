#pragma once
#include "paperback_pch.h"
#include "Systems/WindowSystem.h"
#include "Json/paperback_json.h"

#include <IconsFontAwesome5.h>
#include <sstream>
//----------------------------------
// ImGui Headers
//----------------------------------
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <ImGuiFileBrowser.h>

#include "Editor/EditorPanels.h"
#include "Editor/EditorPanels_inline.h"

#include "Editor/Panels/Editor_EntityInspector.h"
#include "Editor/Panels/Editor_ComponentInspector.h"
#include "Editor/Panels/Editor_ArchetypeInspector.h"
#include "Editor/Panels/Editor_ComponentEdit.h"
#include "Editor/Panels/Editor_WindowSettings.h"


struct imgui_system : paperback::system::instance
{
    using PanelList = std::vector < std::pair <const paperback::editor::type::info*, std::unique_ptr<paperback::editor::instance > >>;
    using PanelMap = std::unordered_map< paperback::editor::type::guid, paperback::editor::instance* >;

    PanelMap m_PanelsMap;
    PanelList m_Panels;

    GLFWwindow* m_pWindow;
    ImFont* m_Imgfont;

    paperback::JsonFile JFile;

    paperback::archetype::instance* m_pArchetype; //refers back to the archetype that the entity is referencing to

    std::vector <rttr::instance> m_Components = {};
    std::vector <const char*> m_ComponentNames = {};

    std::string m_FilePath, m_FileName, m_LoadedPath;

    std::pair<paperback::archetype::instance*, paperback::u32> m_SelectedEntity;

    imgui_addons::ImGuiFileBrowser m_FileDialog; // to access the file dialog addon

    ImGuiDockNodeFlags m_Dockspaceflags;
    ImGuiWindowFlags m_Windowflags;

    bool m_bDockspaceopen, m_bFullscreenpersistant, m_bFullscreen, m_bImgui, m_bDemoWindow;
    bool m_bFileSave, m_bFileOpen, m_bFileSaveAs;

    ////////////////////////////////////////////////////////////////////////////////

    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "imgui_system"
    };

    PPB_INLINE
    void OnSystemCreated(void) noexcept
    {
        m_pWindow = GetSystem< window_system >().m_pWindow; //Get window ptr

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

        m_bImgui = true;


    //-----------------------------------
	//         Register Panels
	//-----------------------------------
        AddPanels< EntityInspector, ArchetypeInspector, ComponentInspector, ComponentEditor, WindowSettings >();
    }

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


                if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {

                    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
                    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), m_Dockspaceflags);
                }
                EditorMenuBar();
                OpenSaveFile();

                //Call Windows Here
                ImGui::PushFont(m_Imgfont);

                PanelsRun();

                ImGui::PopFont();
                ImGui::End(); //End of Docking Space
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
    void OnSystemTerminated(void) noexcept
    {
        PanelMapClear();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }





    //-----------------------------------
    //        Panel Manager
    //-----------------------------------

    template <typename... T_PANELS>
    constexpr void AddPanels() noexcept
    {
        ( ( AddPanel<T_PANELS>()), ... );
    }

    template < typename T_PANEL >
    constexpr T_PANEL& AddPanel() noexcept
    {
        //Register Panels
        m_Panels.push_back({ &paperback::editor::info_v<T_PANEL>, std::make_unique< paperback::editor::details::completed<T_PANEL> >(*this) });

        //Initialize Pointers
        auto* pInfo = m_Panels.back().first;
        auto* pPanel = m_Panels.back().second.get();

        //run init()
        pInfo->m_RunSystem( *pPanel, paperback::editor::type::call::CREATED );

        //save info
        m_PanelsMap.emplace( std::make_pair( paperback::editor::info_v<T_PANEL>.m_Guid, pPanel ) );

        return *(static_cast<T_PANEL*>( pPanel ) );

    }

    template < typename T_PANEL >
    T_PANEL* GetPanel(void)
    {
        return m_PanelsMap.find(paperback::editor::info_v<T_PANEL>.m_Guid) != m_PanelsMap.end()
            ? static_cast<T_PANEL*>(m_PanelsMap.find(paperback::editor::info_v<T_PANEL>.m_Guid)->second)
            : nullptr;
    }


    void PanelsRun() //Call in update
    {
        for (const auto& [Info, Panel] : m_Panels)
        {
            if(Panel->IsEnabled())
                Info->m_RunSystem(*Panel, paperback::editor::type::call::UPDATE);
        }
    }

    void PanelMapClear() //call in terminate
    {
        m_Panels.clear();

        while ( m_Panels.size() )
        {
            auto p = m_Panels.back().second.release();
            m_Panels.back().first->m_Destructor( *p );
            delete reinterpret_cast<void*>( p );
            m_Panels.pop_back();
        }
    }


    ////////////////////////////////////////////////////////////////////////////

    void EditorMenuBar()
    {
        if (ImGui::BeginMenuBar())
        {
            ImGui::PushFont(m_Imgfont);

            if (ImGui::BeginMenu(ICON_FA_FOLDER " File"))
            {
                if (ImGui::MenuItem(ICON_FA_TIMES " New Scene"))
                {
                    if (m_SelectedEntity.first)
                        m_SelectedEntity.first = nullptr; m_SelectedEntity.second = paperback::u32_max;

                    if (!m_Components.empty())
                        m_Components.clear();

                    if (!PPB.GetArchetypeList().empty())
                        PPB.ResetAllArchetypes();
                }

                if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open Scene"))
                {
                    m_bFileOpen = true;
                }

                if (ImGui::MenuItem(ICON_FA_SAVE " Save"))
                {
                    if (!m_LoadedPath.empty())
                    {
                        PPB.SaveScene(m_LoadedPath);
                        m_bFileSave = true;
                    }
                    else
                        m_bFileSaveAs = true;
                }

                if (ImGui::MenuItem("Save Scene As..."))
                {
                    m_bFileSaveAs = true;
                }

                if (ImGui::MenuItem(ICON_FA_REPLY " Return to Menu"))
                {
                }

                if (ImGui::MenuItem(ICON_FA_POWER_OFF " Exit"))
                {
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Editor Panels"))
            {
                if (ImGui::MenuItem("World Outliner"))
                    GetPanel<EntityInspector>()->Enable();

                if (ImGui::MenuItem("Archetype/Prefabs"))
                    GetPanel<ArchetypeInspector>()->Enable();

                if (ImGui::MenuItem("Entity Details"))
                    GetPanel<ComponentInspector>()->Enable();

                // ImGui::Separator();

                // if (ImGui::MenuItem("Asset Browser") )
                //     GetPanel<AssetBrowser>()->Enable();

                ImGui::Separator();

                if (ImGui::MenuItem("Window Settings"))
                    GetPanel<WindowSettings>()->Enable();
            }

            ImGui::PopFont();

            ImGui::PushItemWidth(100.0f);
            ImGui::Text("FPS: %d", PPB.GetFPS());
            ImGui::PopItemWidth();
        }

        ImGui::EndMenuBar();
    }

    void OpenSaveFile()
    {
        if (m_bFileOpen)
            ImGui::OpenPopup("Open Scene");

        if (m_FileDialog.showFileDialog("Open Scene", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2{ 700, 310 }))
        {
            m_FilePath = m_FileDialog.selected_path;
            m_FileName = m_FileDialog.selected_fn;

            if (!m_FilePath.empty())
            {
                m_LoadedPath = m_FilePath;

                if (m_SelectedEntity.first)
                    m_SelectedEntity.first = nullptr; m_SelectedEntity.second = paperback::u32_max;

                if (!m_Components.empty())
                    m_Components.clear();

                if (!PPB.GetArchetypeList().empty())
                    PPB.ResetAllArchetypes();

                PPB.OpenScene(m_FilePath);
            }
        }
        else
            m_bFileOpen = false;

        if (m_bFileSaveAs)
            ImGui::OpenPopup("Save Scene As");

        if (m_FileDialog.showFileDialog("Save Scene As", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2{ 700, 310 }))
        {
            m_FilePath = m_FileDialog.selected_path;
            m_FileName = m_FileDialog.selected_fn;

            if (!m_FilePath.empty())
            {
                PPB.SaveScene(m_FilePath);
                m_bFileSave = true;
            }
        }
        else
            m_bFileSaveAs = false; //dialog is closed
    }

};