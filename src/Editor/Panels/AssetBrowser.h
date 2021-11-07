#pragma once
#include "Editor/EditorInclude.h"
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

struct AssetBrowser : paperback::editor::instance
{
    bool m_bCreate = false, m_bDeleteFolder = false, m_bDeleteFile = false, m_bDelete = false;

    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Asset Browser"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = true;
    }

    void Update(void) noexcept
    {
        Panel();
        MakeNewFolder();
    }

    void Panel();

    void DisplayFolders( float window_width, float window_height );

    void DisplayFolderFiles( float window_width, float window_height );

    void FileDirectoryCheck( fs::path FileDirectory );

    void DisplayFiles( fs::path File, std::string FileName );

    void FileTabBar();

    void CheckFileType();

    void MakeNewFolder();

    void FolderName( fs::path Path, std::deque<std::pair<std::string, fs::path>>& Folders );

    void DeleteFolderContents();

    void DragDropExternal();

    std::string DirectoryName( fs::directory_entry Directory );

    std::string FileString( std::string icon, std::string file_name );

};