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
#include "Functionality/Renderer/Renderer.h"

struct imgui_system : paperback::system::instance
{
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
    bool m_bFileSave, m_bFileOpen, m_bFileSaveAs, m_bNodeOpen, m_bAdd, m_bRemove, m_bWinSys;

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
        m_bDemoWindow = m_bFileSave = m_bFileOpen = m_bFileSaveAs = m_bNodeOpen = m_bAdd = m_bRemove = m_bWinSys = false;
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
                AddComponents();

                if (m_bWinSys)
                    WindowSettings();

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

            if (ImGui::MenuItem("Window Settings"))
                m_bWinSys = !m_bWinSys;

            ImGui::PopFont();

            ImGui::Text("FPS: %d", PPB.GetFPS());
        }

        //if (ImGui::Selectable("Show Demo Window"))
        //    m_bDemoWindow = !m_bDemoWindow;

        ImGui::EndMenuBar();
    }

    void InspectorPanel()
    {
        int NumEntities = 0, Index = 0;

        ImGui::Begin("Entity Inspector");

        if (!PPB.GetArchetypeList().empty()) {

            for (auto& Archetype : PPB.GetArchetypeList())
            {
                Index++;

                for (paperback::u32 i = 0; i < Archetype->GetEntityCount(); ++i)
                {
                    NumEntities++;

                    ImGuiTreeNodeFlags NodeFlags = ((m_SelectedEntity.first == Archetype && m_SelectedEntity.second == i) ? ImGuiTreeNodeFlags_Selected : 0); //update this
                    NodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

                    std::stringstream Label; Label << Archetype->GetName() << "(" << std::to_string(i) << ")";

                    m_bNodeOpen = ImGui::TreeNodeEx((char*)("##" + Archetype->GetName() + " [" + std::to_string(i) + std::to_string(Index) + "]").c_str(), NodeFlags, Label.str().c_str());

                    if (ImGui::IsItemClicked())
                    {
                        m_SelectedEntity.first = Archetype;
                        m_SelectedEntity.second = i;

                        m_Components = Archetype->GetEntityComponents(m_SelectedEntity.second);
                    }

                    if (m_bNodeOpen)
                    {
                        m_SelectedEntity.first = Archetype;
                        m_SelectedEntity.second = i;
                        

                        if (m_SelectedEntity.first)
                        {
                            if (ImGui::Button(ICON_FA_TRASH " Delete Entity"))
                            {
                                ImGui::OpenPopup(ICON_FA_TRASH " Delete?");
                            }

                            DeleteEntity(ICON_FA_TRASH " Delete?", i, Label.str());

                            ImGui::Checkbox("Add Component(s)", &m_bAdd);
                            ImGui::Checkbox("Remove Component(s)", &m_bRemove);

                        }

                        ImGui::TreePop();
                    }
                }
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

        ImGui::BeginChild("Archetypes", { ImGui::GetContentRegionAvailWidth() / 2, ImGui::GetContentRegionAvail().y }, true);

        for (auto& Archetype : PPB.GetArchetypeList())
        {
            ++Index;

            ArchetypeName = Archetype->GetName();
            memset(Buffer, 0, sizeof(Buffer));
            strcpy_s(Buffer, sizeof(Buffer), ArchetypeName.c_str());


            if (Filter.PassFilter(ArchetypeName.c_str()))
            {
                ImGui::PushItemWidth(150.0f);

                if (ImGui::InputText(("##ArchetypeName" + std::to_string(Index)).c_str(), Buffer, IM_ARRAYSIZE(Buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    Buffer[std::string(Buffer).length()] = '\0';
                    Archetype->SetName(std::string(Buffer));
                }

                ImGui::PopItemWidth();

                if (ImGui::IsItemClicked())
                {
                    m_pArchetype = Archetype;
                    m_ComponentNames.clear();

                    for (paperback::u32 i = 0; i < Archetype->GetComponentNumber(); ++i)
                    {
                        m_ComponentNames.push_back(Archetype->GetComponentInfos()[i]->m_pName);
                    }
                }
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Components", { ImGui::GetContentRegionAvailWidth() , ImGui::GetContentRegionAvail().y });

        if (!m_ComponentNames.empty() && m_pArchetype)
        {
            ImGui::Text("Number of Entities: %d", m_pArchetype->GetEntityCount());

            if (ImGui::Button("Clone new Entity"))
            {
                m_pArchetype->CreateEntity();
            }

            ImGui::Separator();

            ImGui::Text("Archetype Components: ");

            for (auto& Names : m_ComponentNames)
            {
                ImGui::Text(Names);
            }
        }

        ImGui::EndChild();


        ImGui::End();
    }

    void WindowSettings()
    {
        ImGui::Begin("Window Settings");

        rttr::instance obj = GetSystem< window_system >().E;

        for (auto& Property : obj.get_type().get_properties())
        {
            rttr::variant propValue = Property.get_value(obj);

            auto PropertyType = Property.get_type(); //etc vector 3, std::string etc
            auto PropertyName = Property.get_name().to_string();

            if (PropertyType == rttr::type::get<std::reference_wrapper<int>>())
            {
                auto& a = propValue.get_value<std::reference_wrapper<int>>().get();

                ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
                ImGui::InputInt(("##" + PropertyName + obj.get_type().get_name().to_string()).c_str(), &a, 1);
            }

            if (PropertyType == rttr::type::get<std::reference_wrapper<std::string>>())
            {
                auto& str = propValue.get_value<std::reference_wrapper<std::string>>().get();
                char buffer[256]{};

                std::copy(str.begin(), str.end(), buffer);
                if (ImGui::InputText(("##" + PropertyName).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                    str = std::string(buffer);
            }
           
            glfwSetWindowSize(m_pWindow, GetSystem< window_system >().E.m_Width, GetSystem< window_system >().E.m_Height);
            Renderer::GetInstanced().UpdateFramebufferSize(GetSystem< window_system >().E.m_Width, GetSystem< window_system >().E.m_Height);
        }

        if (ImGui::Button ("Save"))
        {
            JFile.StartWriter("../../resources/assetloading/config.json").ReadObjects(GetSystem< window_system >().E).EndWriter();

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
            ImGui::Separator();

            for (auto& ComponentInstance : m_Components)
            {
                if (Filter.PassFilter(ComponentInstance.get_type().get_name().to_string().c_str()))
                {
                    if (ImGui::CollapsingHeader(ComponentInstance.get_type().get_name().to_string().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        for (auto& property : ComponentInstance.get_type().get_properties())
                        {
                            rttr::variant propValue = property.get_value(ComponentInstance);

                            if (!propValue) continue;

                            auto PropertyType = property.get_type(); //etc vector 3, std::string etc
                            auto PropertyName = property.get_name().to_string();

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
                                 ImGui::DragFloat3(("##" + PropertyName).c_str(), (float*)&(propValue.get_value<std::reference_wrapper<xcore::vector3>>().get()));
                             }

                            if (PropertyType == rttr::type::get<std::reference_wrapper<paperback::Vector3f>>())
                            {
                                DrawVec3(PropertyName, propValue.get_value<std::reference_wrapper<paperback::Vector3f>>().get(), 0.0f, 70.0f);
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

    void AddComponents()
    {
        std::array<const paperback::component::info*, 1 > ComponentAdd;
        std::array<const paperback::component::info*, 1 > ComponentRemove;
        if (m_SelectedEntity.first)
        {
            ImGui::Begin( "Component Management");

            if (m_bAdd)
            {
                if (ImGui::BeginCombo("##AddComponents", "Available Components"))
                {
                    for (auto& [CompKey, CompValue] : PPB.GetComponentInfoMap())
                    {
                        if (!m_SelectedEntity.first->CheckComponentExistence(CompValue))
                        {
                            if (ImGui::Selectable(CompValue->m_pName))
                            {
                                ComponentAdd[0] = CompValue;
                                ComponentRemove[0] = &paperback::component::info_v<transform>;

                                PPB.AddOrRemoveComponents(m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_SelectedEntity.second }), ComponentAdd, {});

                                m_SelectedEntity.first = nullptr;
                                m_SelectedEntity.second = paperback::u32_max;

                                if (!m_Components.empty())
                                {
                                    m_Components.clear();
                                    ImGui::EndCombo();
                                    ImGui::End();
                                    return;
                                }
                                else
                                {
                                    ImGui::EndCombo();
                                    ImGui::End();
                                    return;
                                }
                            }
                        }
                    }

                    ImGui::EndCombo();
                }
            }


            if (m_bRemove)
            {
                if (ImGui::BeginCombo("##RemoveComponent", "Current Components"))
                {
                    for (paperback::u32 i = 0; i < m_SelectedEntity.first->GetComponentNumber(); ++i)
                    {
                        if (ImGui::Selectable(m_SelectedEntity.first->GetComponentInfos()[i]->m_pName))
                        {
                            ComponentRemove[0] = (m_SelectedEntity.first->GetComponentInfos()[i]);
                            PPB.AddOrRemoveComponents(m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_SelectedEntity.second }), {}, ComponentRemove);
            
                            m_SelectedEntity.first = nullptr;
                            m_SelectedEntity.second = paperback::u32_max;
            
                            if (!m_Components.empty())
                            {
                                m_Components.clear();
                                ImGui::EndCombo();
                                ImGui::End();
                                return;
                            }
                            else
                            {
                                ImGui::EndCombo();
                                ImGui::End();
                                return;
                            }
                        }
                    }

                    ImGui::EndCombo();
                }

            }
            
            ImGui::End();
        }
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

    void DrawVec3(const std::string& Label, paperback::Vector3f& Values, float ResetValue = 0.0f, float ColumnWidth = 100.0f)
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
            Values.x = ResetValue;

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat(("##" + Label + "x").c_str(), &Values.x, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.294f, 0.804f, 0.075f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.361f, 0.918f, 0.122f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.216f, 0.584f, 0.055f, 1.0f });

        if (ImGui::Button(("Y##" + Label + "y").c_str(), ButtonSize))
            Values.y = ResetValue;

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat(("##" + Label + "y").c_str(), &Values.y, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.141f, 0.176f, 0.839f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.318f, 0.345f, 0.882f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.118f, 0.145f, 0.682f, 1.0f });

        if (ImGui::Button(("Z##" + Label + "z").c_str(), ButtonSize))
            Values.z = ResetValue;

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat(("##" + Label + "z").c_str(), &Values.z, 0.1f);
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

    void DeleteEntity( std::string WindowName, paperback::u32 EntityIndex, std::string EntityLabel )
    {
        if (m_SelectedEntity.first)
        {
            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal(WindowName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "Deleting: ");

                ImGui::Text(EntityLabel.c_str());

                ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "This cannot be undone");

                ImGui::Separator();
                
                if (ImGui::Button("Yes"))
                {
                    PPB.DeleteEntity(m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, EntityIndex }));

                    m_SelectedEntity = { nullptr, paperback::u32_max };

                    if (!m_Components.empty())
                        m_Components.clear();

                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine( 0, 4 ); 

                if ( ImGui::Button("Cancel") )
                    ImGui::CloseCurrentPopup();

                ImGui::EndPopup();
            }

        }
    }

    void ImGuiHelp(const char* description, int symbol) {

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







































    PPB_INLINE
        void OnSystemTerminated(void) noexcept
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