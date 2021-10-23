#pragma once
#include "paperback_pch.h"
#include "Systems/WindowSystem.h"

#include <IconsFontAwesome5.h>
#include <sstream>
#include <filesystem>
//----------------------------------
// ImGui Headers
//----------------------------------
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <ImGuiFileBrowser.h>

//----------------------------------
// Panel Headers
//----------------------------------
#include "Editor/EditorPanels.h"
#include "Editor/EditorPanels_inline.h"
#include "Editor/EditorLogger.h"
#include "Editor/EditorLogger_Inline.h"

#include "Editor/Panels/EntityInspector.h"
#include "Editor/Panels/PropertyInspector.h"
#include "Editor/Panels/ArchetypeInspector.h"
#include "Editor/Panels/WindowSettings.h"
#include "Editor/Panels/AssetBrowser.h"
#include "Editor/Panels/ConsoleTerminal.h"

namespace fs = std::filesystem;

enum FileActivity
{
    NONE = 0,
    NEWSCENE,
    OPENSCENE,
    OPENFROMASSET,
    EXIT
};

struct imgui_system : paperback::system::instance
{
    using PanelList = std::vector < std::pair <const paperback::editor::type::info*, std::unique_ptr<paperback::editor::instance > >>;
    using PanelMap = std::unordered_map< paperback::editor::type::guid, paperback::editor::instance* >;

    PanelMap m_PanelsMap;
    PanelList m_Panels;

    GLFWwindow* m_pWindow;
    ImFont* m_Imgfont;

    paperback::archetype::instance* m_pArchetype; //refers back to the archetype that the entity is referencing to

    std::vector <rttr::instance> m_Components = {};
    std::vector <const char*> m_ComponentNames = {};

    std::string m_LoadedPath, m_LoadedFile, m_SelectedFile = {};

    std::pair< paperback::archetype::instance*, paperback::u32 > m_SelectedEntity;

    std::deque< std::pair< std::string, fs::path > > m_DisplayFilePath;

    imgui_addons::ImGuiFileBrowser m_FileDialog; // to access the file dialog addon
    paperback::EditorLogger m_Log;

    ImGuiDockNodeFlags m_Dockspaceflags;
    ImGuiWindowFlags m_Windowflags;

    fs::path m_SelectedPath = "../../resources";

    FileActivity m_Type = FileActivity::NONE;

    bool m_bDockspaceopen, m_bFullscreenpersistant, m_bFullscreen, m_bImgui, m_bDemoWindow;
    bool m_bFileOpen, m_bFileSaveAs, m_bSaveCheck;

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
        m_bFileOpen = m_bFileSaveAs = m_bSaveCheck = false;
        m_DisplayFilePath.push_front(std::make_pair("resources", "../../resources"));

        m_Log.Init();

        //-----------------------------------
        //         Register Panels
        //-----------------------------------
        AddPanels< EntityInspector, ArchetypeInspector, DetailsWindow, WindowSettings, AssetBrowser, ConsoleTerminal >();

        EDITOR_INFO_PRINT("Editor Loaded");

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
                SaveCheckPopUp();

                //Call Windows Here
                ImGui::PushFont(m_Imgfont);

                PanelsRun();
                //ImGui::ShowDemoWindow();

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
        ((AddPanel<T_PANELS>()), ...);
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
        pInfo->m_RunSystem(*pPanel, paperback::editor::type::call::CREATED);

        //save info
        m_PanelsMap.emplace(std::make_pair(paperback::editor::info_v<T_PANEL>.m_Guid, pPanel));

        return *(static_cast<T_PANEL*>(pPanel));

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
            if (Panel->IsEnabled())
                Info->m_RunSystem(*Panel, paperback::editor::type::call::UPDATE);
        }
    }

    void PanelMapClear() //call in terminate
    {
        m_Panels.clear();

        while (m_Panels.size())
        {
            auto p = m_Panels.back().second.release();
            m_Panels.back().first->m_Destructor(*p);
            delete reinterpret_cast<void*>(p);
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
                    m_Type = FileActivity::NEWSCENE;
                    m_bSaveCheck = true;
                }

                if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open Scene"))
                {
                    m_Type = FileActivity::OPENSCENE;
                    m_bSaveCheck = true;
                }

                if (ImGui::MenuItem(ICON_FA_SAVE " Save"))
                {
                    if (!m_LoadedPath.empty())
                    {
                        PPB.SaveScene(m_LoadedPath);
                        EDITOR_TRACE_PRINT(m_LoadedFile + " Saved at: " + m_LoadedPath);
                    }
                    else
                        m_bFileSaveAs = true;
                }

                if (ImGui::MenuItem("Save Scene As..."))
                {
                    m_bFileSaveAs = true;
                }

                if (ImGui::MenuItem(ICON_FA_POWER_OFF " Exit"))
                {
                    //TBC
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
                    GetPanel<DetailsWindow>()->Enable();

                ImGui::Separator();

                if (ImGui::MenuItem("Asset Browser"))
                    GetPanel<AssetBrowser>()->Enable();

                ImGui::Separator();

                if (ImGui::MenuItem("Console Terminal"))
                    GetPanel<ConsoleTerminal>()->Enable();

                if (ImGui::MenuItem("Window Settings"))
                    GetPanel<WindowSettings>()->Enable();

                ImGui::EndMenu();
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
            //m_FilePath = m_FileDialog.selected_path;
            //m_FileName = m_FileDialog.selected_fn;

            if (!m_FileDialog.selected_path.empty())
            {
                m_LoadedPath = m_FileDialog.selected_path;
                m_LoadedFile = m_FileDialog.selected_fn;

                ResetScene();

                PPB.OpenScene(m_FileDialog.selected_path);

                EDITOR_TRACE_PRINT( m_FileDialog.selected_fn + " Loaded" );
            }
        }
        else
            m_bFileOpen = false;

        if (m_bFileSaveAs)
            ImGui::OpenPopup("Save Scene As");

        if (m_FileDialog.showFileDialog("Save Scene As", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2{ 700, 310 }))
        {
            //m_FilePath = m_FileDialog.selected_path;
            //m_FileName = m_FileDialog.selected_fn;

            if (!m_FileDialog.selected_path.empty())
            {
                PPB.SaveScene(m_FileDialog.selected_path);

                EDITOR_TRACE_PRINT(m_FileDialog.selected_fn + " Saved at: " + m_FileDialog.selected_path);
            }
        }
        else
            m_bFileSaveAs = false; //dialog is closed
    }

    void DisplayClassType(const std::string& PropertyName, rttr::type& PropertyType, rttr::variant& PropertyValue)
    {
        if (PropertyType == rttr::type::get<std::reference_wrapper<paperback::Vector3f>>())
        {
            ImGui::DragFloat3(("##" + PropertyName).c_str(), (float*)&(PropertyValue.get_value<std::reference_wrapper<paperback::Vector3f>>().get()));
        }

        if (PropertyType == rttr::type::get<paperback::component::entity::Validation>())
        {
            ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
            ImGui::Text("%d", PropertyValue.get_value<paperback::component::entity::Validation>());
        }

    }

    void DisplayStringType(const std::string& PropertyName, rttr::type& PropertyType, rttr::variant& PropertyValue)
    {
        if (PropertyType == rttr::type::get<std::string>() || PropertyType == rttr::type::get<std::reference_wrapper<std::string>>())
        {
            if (!PropertyType.is_wrapper())
            {
                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                ImGui::Text(PropertyValue.get_value<std::string>().c_str());
            }
            else
            {
                auto& Str = PropertyValue.get_value<std::reference_wrapper<std::string>>().get();
                char Buffer[256]{};

                std::copy(Str.begin(), Str.end(), Buffer);
                if (ImGui::InputText(("##" + PropertyName).c_str(), Buffer, sizeof(Buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                    Str = std::string(Buffer);
            }
        }
    }

    void DisplayBaseTypes(const std::string& PropertyName, rttr::type& PropertyType, rttr::variant& PropertyValue)
    {
        if (PropertyType == rttr::type::get<float>() || PropertyType == rttr::type::get <std::reference_wrapper<float>>())
        {
            if (!PropertyType.is_wrapper())
            {
                ImGui::Text("%s: %f", PropertyName.c_str(), PropertyValue.get_value<float>());
            }
            else
            {
                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                ImGui::PushItemWidth(200.0f);
                ImGui::InputFloat(("##" + PropertyName).c_str(), &(PropertyValue.get_value<std::reference_wrapper<float>>().get()), 0.1f);
                ImGui::PopItemWidth();
            }
        }

        if (PropertyType == rttr::type::get<int>() || PropertyType == rttr::type::get<std::reference_wrapper<int>>())
        {
            if (!PropertyType.is_wrapper())
            {
                ImGui::Text("%s: %d", PropertyName.c_str(), PropertyValue.get_value<int>());
            }
            else
            {
                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                ImGui::PushItemWidth(200.0f);
                ImGui::InputInt(("##" + PropertyName).c_str(), &(PropertyValue.get_value<std::reference_wrapper<int>>().get()), 1);
                ImGui::PopItemWidth();
            }
        }

        if (PropertyType == rttr::type::get<bool>() || PropertyType == rttr::type::get <std::reference_wrapper<bool>>())
        {
            if (!PropertyType.is_wrapper())
            {
                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                ImGui::Text("%d", PropertyValue.get_value<bool>());
            }
            else
            {
                ImGui::Checkbox(PropertyName.c_str(), &(PropertyValue.get_value<std::reference_wrapper<bool>>().get()));
            }
        }

        if (PropertyType == rttr::type::get<size_t>())
        {
            ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
            ImGui::Text("%d", PropertyValue.get_value<size_t>());
        }
    }

    //void DisplayEnumTypes
    void ResetScene()
    {
        if (m_SelectedEntity.first)
        {
            m_SelectedEntity.first = nullptr; 
            m_SelectedEntity.second = paperback::u32_max;
        }

        if (!m_Components.empty())
            m_Components.clear();

        if (!PPB.GetArchetypeList().empty())
            PPB.ResetAllArchetypes();
    }

    void PopUpMessage(const std::string& WindowName, const char* Message) {

        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(WindowName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

            ImGui::Text(Message);

            if (ImGui::Button("OK"))
                ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }
    }

    void SaveCheckPopUp()
    {
        if (m_bSaveCheck)
            ImGui::OpenPopup("Confirmation");

        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) 
        {
            ImGui::Text("Are there any unsave changes?");
            ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f }, "There's no undo!!!");

            if (ImGui::Button("Yes"))
            {
                if (!m_LoadedPath.empty())
                {
                    PPB.SaveScene(m_LoadedPath);
                    EDITOR_TRACE_PRINT(m_LoadedFile + " Saved at: " + m_LoadedPath);
                    ImGui::CloseCurrentPopup();
                }
                else
                {
                    m_bFileSaveAs = true;
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGuiHelp("Click here to save changes");

            ImGui::SameLine();

            if (ImGui::Button("No"))
            {
                switch (m_Type)
                {
                case FileActivity::NEWSCENE:
                {
                    ResetScene();
                    EDITOR_INFO_PRINT("Created a New Scene");
                    ImGui::CloseCurrentPopup();
                }
                break;
                case FileActivity::OPENSCENE:
                {
                    m_bFileOpen = true;
                    ImGui::CloseCurrentPopup();
                }
                break;
                case FileActivity::OPENFROMASSET:
                {
                    if (!m_LoadedPath.empty() && !m_LoadedFile.empty())
                    {
                        ResetScene();
                        PPB.OpenScene(m_LoadedPath);

                        EDITOR_TRACE_PRINT(m_LoadedFile + " Loaded");
                    }
                }
                break;
                }
            }

            ImGuiHelp("Click here to continue on");

            ImGui::EndPopup();
        }

        m_bSaveCheck = false;
    }

    void ImGuiHelp(const char* description, int symbol = 0) {

        if (symbol)
            ImGui::TextDisabled(ICON_FA_EXCLAMATION_CIRCLE);

        if (ImGui::IsItemHovered()) {

            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(description);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }
};

#include "Editor/Panels/EntityInspector_Inline.h"
#include "Editor/Panels/ArchetypeInspector_Inline.h"
#include "Editor/Panels/PropertyInspector_Inline.h"
#include "Editor/Panels/WindowSettings_Inline.h"
#include "Editor/Panels/AssetBrowser_Inline.h"
#include "Editor/Panels/ConsoleTerminal_Inline.h"

