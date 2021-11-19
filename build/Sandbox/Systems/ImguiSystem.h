#pragma once
#include "WindowSystem.h"

#include <dearImGui/IconsFontAwesome5.h>
#include <sstream>
#include <filesystem>
//----------------------------------
// ImGui Headers
//----------------------------------
#include <dearImGui/imgui.h>
#include <dearImGui/imgui_impl_glfw.h>
#include <dearImGui/imgui_impl_opengl3.h>
#include <dearImGui/imgui_internal.h>
#include <dearImGui/ImGuiFileBrowser.h>
#include <dearImGui/ImGuizmo/ImGuizmo.h>
//----------------------------------
// Panel Headers
//----------------------------------
#include "Editor/EditorPanels.h"
#include "Editor/EditorPanels_inline.h"
#include "Editor/EditorLogger.h"
#include "Editor/EditorLogger_Inline.h"

#include "Editor/Panels/Viewport.h"
#include "Editor/Panels/EntityInspector.h"
#include "Editor/Panels/PropertyInspector.h"
#include "Editor/Panels/ArchetypeInspector.h"
#include "Editor/Panels/AssetBrowser.h"
#include "Editor/Panels/ConsoleTerminal.h"
#include "Editor/Panels/WindowSettings.h"


namespace fs = std::filesystem;

enum FileActivity
{
    NONE = 0,
    NEWSCENE,
    OPENSCENE,
    OPENFROMASSET,
    SAVEPREFAB,
    LOADPREFAB,
    LOADFROMASSET,
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

    std::vector < std::pair < rttr::instance, paperback::component::type::guid> > m_Components = {};
    std::vector <const char*> m_ComponentNames = {};

    std::string m_LoadedPath, m_LoadedFile, m_SelectedFile = {}, m_FolderToDelete, m_FileToDelete, m_EntityInfoLoadedPath;
    std::string m_LoadedPrefabPath, m_LoadedPrefabFile;

    std::pair< paperback::archetype::instance*, paperback::u32 > m_SelectedEntity; //first: pointer to the archetype | second: entity index

    std::deque< std::pair< std::string, fs::path > > m_DisplayFilePath;

    imgui_addons::ImGuiFileBrowser m_FileDialog; // to access the file dialog addon
    paperback::EditorLogger m_Log;

    ImGuiDockNodeFlags m_Dockspaceflags;
    ImGuiWindowFlags m_Windowflags;

    fs::path m_SelectedPath = "../../resources";

    FileActivity m_Type = FileActivity::NONE;

    bool m_bDockspaceopen, m_bFullscreenpersistant, m_bFullscreen, m_bImgui, m_bDemoWindow;
    bool m_bFileOpen, m_bFileSaveAs, m_bSaveCheck, m_bLoadPrefab, m_bSavePrefab;

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
        m_bFileOpen = m_bFileSaveAs = m_bSaveCheck = m_bSavePrefab = m_bLoadPrefab = false;
        m_DisplayFilePath.push_front(std::make_pair("resources", "../../resources"));

        m_Log.Init(); //Init ImTerm (Console)

        //-----------------------------------
        //         Register Panels
        //-----------------------------------
        AddPanels< EntityInspector, ArchetypeInspector, DetailsWindow, EditorViewport, 
                   WindowSettings, AssetBrowser, ConsoleTerminal
        >();

        EDITOR_INFO_PRINT("Editor Loaded");
    }

    PPB_INLINE
    void Update(void)
    {
        if (m_bImgui)
        {
            // ImGui New Frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //ImGuizmo Start
            ImGuizmo::BeginFrame();

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
                SaveLoadPrefab();

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

    PPB_INLINE
    void OnStateChange(void) noexcept
    {
        m_bImgui = (PPB.VerifyState("Editor")) ? true : false;
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
                    if (!m_LoadedPath.empty() && !m_EntityInfoLoadedPath.empty())
                    {
                        PPB.SaveScene(m_LoadedPath, m_EntityInfoLoadedPath);
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

                if (ImGui::MenuItem("Details Panel"))
                    GetPanel<DetailsWindow>()->Enable();

                if (ImGui::MenuItem("Viewport"))
                    GetPanel<EditorViewport>()->Enable();

                ImGui::Separator();

                if (ImGui::MenuItem("Content Browser"))
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

    //-----------------------------------
    //        File Loading/Saving
    //-----------------------------------

    void OpenSaveFile()
    {
        if (m_bFileOpen)
            ImGui::OpenPopup("Open Scene");

        if (m_FileDialog.showFileDialog("Open Scene", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2{ 700, 310 }))
        {
            if (!m_FileDialog.selected_path.empty())
                OpenFile(m_FileDialog.selected_path, m_FileDialog.selected_fn);
        }
        else
            m_bFileOpen = false;

        if (m_bFileSaveAs)
            ImGui::OpenPopup("Save Scene As");

        if (m_FileDialog.showFileDialog("Save Scene As", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2{ 700, 310 }))
        {
            if (!m_FileDialog.selected_path.empty())
                SaveFileAs(m_FileDialog.selected_path, m_FileDialog.selected_fn);
        }
        else
            m_bFileSaveAs = false; //dialog is closed
    }

    void SaveFileAs(std::string& FilePath, std::string& FileName)
    {
        std::string EntityInfoPath;

        if (!m_bSavePrefab)
        {
            if (FileName.find(".json") == std::string::npos)
                FileName.append(".json");

            if (FilePath.find(".json") == std::string::npos)
                FilePath.append(".json");

            EntityInfoPath = SetEntityInfoPath(FilePath, FileName);

            if (!EntityInfoPath.empty())
                PPB.SaveScene(FilePath, EntityInfoPath);
        }
        else
        {
            if (FileName.find(".prefab") == std::string::npos)
                FileName.append(".prefab");

            if (FilePath.find(".prefab") == std::string::npos)
                FilePath.append(".prefab");

            PPB.SavePrefabs(FilePath);
            m_bSavePrefab = false;
        }

        EDITOR_TRACE_PRINT(FileName + " Saved at: " + FilePath);
    }

    void OpenFile(std::string& FilePath, std::string& FileName)
    {
        std::string EntityInfoPath;

        if (!m_bLoadPrefab)
        {
            EntityInfoPath = SetEntityInfoPath(FilePath, FileName);

            if (!EntityInfoPath.empty())
            {

                PPB.OpenEditScene(FilePath, EntityInfoPath);
                EDITOR_TRACE_PRINT(FileName + " Loaded");

                m_LoadedPath = FilePath;
                m_LoadedFile = FileName;
                m_EntityInfoLoadedPath = EntityInfoPath;
            }
            else
                EDITOR_CRITICAL_PRINT("Missing EntityInfo File");
        }
        else
        {

            if (FilePath.find(".prefab") == std::string::npos)
            {
                EDITOR_CRITICAL_PRINT("Trying to load a Non Prefab File");
                return;
            }
            
            PPB.LoadPrefabs(FilePath);
            EDITOR_TRACE_PRINT("Prefabs from: " + FileName + " Loaded");

            m_bLoadPrefab = false;
        }
    }

    void SaveCheckPopUp()
    {
        if (m_bSaveCheck)
            ImGui::OpenPopup(ICON_FA_EXCLAMATION_TRIANGLE " Confirm?");

        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(ICON_FA_EXCLAMATION_TRIANGLE " Confirm?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f }, "Have you save whatever you're working on?");

            if (ImGui::Button(ICON_FA_FOLDER_OPEN " Yes"))
            {
                switch (m_Type)
                {
                case FileActivity::NEWSCENE:
                {
                    ResetScene();
                    PPB.Initialize();
                    EDITOR_INFO_PRINT("New Scene Created");

                    m_Type = FileActivity::NONE;
                    ImGui::CloseCurrentPopup();
                }
                break;
                case FileActivity::OPENSCENE:
                {
                    ResetScene();

                    m_bFileOpen = true;

                    m_Type = FileActivity::NONE;
                    ImGui::CloseCurrentPopup();
                }
                break;
                case FileActivity::OPENFROMASSET:
                {
                    if (!m_LoadedPath.empty() && !m_LoadedFile.empty())
                    {
                        ResetScene();

                        OpenFile(m_LoadedPath, m_LoadedFile);

                        m_Type = FileActivity::NONE;
                        ImGui::CloseCurrentPopup();
                    }
                }
                break;
                }
            }

            //ImGuiHelp("Click here to save changes");

            ImGui::SameLine();

            if (ImGui::Button(ICON_FA_SAVE " No"))
            {
                if (!m_LoadedPath.empty() && !m_EntityInfoLoadedPath.empty())
                {
                    PPB.SaveScene(m_LoadedPath, m_EntityInfoLoadedPath);
                    EDITOR_TRACE_PRINT(m_LoadedFile + " Saved at: " + m_LoadedPath);

                    m_Type = FileActivity::NONE;
                    ImGui::CloseCurrentPopup();
                }
                else
                {
                    m_bFileSaveAs = true;

                    m_Type = FileActivity::NONE;
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::SameLine();

            if (ImGui::Button(ICON_FA_TIMES " Cancel"))
            {
                m_Type = FileActivity::NONE;
                ImGui::CloseCurrentPopup();
            }

            //ImGuiHelp("Click here to continue on");

            ImGui::EndPopup();
        }

        m_bSaveCheck = false;
    }

    void SaveLoadPrefab()
    {
        switch (m_Type)
        {
        case FileActivity::SAVEPREFAB:
        {
            //if (!m_LoadedPrefabPath.empty())
            //{
            //    PPB.SavePrefabs(m_LoadedPrefabFile);
            //    EDITOR_TRACE_PRINT(m_LoadedPrefabFile + " Saved at: " + m_LoadedPrefabPath);

            //    m_Type = FileActivity::NONE;
            //}
            //else
            //{
            //    m_bSavePrefab = true;
            //    m_bFileSaveAs = true;
            //    m_Type = FileActivity::NONE;

            //}

            m_bSavePrefab = true;
            m_bFileSaveAs = true;
            m_Type = FileActivity::NONE;
        }
        break;
        case FileActivity::LOADPREFAB:
        {
            m_bLoadPrefab = true;
            m_bFileOpen = true;
            m_Type = FileActivity::NONE;
        }
        break;
        case FileActivity::LOADFROMASSET:
        {
            if (!m_LoadedPrefabPath.empty())
            {
                m_bLoadPrefab = true;
                OpenFile(m_LoadedPrefabPath, m_LoadedPrefabFile);
                m_Type = FileActivity::NONE;
            }
        }
        break;
        }
    }

    //-----------------------------------
    //        Property Display
    //-----------------------------------

    void EnumerationCombo(std::vector<const char*> Value_List, const std::string& Name, paperback::u8& Selection)
    {
        //const char* current_item = Value_List[Selection];
        if (ImGui::BeginCombo(("##" + Name).c_str(), "Choose Layer"))
        {
            for (unsigned i = 0; i < Value_List.size(); ++i)
            {
                //bool is_selected = (current_item == Value_List[i]);
                if (ImGui::Selectable(Value_List[i]))
                    Selection = i;
            }
            ImGui::EndCombo();
        }
    }

    void DisplayEnumeration(const std::string& PropertyName, rttr::type& PropertyType, rttr::variant& PropertyValue, rttr::instance& Instance, rttr::property Property)
    {
        auto Enumeration = PropertyType.get_enumeration();
        auto EnumList = Enumeration.get_names();
        std::vector<const char*> ValueList;

        for (auto& Enum : EnumList)
            ValueList.push_back(Enum.data());

        auto Selection = PropertyValue.get_value<paperback::u8>();
        EnumerationCombo(ValueList, PropertyName, Selection);

        auto Result = Enumeration.name_to_value(Enumeration.value_to_name(Selection));

        if (PropertyName == "Collidable Layers")
        {
            auto EntityCollideLayers = m_SelectedEntity.first->FindComponent<collidable>(paperback::vm::PoolDetails{ 0, m_SelectedEntity.second });

            paperback::u8 Layer = Result.get_value<paperback::u8>();
            EntityCollideLayers->Set(Layer); //need to test
        }
        else
            Property.set_value(Instance, Result);
    }

    void DisplayClassType(const std::string& PropertyName, rttr::type& PropertyType, rttr::variant& PropertyValue)
    {
        if (PropertyType == rttr::type::get<std::reference_wrapper<paperback::Vector3f>>())
        {
            ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
            ImGui::DragFloat3(("##" + PropertyName).c_str(), (float*)&(PropertyValue.get_value<std::reference_wrapper<paperback::Vector3f>>().get()), 0.1f, 0.1f);
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

        else if (PropertyType == rttr::type::get<int>() || PropertyType == rttr::type::get<std::reference_wrapper<int>>())
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

        else if (PropertyType == rttr::type::get<bool>() || PropertyType == rttr::type::get <std::reference_wrapper<bool>>())
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

        else if (PropertyType == rttr::type::get<size_t>())
        {
            ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
            ImGui::Text("%d", PropertyValue.get_value<size_t>());
        }

        else if (PropertyType == rttr::type::get<paperback::u32>() /*|| PropertyType == rttr::type::get <std::reference_wrapper<paperback::u32>>()*/)
        {
            if (!PropertyType.is_wrapper())
            {
                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                ImGui::Text("%d", PropertyValue.get_value<paperback::u32>());
            }
            //else
            //{
            //    ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
            //    ImGui::PushItemWidth(200.0f);
            //    ImGui::InputScalar(("##" + PropertyName).c_str(), ImGuiDataType_U32, &(PropertyValue.get_value<std::reference_wrapper<paperback::u32>>().get()), (int*)1);
            //    ImGui::PopItemWidth();
            //}
        }

        else if (PropertyType == rttr::type::get<paperback::i32>() || PropertyType == rttr::type::get <std::reference_wrapper<paperback::i32>>())
        {
            if (!PropertyType.is_wrapper())
            {
                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                ImGui::Text("%d", PropertyValue.get_value<paperback::i32>());
            }
            else
            {
                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                ImGui::PushItemWidth(200.0f);
                ImGui::InputInt(("##" + PropertyName).c_str(), &(PropertyValue.get_value<std::reference_wrapper<paperback::i32>>().get()), 1);
                ImGui::PopItemWidth();
            }
        }

    }

    //-----------------------------------
    //        Helper Fns
    //-----------------------------------

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

    std::string SetEntityInfoPath(std::string& Path, std::string& FileName)
    {
        std::string a = Path.substr(0, Path.find(FileName.c_str()));
        return a.append(FileName.substr(0, FileName.find(".json")).append("_EntityInfo.json"));
    }

    bool SetEditorMode(bool Editor)
    {
        if (Editor)
            return ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
        else
            return false;

    }

    //-----------------------------------
    //   Functions Used between Panels
    //-----------------------------------

    void PopUpMessage(const std::string& WindowName, const char* Message) {

        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(WindowName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

            ImGui::Text(Message);

            if (ImGui::Button("OK"))
                ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }
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

    void UpdateComponents(paperback::u32 EntityGlobalIndex)
    {
        //Get Entity Info

        auto& EntityInfo = PPB.GetEntityInfo(EntityGlobalIndex);

        m_SelectedEntity.first = EntityInfo.m_pArchetype;
        m_SelectedEntity.second = EntityInfo.m_PoolDetails.m_PoolIndex;
        m_Components = m_SelectedEntity.first->GetEntityComponents(m_SelectedEntity.second);
    }

    void LinkParentChild(paperback::component::entity& Entity, parent& NewParent, paperback::component::entity NewParentEntity)
    {
        paperback::u32 OldParentGID;
        std::array<const paperback::component::info*, 1 > ComponentAdd;

        auto& EntityInfo = PPB.GetEntityInfo(Entity.m_GlobalIndex);

        if (EntityInfo.m_pArchetype->FindComponent<child>(EntityInfo.m_PoolDetails) != nullptr) //Check for Child Component
        {
            //Get the Child Component
            auto& Child = EntityInfo.m_pArchetype->GetComponent<child>(EntityInfo.m_PoolDetails);
            OldParentGID = Child.m_ParentGlobalIndex; //Get Old Parent GID

            //Check if already attached to a parent
            if (OldParentGID != NewParentEntity.m_GlobalIndex)
            {
                auto& OldParent = PPB.GetEntityInfo(OldParentGID);

                //Old Parent remove child
                OldParent.m_pArchetype->GetComponent<parent>(OldParent.m_PoolDetails).RemoveChild(Entity.m_GlobalIndex);
            }

            //if (OldParentGID == Entity.m_GlobalIndex) //check if parent entity matches the current selected child
            //{
            //    EDITOR_WARN_PRINT("Unable to add entity as a child");
            //    return;
            //}

            //Link Child to Existing Parent
            NewParent.AddChild(Entity.m_GlobalIndex);
            //Update Child's Parent GID
            Child.AddParent(NewParentEntity.m_GlobalIndex);
        }
        else
        {
            //save new child GID
            auto EntityIndex = Entity.m_GlobalIndex;
            //Link Child to Existing Parent
            NewParent.AddChild(EntityIndex);

            //Add in child component
            ComponentAdd[0] = &paperback::component::info_v<child>;
            PPB.AddOrRemoveComponents(Entity, ComponentAdd, {});

            //Add Parent GID to Child
            auto& Info = PPB.GetEntityInfo(EntityIndex);
            Info.m_pArchetype->GetComponent<child>(Info.m_PoolDetails).AddParent(NewParentEntity.m_GlobalIndex);
        }
    }

    void DisplayChildEntities(parent& Parent/*, bool DisplayEntity */)
    {
        int Index = 0;
        paperback::u32 ChildToUnlink;
        std::array<const paperback::component::info*, 1 > ComponentRemove;

        paperback::archetype::instance* SelectedChild = nullptr;
        paperback::u32 ChildIndex = paperback::u32_max;
        std::string ChildName{};

        bool b_NodeOpen = false, Unlink = false;

        if (Parent.m_ChildrenGlobalIndexes.size() != 0)
        {
            for (auto& Child : Parent.m_ChildrenGlobalIndexes)
            {
                Index++;

                auto& ChildEntityInfo = PPB.GetEntityInfo(Child); //get the entity info

                ImGuiTreeNodeFlags NodeFlags = ((m_SelectedEntity.first == ChildEntityInfo.m_pArchetype && m_SelectedEntity.second == ChildEntityInfo.m_PoolDetails.m_PoolIndex) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

                auto ChildParent = ChildEntityInfo.m_pArchetype->FindComponent<parent>(ChildEntityInfo.m_PoolDetails);
                auto Name = ChildEntityInfo.m_pArchetype->FindComponent<name>(ChildEntityInfo.m_PoolDetails);

                if (ChildParent)
                    NodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
                else
                    NodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

                if (Name)
                    ChildName = Name->m_Value;
                else
                {
                    ComponentRemove[0] = &paperback::component::info_v<name>;
                    PPB.AddOrRemoveComponents(ChildEntityInfo.m_pArchetype->GetComponent<paperback::component::entity>(ChildEntityInfo.m_PoolDetails), ComponentRemove, {});

                    if (!m_Components.empty())
                        UpdateComponents(Child);

                }

                b_NodeOpen = ImGui::TreeNodeEx((char*)("##" + ChildEntityInfo.m_pArchetype->GetName() + std::to_string(ChildEntityInfo.m_PoolDetails.m_PoolIndex) + std::to_string(Index)).c_str(),
                    NodeFlags, /*(ChildEntityInfo.m_pArchetype->GetName() + " [" + std::to_string(ChildEntityInfo.m_PoolDetails.m_PoolIndex) + "]").c_str()*/ ChildName.c_str());

                if (ImGui::IsItemClicked())
                {
                    m_SelectedEntity.first = ChildEntityInfo.m_pArchetype;
                    m_SelectedEntity.second = ChildEntityInfo.m_PoolDetails.m_PoolIndex;
                    m_Components = m_SelectedEntity.first->GetEntityComponents(m_SelectedEntity.second);
                }

                //bool Deleted = false;

                if (ImGui::BeginPopupContextItem())
                {
                    //if (DisplayEntity)
                    //{
                    //    if (ImGui::MenuItem(ICON_FA_TRASH "Delete?"))
                    //    {
                    //        m_Imgui.m_SelectedEntity.first = ChildEntityInfo.m_pArchetype;
                    //        m_Imgui.m_SelectedEntity.second = ChildEntityInfo.m_PoolDetails.m_PoolIndex;
                    //        Deleted = true;
                    //    }
                    //}

                    if (ImGui::MenuItem(ICON_FA_TRASH "UnParent?"))
                    {
                        ChildToUnlink = Child;

                        SelectedChild = ChildEntityInfo.m_pArchetype;
                        ChildIndex = ChildEntityInfo.m_PoolDetails.m_PoolIndex;

                        Unlink = true;
                    }

                    ImGui::EndPopup();
                }

                if (b_NodeOpen)
                {
                    if (ChildParent)
                        DisplayChildEntities(*ChildParent);

                    ImGui::TreePop();
                }

                //if (Deleted)
                //    ImGui::OpenPopup(ICON_FA_TRASH " Delete?");

            }

            //DeleteEntity(ICON_FA_TRASH " Delete?", m_SelectedEntity.second);

            if (Unlink && SelectedChild)
            {
                Parent.RemoveChild(ChildToUnlink);
                ComponentRemove[0] = &paperback::component::info_v<child>;

                auto& Entity = SelectedChild->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, ChildIndex });
                PPB.AddOrRemoveComponents(Entity, {}, ComponentRemove);

                SelectedChild = nullptr;
                ChildIndex = {};

                if (m_SelectedEntity.first)
                {
                    m_SelectedEntity.first = nullptr;
                    m_SelectedEntity.second = paperback::u32_max;
                }

                if (!m_Components.empty())
                    UpdateComponents(Entity.m_GlobalIndex);


                Unlink = false;
            }
        }
    }
};

#include "Editor/Panels/EntityInspector_Inline.h"
#include "Editor/Panels/PropertyInspector_Inline.h"
#include "Editor/Panels/ArchetypeInspector_Inline.h"
#include "Editor/Panels/ConsoleTerminal_Inline.h"
#include "Editor/Panels/AssetBrowser_Inline.h"
#include "Editor/Panels/Viewport_Inline.h"
#include "Editor/Panels/WindowSettings_Inline.h"