#pragma once
#include "Editor/EditorInclude.h"
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

struct AssetBrowser : paperback::editor::instance
{
    bool m_bCreate;
    fs::path m_SelectedPath;
    std::string m_SelectedFile;

    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Asset Browser"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = true;
        m_bCreate = false;

        m_SelectedPath = "../../resources";
        m_SelectedFile = {};
    }

    void Update(void) noexcept
    {
        Panel();
        MakeNewFolder();
    }

    void Panel()
    {
        float windowW = ImGui::GetContentRegionAvailWidth(), windowH = ImGui::GetContentRegionAvail().y;

        ImGui::Begin(AssetBrowser::typedef_v.m_pName);

        DisplayFolders( windowW, windowH );

        ImGui::SameLine();

        DisplayFolderFiles(windowW, windowH);

        ImGui::End();
    }

    void DisplayFolders(float window_width, float window_height) {

        bool Opened = false;

        // Print out all directories
        ImGui::BeginChild("File Directories", { window_width / 7, window_height }, true);

        ImGui::Text("Folders/Directories:");

        //if (ImGui::Selectable(ICON_FA_FOLDER " resources"))
        //    m_SelectedPath = "../..resources";
        ImGuiTreeNodeFlags Flags = (( m_SelectedPath == "../../resources" ) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

        Opened = ImGui::TreeNodeEx((ICON_FA_FOLDER " resources"), Flags);

        if (ImGui::IsItemClicked())
            m_SelectedPath = "../../resources";

        if ( Opened )
        {
            FileDirectoryCheck("../../resources");

            ImGui::TreePop();
        }

        ImGui::EndChild();
    }

    void DisplayFolderFiles(float window_width, float window_height) {

        ImGui::BeginChild("Files", { window_width / 3 , window_height }, true, ImGuiWindowFlags_MenuBar);
        ImGui::Text("Current Path:"); ImGui::SameLine(0, 3);
        ImGui::Text(FileString(ICON_FA_FOLDER_OPEN, m_SelectedPath.generic_string()).c_str());

        FileMenuBar();
        CheckFileType();

        ImGui::EndChild();
    }

    void FileMenuBar() {
        //std::string path{};
        ImGui::BeginMenuBar();
        //if (ImGui::MenuItem(ICON_FA_FOLDER_PLUS)) {
        //    m_bCreate = true;
        //}
        ////imgui_->ImguiHelp("Add New Folder", 0);

        ////if (ImGui::MenuItem(ICON_FA_COPY) && !path_selection_.empty())
        ////    CopyFilesDirectory();


        ImGui::EndMenuBar();
    }


    void FileDirectoryCheck( fs::path FileDirectory )
    {
        size_t Counter = 0;
        bool Opened = false;

        for ( auto& Directory : fs::directory_iterator( FileDirectory ) )
        {
            ++Counter;

            if ( fs::is_directory( Directory ))
            {
                ImGuiTreeNodeFlags Flags = ( ( m_SelectedPath == Directory ) ? ImGuiTreeNodeFlags_Selected : 0 ) | ImGuiTreeNodeFlags_OpenOnArrow;

                std::string Dir = DirectoryName(Directory);

                Opened = ImGui::TreeNodeEx((void*)(size_t)Counter, Flags, FileString(ICON_FA_FOLDER, Dir).c_str());

                if (ImGui::IsItemClicked())
                    m_SelectedPath = Directory;

                if (Opened)
                {
                    FileDirectoryCheck( Directory );
                    ImGui::TreePop();
                }
            }
        }
    }

    void CheckFileType()
    {
        std::string FileName{};

        static ImGuiTextFilter Filter;
        Filter.Draw( ICON_FA_FILTER, 200.0f );

        if ( !m_SelectedPath.empty() )
        {
            for ( auto& File : fs::directory_iterator(m_SelectedPath) )
            {
                if ( fs::is_regular_file( File ))
                {
                    FileName = File.path().filename().generic_string();

                    if (Filter.PassFilter(FileName.c_str()))
                    {
                        if (File.path().extension() == ".png")
                            ImGui::Text(FileString(ICON_FA_FILE_IMAGE, FileName).c_str());
                        else if (File.path().extension() == ".json" || File.path().extension() == ".nui")
                            ImGui::Text(FileString(ICON_FA_FILE_CODE, FileName).c_str());
                        else if (File.path().extension() == ".nui")
                            ImGui::Text(FileString(ICON_FA_KIWI_BIRD, FileName).c_str());
                        else if (File.path().extension() == ".mp3" || File.path().extension() == ".wav")
                            ImGui::Text(FileString(ICON_FA_FILE_AUDIO, FileName).c_str());
                        else if (File.path().extension() == ".txt")
                            ImGui::Text(FileString(ICON_FA_FILE_WORD, FileName).c_str());
                        else if (File.path().extension() == ".ttf")
                            ImGui::Text(FileString(ICON_FA_FONT, FileName).c_str());
                        else
                            ImGui::Text(FileString(ICON_FA_FILE, FileName).c_str()); // will update when needed
                    }
                }

                if (fs::is_directory(File)) {

                    if (ImGui::Selectable(FileString(ICON_FA_FOLDER, DirectoryName(File)).c_str()))
                        m_SelectedPath = File;
                }

            }
        }
    }

    void MakeNewFolder() {

        if (m_bCreate)
            ImGui::OpenPopup("Add Folder");

        if (ImGui::BeginPopup("Add Folder")) {

            std::string folderName;
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), folderName.c_str());
            ImGui::Text(ICON_FA_FOLDER " Adding a New Folder to: "); ImGui::SameLine(0, 3);
            ImGui::Text(m_SelectedPath.generic_string().c_str());
            if (ImGui::InputText("##newfile", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {

                if (!m_SelectedPath.empty())
                    fs::create_directory(m_SelectedPath / buffer);
                else
                    fs::create_directory((fs::path)"../../resources" / buffer);

                m_bCreate = false;

                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::Button( "Cancel") )
                ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }
    }

    std::string DirectoryName(fs::directory_entry Directory) {

        std::string tempPath = Directory.path().generic_string();
        return tempPath.substr(tempPath.find_last_of("/") + 1);
    }

    std::string FileString(std::string icon, std::string file_name) {

        return (icon + std::string(" ") + file_name);
    }
};