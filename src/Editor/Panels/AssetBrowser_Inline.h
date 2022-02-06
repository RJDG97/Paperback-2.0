#include "Editor/Panels/AssetBrowser.h"
#include "Systems/ImguiSystem.h"
#include "../../Functionality/RenderResource/RenderResourceLoader.h"
#pragma once

void AssetBrowser::Panel()
{
    float windowW = ImGui::GetContentRegionAvailWidth(), windowH = ImGui::GetContentRegionAvail().y;

    ImGui::Begin(AssetBrowser::typedef_v.m_pName, &m_bEnabled, ImGuiWindowFlags_MenuBar);

    FileTabBar();

    DisplayFolders(windowW, windowH);

    ImGui::SameLine();

    DisplayFolderFiles(windowW, windowH);

    DragDropExternal();

    ImGui::End();
}

void AssetBrowser::DisplayFolders(float window_width, float window_height)
{
    bool Opened = false;

    // Print out all directories
    ImGui::BeginChild("File Directories", { window_width / 8, window_height }, true);

    ImGui::Text("Folders/Directories:");

    ImGuiTreeNodeFlags Flags = ((m_Imgui.m_SelectedPath == "../../resources") ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

    Opened = ImGui::TreeNodeEx((ICON_FA_FOLDER " resources"), Flags);

    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
    {
        m_Imgui.m_SelectedPath = "../../resources";

        m_Imgui.m_DisplayFilePath.clear();

        FolderName(m_Imgui.m_SelectedPath, m_Imgui.m_DisplayFilePath);
    }

    if (Opened)
    {
        FileDirectoryCheck("../../resources");
        ImGui::TreePop();
    }

    ImGui::EndChild();
}

void AssetBrowser::DisplayFolderFiles(float window_width, float window_height)
{

    ImGui::BeginChild("Files", { window_width / 5 , window_height }, true, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar);

    CheckFileType();

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("New Folder"))
            m_bCreate = true;

        ImGui::EndPopup();
    }

    ImGui::EndChild();
}

void AssetBrowser::FileDirectoryCheck(fs::path FileDirectory)
{
    size_t Counter = 0;
    bool Opened = false;

    for (auto& Directory : fs::directory_iterator(FileDirectory))
    {
        ++Counter;

        if (fs::is_directory(Directory))
        {
            ImGuiTreeNodeFlags Flags = ((m_Imgui.m_SelectedPath == Directory) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

            std::string Dir = DirectoryName(Directory);

            Opened = ImGui::TreeNodeEx((void*)(size_t)Counter, Flags, FileString(ICON_FA_FOLDER, Dir).c_str());

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
            {
                m_Imgui.m_SelectedPath = Directory;

                m_Imgui.m_DisplayFilePath.clear();

                FolderName(m_Imgui.m_SelectedPath, m_Imgui.m_DisplayFilePath);
            }

            if (Opened)
            {
                FileDirectoryCheck(Directory);
                ImGui::TreePop();
            }
        }
    }
}

void AssetBrowser::CheckFileType()
{
    std::string FileName{};

    static ImGuiTextFilter Filter;
    Filter.Draw(ICON_FA_FILTER, 200.0f);

    if (!m_Imgui.m_SelectedPath.empty() && fs::exists(m_Imgui.m_SelectedPath))
    {
        for (auto& File : fs::directory_iterator(m_Imgui.m_SelectedPath))
        {
            if (fs::is_regular_file(File))
            {
                FileName = File.path().filename().generic_string();

                if (Filter.PassFilter(FileName.c_str()))
                {
                    DisplayFiles(File.path(), FileName);

                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                    {
                        if (File.path().extension() == ".json")
                        {
                            if (File.path().filename() != "config.json") //might change
                            {
                                m_Imgui.m_LoadedPath = fs::absolute(File.path()).generic_string();
                                m_Imgui.m_LoadedFile = File.path().filename().generic_string();

                                m_Imgui.m_Type = FileActivity::OPENFROMASSET;
                                m_Imgui.m_bSaveCheck = true;
                            }
                        }

                        if (File.path().extension() == ".prefab")
                        {
                            m_Imgui.m_LoadedPrefabPath = fs::absolute(File.path()).generic_string();
                            m_Imgui.m_LoadedPrefabFile = File.path().filename().generic_string();

                            m_Imgui.m_Type = FileActivity::LOADFROMASSET;
                        }
                    }
                }
            }

            if (fs::is_directory(File))
            {

                if (ImGui::Selectable(FileString(ICON_FA_FOLDER, DirectoryName(File)).c_str()))
                {
                    m_Imgui.m_SelectedPath = File;

                    m_Imgui.m_DisplayFilePath.clear();

                    FolderName(m_Imgui.m_SelectedPath, m_Imgui.m_DisplayFilePath);
                }
            }
        }

        DeleteFolderContents();
    }
}

void AssetBrowser::DisplayFiles(fs::path File, std::string FileName)
{

    if (File.extension() == ".png" || File.extension() == ".jpg" || File.extension() == ".dds")
        ImGui::Selectable(FileString(ICON_FA_FILE_IMAGE, FileName).c_str());
    else if (File.extension() == ".json" || File.extension() == ".texture" || File.extension() == ".mesh")
        ImGui::Selectable(FileString(ICON_FA_FILE_CODE, FileName).c_str());
    else if (File.extension() == ".nui")
        ImGui::Selectable(FileString(ICON_FA_KIWI_BIRD, FileName).c_str());
    else if (File.extension() == ".mp3" || File.extension() == ".wav")
        ImGui::Selectable(FileString(ICON_FA_FILE_AUDIO, FileName).c_str());
    else if (File.extension() == ".txt")
        ImGui::Selectable(FileString(ICON_FA_FILE_WORD, FileName).c_str());
    else if (File.extension() == ".ttf")
        ImGui::Selectable(FileString(ICON_FA_FONT, FileName).c_str());
    else if (File.extension() == ".prefab")
        ImGui::Selectable(FileString(ICON_FA_CUBES, FileName).c_str());
    else
        ImGui::Text(FileString(ICON_FA_EXCLAMATION_CIRCLE, FileName).c_str()); // will update when needed

}

void AssetBrowser::FileTabBar()
{
    bool UpdatePath = false;

    ImGui::BeginMenuBar();

    if (!m_Imgui.m_DisplayFilePath.empty())
    {
        for (auto& Path : m_Imgui.m_DisplayFilePath)
        {
            ImGui::Text(ICON_FA_FOLDER_OPEN); ImGui::SameLine();

            if (ImGui::Button(Path.first.c_str()))
            {
                m_Imgui.m_SelectedPath = Path.second;

                UpdatePath = true;
            }

            ImGui::SameLine();

            if (Path != m_Imgui.m_DisplayFilePath.back())
                ImGui::Text(ICON_FA_CHEVRON_RIGHT);
        }

        if (UpdatePath)
        {
            m_Imgui.m_DisplayFilePath.clear();
            FolderName(m_Imgui.m_SelectedPath, m_Imgui.m_DisplayFilePath);
        }
    }

    ImGui::EndMenuBar();
}

void AssetBrowser::FolderName(fs::path Path, std::deque<std::pair<std::string, fs::path>>& Folders)
{
    std::string TempString{};
    fs::path TempPath{};

    if (Path.generic_string().find("/") != std::string::npos)
    {
        TempString = Path.generic_string().substr(Path.generic_string().find_last_of("/") + 1); //Get Folder Name
        Folders.push_front(std::make_pair(TempString, Path));

        TempPath = Path.generic_string().substr(0, Path.generic_string().find_last_of("/")); //Remove the already pushed Folder Name

        if (TempPath.generic_string().find("/") != std::string::npos && TempPath.generic_string() != "../..")
            FolderName(TempPath, Folders);
    }
}

void AssetBrowser::MakeNewFolder()
{
    if (m_bCreate)
        ImGui::OpenPopup("Add Folder");

    if (ImGui::BeginPopup("Add Folder")) {

        std::string folderName;
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), folderName.c_str());

        ImGui::Text(ICON_FA_FOLDER " Adding a New Folder to: "); ImGui::SameLine(0, 3);
        ImGui::Text(m_Imgui.m_SelectedPath.generic_string().c_str());

        if (ImGui::InputText("##newfile", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {

            if (!m_Imgui.m_SelectedPath.empty())
            {
                fs::create_directory(m_Imgui.m_SelectedPath / buffer);
                EDITOR_TRACE_PRINT("Folder Created: " + fs::path(m_Imgui.m_SelectedPath / buffer).generic_string());
            }
            else
            {
                fs::create_directory((fs::path)"../../resources" / buffer);
                EDITOR_TRACE_PRINT("Folder Created: " + fs::path((fs::path)"../../resources" / buffer).generic_string());
            }

            m_bCreate = false;

            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

void AssetBrowser::DeleteFolderContents()
{
    if (ImGui::BeginPopupModal(ICON_FA_TRASH " Delete?"))
    {
        if (m_bDeleteFolder)
        {
            ImGui::Text("The folder is not empty");
            ImGui::Text(("Delete: " + m_Imgui.m_FolderToDelete + " ?").c_str());

            if (ImGui::Button("Delete"))
            {
                fs::remove_all(m_Imgui.m_FolderToDelete);
                EDITOR_INFO_PRINT(m_Imgui.m_FolderToDelete + " has been deleted");

                m_Imgui.m_FolderToDelete = "{}";

                m_bDeleteFolder = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();
        }

        if (m_bDeleteFile)
        {
            ImGui::Text(("Delete: " + m_Imgui.m_FileToDelete + " ?").c_str());

            if (ImGui::Button("Delete"))
            {
                fs::remove(m_Imgui.m_FileToDelete);
                EDITOR_INFO_PRINT(m_Imgui.m_FileToDelete + " has been deleted");

                m_Imgui.m_FolderToDelete = "{}";

                m_bDeleteFile = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();
        }

        if (ImGui::Button("Cancel"))
        {
            m_bDeleteFile = false;
            m_bDeleteFolder = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    m_bDelete = false;
}

void AssetBrowser::DragDropExternal()
{
    glfwSetDropCallback(PPB.GetSystem<window_system>().m_pWindow, [](GLFWwindow* Window, int Count, const char** Paths)
        {
            auto& Files = PPB.GetDragDropFiles();

            for (int i = 0; i < Count; ++i)
                Files.push_back(Paths[i]);
        });

    if (!PPB.GetDragDropFiles().empty())
    {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceExtern))
        {
            ImGui::SetDragDropPayload("Explorer Files", nullptr, 0);
            ImGui::BeginTooltip();
            ImGui::Text("Explorer Files: %d", PPB.GetDragDropFiles().size());
            ImGui::EndTooltip();
            ImGui::EndDragDropSource();
        }
    }

    if (!m_Imgui.m_SelectedPath.generic_string().empty())
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (ImGui::AcceptDragDropPayload("Explorer Files"))
            {
                for (const auto& Path : PPB.GetDragDropFiles())
                {
                    auto Destination = m_Imgui.m_SelectedPath / Path.filename();
                    fs::copy_file(Path, Destination, fs::copy_options::overwrite_existing);

                    if ((/*m_Imgui.m_SelectedPath.parent_path() != "../../resources/textures" || */m_Imgui.m_SelectedPath != "../../resources/textures") && Path.extension() == ".dds")
                    {
                        RenderResourceLoader::GetInstanced().m_TexturesToLoad.push_back({ Path.stem().generic_string().c_str(), Destination.generic_string().c_str() });
                    }

                }
                PPB.GetDragDropFiles().clear();
            }
            ImGui::EndDragDropTarget();
        }
    }

    if (RenderResourceLoader::GetInstanced().m_TexturesToLoad.size())
        RenderResourceLoader::GetInstanced().AddNewTexture();
}


void AssetBrowser::FileMenuBar()
{
    ImGui::BeginMenuBar();

    ImGui::EndMenuBar();
}

std::string AssetBrowser::DirectoryName(fs::directory_entry Directory) {

    std::string tempPath = Directory.path().generic_string();
    return tempPath.substr(tempPath.find_last_of("/") + 1);
}

std::string AssetBrowser::FileString(std::string icon, std::string file_name) {

    return (icon + std::string(" ") + file_name);
}