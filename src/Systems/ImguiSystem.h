#pragma once
#include "paperback_pch.h"
#include "Systems/WindowSystem.h"

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

struct imgui_system : paperback::system::instance
{
    GLFWwindow* m_pWindow;
    ImFont* m_Imgfont;

    paperback::archetype::instance* m_pArchetype; //refers back to the archetype that the entity is referencing to

    std::vector <rttr::instance> m_Components = {};
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
        ImGuiContext(); //Setup ImGui Context

        m_bImgui = true;
        m_bDemoWindow = m_bFileSave = m_bFileOpen = m_bFileSaveAs = false;
        m_FilePath = m_FileName = m_LoadedPath = {};

        m_SelectedEntity = { nullptr, paperback::u32_max };
        m_pArchetype = nullptr;
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
                ImGuiDockingSetup();

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::SetNextWindowBgAlpha(0.0f); // set the transparency of the docking central node
                ImGui::Begin("DockSpace", &m_bDockspaceopen, m_Windowflags);
                ImGui::PopStyleVar();

                if (m_bFullscreen)
                    ImGui::PopStyleVar(2);

                EditorMenuBar();
                OpenSaveFile();
                PopUps();

                if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {

                    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
                    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), m_Dockspaceflags);
                }


                //Call Windows Here
                ImGui::PushFont(m_Imgfont);

                ArchetypeList();
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

    void EditorMenuBar()
    {
        if (ImGui::BeginMenuBar())
        {
            ImGui::PushFont(m_Imgfont);

            if (ImGui::BeginMenu(ICON_FA_FOLDER " File"))
            {
                if (ImGui::MenuItem(ICON_FA_TIMES " New Scene"))
                {

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
                    //m_bImgui = false;
                }

                if (ImGui::MenuItem(ICON_FA_POWER_OFF " Exit"))
                {
                    //m_bImgui = false;
                }

                ImGui::EndMenu();
            }

            ImGui::PopFont();

            ImGui::Text("FPS: %d", PPB.GetFPS());
        }

        //if (ImGui::Selectable("Show Demo Window"))
        //    m_bDemoWindow = !m_bDemoWindow;

        ImGui::EndMenuBar();
    }

    void InspectorPanel()
    {
        int NumEntities = 0;

        ImGui::Begin("Entity Inspector");

        bool b_NodeOpen{ false };

        for (auto& Archetype : PPB.m_EntityMgr.m_pArchetypeList)
        {
            for (paperback::u32 i = 0; i < Archetype->m_EntityCount; ++i)
            {
                NumEntities++;

                ImGuiTreeNodeFlags NodeFlags = ((m_SelectedEntity.first == Archetype.get() && m_SelectedEntity.second == i) ? ImGuiTreeNodeFlags_Selected : 0); //update this
                NodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

                b_NodeOpen = ImGui::TreeNodeEx((void*)(paperback::u32)i, NodeFlags, (Archetype->m_pName + "(" + std::to_string(i) + ")").c_str());

                if (ImGui::IsItemClicked())
                {
                    m_SelectedEntity.first = Archetype.get();
                    m_SelectedEntity.second = i;

                    m_Components = Archetype->GetEntityComponents(m_SelectedEntity.second);
                }

                if (b_NodeOpen)
                {
                    if (m_SelectedEntity.first)
                    {
                        if (ImGui::Button("Spawn Entity"))
                        {
                            //m_SelectedEntity.first->CreateEntity();
                            const auto Details = m_SelectedEntity.first->CreateEntity();
                            PPB.m_EntityMgr.RegisterEntity( Details, *(m_SelectedEntity.first) );

                        }
                    }

                    ImGui::TreePop();
                }
                else
                    m_pArchetype = nullptr;
            }
        }

        ImGui::Separator();
        ImGui::Text("%d Entities", NumEntities);

        ImGui::End();
    }

    void ArchetypeList()
    {
        int Index = 0;
        std::string ArchetypeName;
        char Buffer[256];

        ImGui::Begin("PreFabs");

        static ImGuiTextFilter Filter;
        Filter.Draw(ICON_FA_FILTER, 150.0f);

        for (auto& Archetype : PPB.m_EntityMgr.m_pArchetypeList)
        {
            ++Index;

            ArchetypeName = Archetype->m_pName;
            memset(Buffer, 0, sizeof(Buffer));
            strcpy_s(Buffer, sizeof(Buffer), ArchetypeName.c_str());

            if (Filter.PassFilter(ArchetypeName.c_str()))
            {

                if (ImGui::InputText(("##ArchetypeName" + std::to_string(Index)).c_str(), Buffer, IM_ARRAYSIZE(Buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    Buffer[std::string(Buffer).length()] = '\0';
                    Archetype->m_pName = std::string(Buffer);
                }

                ImGui::SameLine();

                if (ImGui::Button(ICON_FA_PLUS_SQUARE))
                {
                    m_pArchetype = Archetype.get();

                    if (m_pArchetype)
                        m_pArchetype->CreateEntity();
                }
            }
        }

        ImGui::End();
    }

    void ComponentInspector()
    {
        ImGui::Begin("Component Inspector");

        static ImGuiTextFilter Filter;
        Filter.Draw(ICON_FA_FILTER, 150.0f);

        if (!m_Components.empty())
        {
            for (auto& ComponentInstance : m_Components)
            {
                if (Filter.PassFilter(ComponentInstance.get_type().get_name().to_string().c_str()))
                {
                    if (ImGui::CollapsingHeader(ComponentInstance.get_type().get_name().to_string().c_str()))
                    {
                        for (auto& property : ComponentInstance.get_type().get_properties())
                        {
                            rttr::variant propValue = property.get_value(ComponentInstance);

                            if (!propValue) continue;

                            auto PropertyType = property.get_type(); //etc vector 3, std::string etc
                            auto PropertyName = property.get_name().to_string();

                            //PropertyName.assign(PropertyName + ComponentInstance.get_type().get_name().to_string());
                            if (PropertyType == rttr::type::get <std::reference_wrapper<float>>())
                            {
                                ImGui::Text(PropertyName.c_str());
                                ImGui::DragFloat(("##" + PropertyName).c_str(), &(propValue.get_value<std::reference_wrapper<float>>().get()), 0.01f);
                            }

                            if (PropertyType == rttr::type::get<std::reference_wrapper<int>>())
                            {
                                ImGui::Text(PropertyName.c_str());
                                ImGui::DragInt(("##" + PropertyName).c_str(), &(propValue.get_value<std::reference_wrapper<int>>().get()), 1);
                            }

                            if (PropertyType == rttr::type::get<std::reference_wrapper<xcore::vector3>>())
                            {
                                DrawVec3(PropertyName, propValue.get_value<std::reference_wrapper<xcore::vector3>>().get(), 0.0f, 70.0f);
                            }

                            if (PropertyType == rttr::type::get<std::string>())
                            {
                                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                                ImGui::Text(propValue.get_value<std::string>().c_str());
                            }

                            if (PropertyType == rttr::type::get<paperback::component::entity::Validation>())
                            {
                                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                                ImGui::Text("%d", propValue.get_value<paperback::component::entity::Validation>());
                            }

                            if (PropertyType == rttr::type::get<uint32_t>())
                            {
                                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                                ImGui::Text("%d", propValue.get_value<uint32_t>());
                            }

                            if (PropertyType == rttr::type::get<bool>())
                            {
                                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                                ImGui::Text("%d", propValue.get_value<bool>());
                            }

                            if (PropertyType == rttr::type::get<size_t>())
                            {
                                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                                ImGui::Text("%d", propValue.get_value<size_t>());

                            }
                        }
                    }
                }
            }
        }

        ImGui::End();
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

    void DrawVec3(const std::string& Label, xcore::vector3& Values, float ResetValue = 0.0f, float ColumnWidth = 100.0f)
    {
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, ColumnWidth);
        ImGui::Text(Label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

        float LineHeight = ImGui::GetIO().FontDefault->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImVec2 ButtonSize = { LineHeight + 3.0f, LineHeight };


        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.773f, 0.027f, 0.067f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.965f, 0.075f, 0.118f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.773f, 0.027f, 0.067f, 1.0f });

        if (ImGui::Button(("X##" + Label + "x").c_str(), ButtonSize))
            Values.m_X = ResetValue;

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat(("##" + Label + "x").c_str(), &Values.m_X, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.294f, 0.804f, 0.075f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.361f, 0.918f, 0.122f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.216f, 0.584f, 0.055f, 1.0f });

        if (ImGui::Button(("Y##" + Label + "y").c_str(), ButtonSize))
            Values.m_Y = ResetValue;

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat(("##" + Label + "y").c_str(), &Values.m_Y, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.141f, 0.176f, 0.839f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.318f, 0.345f, 0.882f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.118f, 0.145f, 0.682f, 1.0f });

        if (ImGui::Button(("Z##" + Label + "z").c_str(), ButtonSize))
            Values.m_Z = ResetValue;

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat(("##" + Label + "z").c_str(), &Values.m_Z, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::Columns(1);
    }

    void PopUps()
    {
        if (m_bFileSave)
            ImGui::OpenPopup("Save Confirmation");

        PopUpMessage("Save Confirmation", ("File " + m_FileName + "\n" + "Saved at: " + m_FilePath).c_str());
        m_bFileSave = false;
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







































    PPB_INLINE
        void Terminated(void) noexcept
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
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