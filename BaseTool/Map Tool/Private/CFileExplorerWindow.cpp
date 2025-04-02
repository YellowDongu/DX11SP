#include "pch.h"
#include "CFileExplorerWindow.h"
#include <stack>

CFileExplorerWindow::CFileExplorerWindow() : refresh(true), rootDirectory(L"../../")
{
}

CFileExplorerWindow::~CFileExplorerWindow()
{
}

HRESULT CFileExplorerWindow::CreateFrame(void)
{
    if (refresh)
    {
        node.folderName = "root";
        node.directories.clear();
        node.files.clear();
        ExploreDirectory(rootDirectory, node);
        refresh = false;
    }

    ImGui::Begin("Explorer", &active, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("Refresh##Explorer"))
        {
            refresh = true;
        }

        ImGui::EndMenuBar();
    }

    ShowFiles(node);
    ImGui::End();

	return S_OK;
}

void CFileExplorerWindow::showFiles(const FileNode& node)
{
    if (ImGui::TreeNode(node.folderName.c_str()))
    {
        for (const auto& subDirectory : node.directories)
        {
            showFiles(subDirectory);
        }

        for (const auto& file : node.files)
        {
            if (ImGui::Selectable(file.first.c_str()))
            {
                if (file.first.substr(file.first.size() - 6) == ".Scene")
                {
                    Console::WriteLine("Scene");
                }

                Console::WriteLine(file.second);
            }
        }
        ImGui::TreePop();
    }
}

void CFileExplorerWindow::ExploreDirectory(const std::wstring& directory, FileNode& node)
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + L"\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        const std::wstring filename = findFileData.cFileName;

        if (filename != L"." && filename != L"..")
        {
            const std::wstring fullPath = directory + L"\\" + filename;

            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                FileNode subDir;
                subDir.folderName = To_String(findFileData.cFileName);
                ExploreDirectory(fullPath, subDir);
                node.directories.push_back(subDir);
            }
            else
            {
                node.files.push_back({ To_String(filename), To_String(fullPath)});
            }
        }
    }
    while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void CFileExplorerWindow::ShowFiles(const FileNode& node, bool lowMemoryMode)
{
    if (lowMemoryMode)
    {
        std::stack<const FileNode*> stack;
        stack.push(&node);

        while (!stack.empty())
        {
            const FileNode* currentNode = stack.top();
            stack.pop();

            if (ImGui::TreeNode(currentNode->folderName.c_str()))
            {
                for (const auto& file : currentNode->files)
                {
                    if (ImGui::Selectable(file.first.c_str()))
                    {
                        Console::WriteLine(file.second);
                        ImGui::SetClipboardText(file.second.c_str());
                    }
                }

                for (const auto& subDirectory : currentNode->directories)
                {
                    stack.push(&subDirectory);
                }

                ImGui::TreePop();
            }
        }
    }
    else
    {
        // 재귀를 하지 않으면 이뻐지지 않는다... ㅅㅂ
        showFiles(node);
    }
}
